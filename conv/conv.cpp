// conv.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include "thread.h"
#include "image.h"


void conv(matrix &x, const matrix &k)
{
	int const NThreads = 4;
	matrix y;
	y.create(x.rows + k.rows, x.cols + k.cols);
	int const PerThread = x.rows / NThreads;
	std::vector<thread> threads;
	
	for (unsigned row = 0; row < x.rows; row++)
		for (unsigned col = 0; col < x.cols; col++)
		{
			auto yrow = row + k.rows / 2;
			auto ycol = col + k.cols / 2;
			y(yrow, ycol) = x(row, col);
		}

	// Compute sum of k
	int weight = 0;
	for (unsigned row = 0; row < k.rows; row++)
		for (unsigned col = 0; col < k.cols; col++)
			weight += k(row, col);

	// Do the convolution
	try
	{
		for (int iTh = 0; iTh < NThreads; iTh++)
		{
			threads.push_back(create_thread([iTh, PerThread,&x,&y,&k,weight]
			{
				//for (unsigned row = 0; row < x.rows; row++)
				for (unsigned row = iTh * PerThread; row < (iTh + 1) * PerThread; row++)
					for (unsigned col = 0; col < x.cols; col++)
					{
						int t = 0;
						auto yrow = row + k.rows / 2 + 1;

						for (int krow = k.rows - 1; krow >= 0; krow--, yrow++)
						{
							auto ycol = col + k.cols / 2 + 1;
							for (int kcol = k.cols - 1; kcol >= 0; kcol--, ycol++)		t += y(yrow, ycol) * k(krow, kcol);
						}

						if (weight != 0)	t /= weight;

						x(row, col) = t;
					}
			}));
		}
		for (auto &&thread : threads)	join(thread);
	}
	catch (std::system_error &ex)
	{
		printf("Error: %d\n", ex.code().value());
	}
	catch (std::exception &ex)
	{
		printf("Error: %s\n", ex.what());
	}
	catch (...)
	{
		printf("Error!\n");
	}
}


int binomial_coefficient(int n, int k)
{
	if (n <= 1 || k == 0)
		return 1;
	else
		return binomial_coefficient(n - 1, k - 1) * n / k;
}

matrix binomial(int n)
{
	if ((n & 1) == 0)	throw std::exception("n must be odd");

	matrix x, y;
	x.create(1, n);
	y.create(n, 1);

	for (int i = 0; i < n / 2; i++)
	{

		x(0, i) = x(0, n - i - 1) = binomial_coefficient(n - 1, i);
		y(i, 0) = y(n - i - 1, 0) = binomial_coefficient(n - 1, i);
	}

	x(0, n / 2) = binomial_coefficient(n - 1, n / 2);
	y(n / 2, 0) = binomial_coefficient(n - 1, n / 2);

	return y * x;
}


int main()
{
	auto bmp = load_image("Original_Pic.jpg");
	auto orig = bmp;

	matrix kernel = binomial(3);

	conv(bmp, kernel);

	save_png(bmp, "Blurred.png");

	return 0;
}


