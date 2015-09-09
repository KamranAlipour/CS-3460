// primes.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include <iostream>
#include <vector>

#include "thread.h"

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
	int const NThreads = 4;
	int const NLastNum = 10000000;
	int const PerThread = NLastNum / NThreads;
	std::vector<thread> threads;
	/*
	for (int i = 2; i < NLastNum; i++)
	{
		if (is_prime(i))
		{
			std::cout << i << std::endl;
		}
	}
	*/

	try
	{
		for (int iTh = 0; iTh < NThreads; iTh++)
		{
			threads.push_back(create_thread([iTh, PerThread]
			{
				for (int i = iTh * PerThread; i < (iTh+1) * PerThread; i += 1)	if (is_prime(i))	std::cout << i << std::endl;
			}));
			join(threads[iTh]);
		}

		//for (auto &&thread : threads)	join(thread);
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