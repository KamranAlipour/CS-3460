
#include "stdafx.h"

#include "server.h"
#include "file.h"

namespace async
{
	void socket_handler(cs477::net::socket sock)
	{

		// Read an http request
		auto f = cs477::net::read_http_request_async(sock).then([sock](auto f)
		{
			auto rq = f.get();
			int status = 200;

			// Dispatch request 
			
			if (rq.method == "GET")
			{
				// The address of the static file is in http_request URL and the file should be read from that address
				const char * f_address = rq.url.c_str() + 1; 
				auto file_future = cs477::read_file_async(f_address).then([sock](auto file_future) 
				{
					auto file = file_future.get();
					cs477::net::write_http_response_async(sock, make_response(200, file));
				});
			}
			else
			{
				status = 404;
			}

			return 0;
		});
	}


	void run(const sockaddr_in &addr)
	{
		auto host = std::make_shared<cs477::net::acceptor>();
		host->listen(addr);

		for (int i = 0; i < 32; i++)
		{
			host->accept_async(socket_handler);
		}

		// Just wait forever.
		cs477::promise<void> p;
		auto f = p.get_future();
		f.wait();
	}
}
