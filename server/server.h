
#pragma once

#include <thread>

#include "../include/cs477.h"
#include "../include/queue.h"
#include "../include/data.h"
#include "../include/net.h"
#include "../include/http.h"


cs477::net::http_response make_response(int status, const std::string &json);

namespace single_thread
{

	void run(const sockaddr_in &addr, std::shared_ptr<cs477::data::database> db);
}

namespace multi_thread
{
	void run(std::shared_ptr<cs477::data::database> db);
	void run2(std::shared_ptr<cs477::data::database> db);
}

namespace async
{
	void run(const sockaddr_in &addr);
}

