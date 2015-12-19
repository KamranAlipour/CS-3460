// quicksort.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include "thread.h"
#include "../include/cs477.h"
#include "../include/future.h"

int N; // N: defines the minimum length of vector for which sort should be done parallel
int test_vec_length = 1000000; // the length of test vector used to compare the runtime of parallel and serial swapping to define N

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
		if ((q - p)>N)
		{
			// Assign half of the work to the thread pool
			cs477::future<void> f = cs477::queue_work([&A, p, r]() {quickSort(A, p, r); });
			// Do the other half on the same thread
			quickSort(A, r + 1, q);
			f.get();
		}
		else
		{
			quickSort(A, p, r);
			quickSort(A, r + 1, q);
		}
	}
}

// This function is used to measure time of sorting with different methods.
template <class Function> __int64 time_call(Function&& f)
{
	__int64 begin = GetTickCount();
	f();
	return GetTickCount() - begin;
}

inline void serial_swap()
{	// measuring runtime for swapping two elements of vector parallel wise
	
	std::vector<int> test(test_vec_length); // test vector
	for (auto i = 1; i < test_vec_length;i++)
	std::swap(test[i-1], test[i]);
}

inline void parallel_swap()
{
	// measuring runtime for swapping two elements of vector parallel wise
	
	std::vector<int> test(test_vec_length); // test vector
	cs477::queue_work([&]() {
		for (auto i = 1; i < test_vec_length; i++)
		std::swap(test[i-1], test[i]);
	}).get();
}

inline int Test_Define_N()
{
	// First we need to find the limit of N (Defined above)
	// to do this we first try to sort a 100 both parallel and serial
	// then compare the runtime and find out for what length of vector it's worth
	// to do parallel instead of serial

	auto ser_time = 0; // test serial runtime
	auto par_time = 0; // test parallel runtime
	for (auto i = 1; i < 20; i++) // we do the test 20 times to make sure we get somthing valid!
	{
		auto ts = time_call(serial_swap);
		if (ser_time < ts) ser_time = ts;
		auto tp = time_call(parallel_swap);
		if (par_time < tp) par_time = tp;
	}

	return par_time / ser_time * test_vec_length / 2; // it is divided by 2 because when parallel, 
												      // on each sort level half of the work goes to thread pool and half stays on the thread
}

int main()
{
	N = Test_Define_N(); // First we need to find the limit of N (Defined above)
		
	int vec_length = 10000000; // length of random numbers vector to be sorted
	std::vector<int> A(vec_length);

	// Filling the vector of random numbers
	std::generate(A.begin(), A.end(), std::rand);
	
	// Print out the initial vector
	//std::cout << "======Original=======" << std::endl;
	//for (auto e : A)	std::cout << e << " ";
	//std::cout << std::endl;
	

	// Quick sort algorithm
	quickSort(A, 0, vec_length);

	// Print out the sorted vector
	//std::cout << "======Sorted=======" << std::endl;
	//for (auto e : A)	std::cout << e << std::endl;
	//std::cout << std::endl;


	return 0;
}


