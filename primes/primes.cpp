// primes.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include "thread.h"
#include <time.h>
#include <mutex>
#include "../include/cs477.h"

bool is_prime(int n)
{
	auto j = static_cast<int>(sqrt(n));
	for (int i = 2; i <= j; i++)
	{
		if (n % i == 0) return false;
	}
	return true;
}

int main()
{
	int const NThreads = std::thread::hardware_concurrency();
	int const NLastNum = 100000000;
	int const PerThread = NLastNum / NThreads;

	std::vector<int> final_result;

	std::vector<cs477::future<std::vector<int>>> futures;

	try
	{

		for (unsigned int iTh = 0; iTh < NThreads; ++iTh) {
			auto future = cs477::queue_work([iTh, &PerThread] {
				std::vector<int> vec;

				for (int k = iTh * PerThread; k < (iTh + 1) * PerThread; ++k) {
					if (is_prime(k)) {
						vec.push_back(k);
					}
				}
				return vec;
			});
			futures.push_back(std::move(future));
		}


		cs477::future<std::vector<cs477::future<std::vector<int>>>> all_returned = cs477::when_all(futures.begin(), futures.end());
		for (auto&& future : all_returned.get())
			for (auto&& p : future.get()) 
				final_result.push_back(p);
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