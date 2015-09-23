// quicksort.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include "thread.h"
#include <time.h>

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
		if ((q-p)>500000)
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
	int vec_length = 10000000; // length of random numbers vector
	std::vector<int> A(vec_length);

	// Initialize the random seed
	//srand(time(NULL));

	// Filling the vector of random numbers
	std::generate(A.begin(), A.end(), std::rand);
	// Print out the initial vector
	/*
	std::cout << "======Original=======" << std::endl;
	for (auto e : A)	std::cout << e << " ";
	std::cout << std::endl;
	*/

	// Quick sort algorithm
	quickSort(A, 0, vec_length);

	// Print out the sorted vector

	//std::cout << "======Sorted=======" << std::endl;
	//for (auto e : A)	std::cout << e << std::endl;
	//std::cout << std::endl;


	return 0;
}