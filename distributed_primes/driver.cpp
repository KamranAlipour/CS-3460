// driver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cs477.h"
#include "future.h"
#include "shared_mem.h"
#include <thread>
#include <memory>
#include "dist_params.h"

#define PROC_QUE 1000
#define PRIMES_LENGTH 1000000

int main_() {

	// shared queues

	std::shared_ptr<cs477::bounded_queue<unsigned int, PROC_QUE>> to_proc =
		std::shared_ptr<cs477::bounded_queue<unsigned int, PROC_QUE>>();
	to_proc->create("to_proc");
	std::shared_ptr<cs477::bounded_queue<std::array<unsigned int, PRIMES_LENGTH>, PROC_QUE>> to_driver =
		std::make_shared<cs477::bounded_queue<std::array<unsigned int, PRIMES_LENGTH> , PROC_QUE >> ();
	to_driver->create("to_driver");

	for (int i = 0; i < PROC_QUE; ++i) to_proc->write(i);
	
	for (;;) {
		// Print out the list of primes in each queue from processor
		for (auto&& entry : to_driver->read()) std::cout << entry << " ";
		std::cout	<<	std::endl;
	}
	return 0;
}

