
#include "stdafx.h"
#include "cs477.h"
#include "future.h"
#include "shared_mem.h"
#include <thread>
#include <memory>

#define PROC_QUE 1000
#define PRIMES_LENGTH 1000000



bool is_prime(int n); 

int main() {

	std::shared_ptr<cs477::bounded_queue<unsigned int, PROC_QUE>> to_proc =
		std::shared_ptr<cs477::bounded_queue<unsigned int, PROC_QUE>>();
	to_proc->create("to_proc");
	std::shared_ptr<cs477::bounded_queue<std::array<unsigned int, PRIMES_LENGTH>, PROC_QUE>> to_driver =
		std::make_shared<cs477::bounded_queue<std::array<unsigned int, PRIMES_LENGTH> , PROC_QUE >> ();
	to_driver->create("to_driver");

	int NThreads = std::thread::hardware_concurrency();
	int PerThread = PRIMES_LENGTH;
	int thread_unit = PerThread / NThreads;

	for (;;) {
		//from driver to processor
		auto start = to_proc->read();

		std::vector<cs477::future<std::vector<int>>> futures;

		try {
			for (unsigned int iTh = start; iTh < NThreads + start; ++iTh) {
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

			//waiting for all the threads in the processor return ...
			cs477::future<std::vector<cs477::future<std::vector<int>>>> all_returned = cs477::when_all(futures.begin(), futures.end());
			shared_prime_reply<PRIMES_LENGTH> final_primes;
			//concat all the results from futures into the final array
			auto i = 0;
			for (auto&& future : all_returned.get())
				for (auto&& p : future.get())
				{
					final_primes.at(i) = p;
					i++;
				}
			to_driver->write(final_primes);
		}
		catch (std::system_error &ex) {
			printf("Error: %d (%s)\n", ex.code().value(), ex.what());
		}
		catch (std::exception &ex) {
			printf("Error: %s\n", ex.what());
		}
		catch (...) {
			printf("Error!\n");
		}
	}
	return 0;
}

bool is_prime(int n)
{
	for (int i = 2; i < n; i++) {
		if (n % i == 0) return false;
	}
	return true;
}