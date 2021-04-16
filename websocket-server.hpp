#ifndef WEBSOCKET_SERVER_HEADER
#define WEBSOCKET_SERVER_HEADER

#include <bits/stdc++.h>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/buffers_iterator.hpp>
#include <cstdlib>
#include "debug.hpp"
#include "event-emitter.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

namespace websocket_server
{
	class Conn : public std::enable_shared_from_this<Conn>
	{
		private:
			websocket::stream<beast::tcp_stream> ws;
			beast::flat_buffer read_buf;

		public:
			events::EventEmitter<std::string&> message_event;
			events::EventEmitter<> close_event;

			explicit Conn(tcp::socket&& socket) : ws(std::move(socket)) {}

			void run()
			{
				net::dispatch(ws.get_executor(),
					beast::bind_front_handler(&Conn::on_run, shared_from_this()));
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
					&Conn::on_accept, shared_from_this()));
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

				ws.async_read(read_buf, beast::bind_front_handler(
					&Conn::on_read, shared_from_this()));
			}

			void on_read(beast::error_code err, size_t bytes_transferred)
			{
				boost::ignore_unused(bytes_transferred);

				if (err == websocket::error::closed)
				{
					close_event.trigger();
					return;
				}

				if (err)
				{
					fprintf(stderr, "error on read: %s\n",
						err.message().c_str());
				}

				std::string message(beast::buffers_to_string(read_buf.data()));
				message_event.trigger(message);

				read_buf.consume(read_buf.size());
			}

			void write(std::string message)
			{
				ws.text(true);

				beast::flat_buffer send_buf;
				size_t n = boost::asio::buffer_copy(send_buf.prepare(message.size()),
					boost::asio::buffer(message));
				send_buf.commit(n);

				ws.async_write(send_buf.data(), beast::bind_front_handler(
					&Conn::on_write, shared_from_this()));
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

				// read_buf.consume(read_buf.size());

				do_read();
			}
	};

	class Listener : public std::enable_shared_from_this<Listener>
	{
		private:
			events::EventEmitter<Conn&>& conn_event;
			net::io_context& io_ctx;
			tcp::acceptor acceptor;

		public:
			Listener(events::EventEmitter<Conn&>& conn_event,
				net::io_context& io_ctx, tcp::endpoint endpoint)
					: io_ctx(io_ctx), acceptor(io_ctx), conn_event(conn_event)
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
					std::shared_ptr<Conn> conn = std::make_shared<Conn>(std::move(socket));
					conn->run();
					conn_event.trigger(*conn);
				}

				do_accept();
			}
	};

	class WebsocketServer
	{
		private:
			size_t thread_count;

		public:
			events::EventEmitter<Conn&> conn_event;

			WebsocketServer(size_t thread_count) : thread_count(thread_count) {}

			void listen(uint16_t port)
			{
				net::ip::address addr = net::ip::make_address("0.0.0.0");

				net::io_context io_ctx(thread_count);
				std::make_shared<Listener>(conn_event, io_ctx,
					tcp::endpoint(addr, port))->run();

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
	};
};

#endif