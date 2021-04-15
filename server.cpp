#include <bits/stdc++.h>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/buffers_iterator.hpp>
// #include <boost/asio/connect.hpp>
// #include <boost/asio/ip/tcp.hpp>
#include <cstdlib>

#define PORT 1337

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

const int thread_count = std::thread::hardware_concurrency();

class Session : public std::enable_shared_from_this<Session>
{
	websocket::stream<beast::tcp_stream> ws;
	beast::flat_buffer buffer;

	public:
		explicit Session(tcp::socket&& socket) : ws(std::move(socket)) {}

		void run()
		{
			net::dispatch(ws.get_executor(),
				beast::bind_front_handler(&Session::on_run, shared_from_this()));
		}

		void on_run()
		{
			// Set default timeout settings

			ws.set_option(websocket::stream_base::timeout::suggested(
				beast::role_type::server));

			// Set custom server field

			ws.set_option(websocket::stream_base::decorator(
				[](websocket::response_type& res)
				{
					res.set(http::field::server, "MiniChess++ Server");
				}));

			// Accept the websocket handshake

			ws.async_accept(beast::bind_front_handler(
				&Session::on_accept, shared_from_this()));
		}

		void on_accept(beast::error_code err)
		{
			if (err)
			{
				fprintf(stderr, "error on accept: %s\n",
					err.message().c_str());
				return;
			}

			do_read();
		}

		void do_read()
		{
			// Read message into buffer

			ws.async_read(buffer, beast::bind_front_handler(
				&Session::on_read, shared_from_this()));
		}

		void on_read(beast::error_code err, size_t bytes_transferred)
		{
			boost::ignore_unused(bytes_transferred);

			if (err == websocket::error::closed) return;

			if (err)
			{
				fprintf(stderr, "error on read: %s\n",
					err.message().c_str());
			}

			ws.text(true);

			beast::flat_buffer send_buf;
			std::string data = "Hello, World!";

			size_t n = boost::asio::buffer_copy(send_buf.prepare(data.size()),
				boost::asio::buffer(data));
			send_buf.commit(n);

			ws.async_write(send_buf.data(), beast::bind_front_handler(
				&Session::on_write, shared_from_this()));
		}

		void on_write(beast::error_code err, size_t bytes_transferred)
		{
			boost::ignore_unused(bytes_transferred);

			if (err)
			{
				fprintf(stderr, "error on write: %s\n",
					err.message().c_str());
				return;
			}

			buffer.consume(buffer.size());

			do_read();
		}
};

class Listener : public std::enable_shared_from_this<Listener>
{
	private:
		net::io_context& io_ctx;
		tcp::acceptor acceptor;

	public:
		Listener(net::io_context& io_ctx, tcp::endpoint endpoint)
			: io_ctx(io_ctx), acceptor(io_ctx)
		{
			beast::error_code err;

			// Open the acceptor

			acceptor.open(endpoint.protocol(), err);

			if (err)
			{
				fprintf(stderr, "error opening endpoint: %s\n",
					err.message().c_str());
				return;
			}

			// Allow address reuse

			acceptor.set_option(net::socket_base::reuse_address(true), err);

			if (err)
			{
				fprintf(stderr, "error setting allow address reuse: %s\n",
					err.message().c_str());
				return;
			}

			// Bind to server address

			acceptor.bind(endpoint, err);

			if (err)
			{
				fprintf(stderr, "error binding to server address: %s\n",
					err.message().c_str());
				return;
			}

			// Start listening for connections

			acceptor.listen(net::socket_base::max_listen_connections, err);

			if (err)
			{
				fprintf(stderr, "error listening: %s\n",
					err.message().c_str());
				return;
			}
		}

		void run()
		{
			do_accept();
		}

	private:
		void do_accept()
		{
			acceptor.async_accept(net::make_strand(io_ctx),
				beast::bind_front_handler(&Listener::on_accept, shared_from_this()));
		}

		void on_accept(beast::error_code err, tcp::socket socket)
		{
			if (err)
			{
				fprintf(stderr, "error on accept: %s\n",
					err.message().c_str());
			}
			else
			{
				std::make_shared<Session>(std::move(socket))->run();
			}

			do_accept();
		}
};

int main()
{
	net::ip::address addr = net::ip::make_address("0.0.0.0");
	uint16_t port = PORT;

	net::io_context io_ctx(thread_count);
	std::make_shared<Listener>(io_ctx, tcp::endpoint(addr, port))->run();

	std::vector<std::thread> threads;

	for (size_t i = 0; i < thread_count - 1; i++)
	{
		threads.emplace_back([&io_ctx]()
		{
			io_ctx.run();
		});
	}

	io_ctx.run();
}