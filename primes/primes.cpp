// primes.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include "thread.h"
#include <time.h>
#include <mutex>

std::mutex MyMutex;

bool is_prime(int n)
{
	auto j = static_cast<int>(sqrt(n));
	for (int i = 2; i <= j; i++)
	{
		if (n % i == 0) return false;
	}
	return true;
}

int partition(std::vector<int>& A, int p, int q)
{
	int x = A[p];
	int i = p;
	int j;
	for (j = p + 1; j<q; j++)
	{
		if (A[j] <= x)
		{
			i++;
			std::swap(A[i], A[j]);
		}

	}
	std::swap(A[i], A[p]);
	return i;
}

void quickSort(std::vector<int>& A, int p, int q)
{
	int r;
	if (p<q)
	{
		r = partition(A, p, q);
		if ((q - p)>250000)
		{
			thread Th1 = create_thread([&A, p, r] {quickSort(A, p, r); });

			thread Th2 = create_thread([&A, r, q] {quickSort(A, r + 1, q); });

			join(Th1);
			join(Th2);
		}
		else
		{
			quickSort(A, p, r);
			quickSort(A, r + 1, q);
		}
	}
}
int main()
{
	int const NThreads = 4; // this number of threads is only for the primes part and not the sort part
	int const NLastNum = 1000000;
	int const PerThread = NLastNum / NThreads;
	std::vector<thread> threads;
	std::vector<int> final_result;

	try
	{
		
		for (int iTh = 0; iTh < NThreads; iTh++)
		{
			threads.push_back(create_thread([iTh, PerThread,&final_result]
			{
				for (int i = iTh * PerThread; i < (iTh + 1) * PerThread; i += 1)
					if (is_prime(i))
					{
						MyMutex.lock();
						final_result.push_back(i);
						MyMutex.unlock();
					}
			}));
		}

		for (auto &&thread : threads)	join(thread);
		
		// from my Quick sort project which does the sorting with parallel threading

		int final_size = final_result.size();
		quickSort(final_result, 0, final_size);
	
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

	return 0;

}