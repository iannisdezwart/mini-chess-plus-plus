#ifndef WEBSOCKET_CLIENT_HEADER
#define WEBSOCKET_CLIENT_HEADER

#include <bits/stdc++.h>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>

namespace ws
{
	namespace beast = boost::beast;
	namespace http = beast::http;
	namespace websocket = beast::websocket;
	namespace net = boost::asio;
	using tcp = boost::asio::ip::tcp;

	class ClientConn : public std::enable_shared_from_this<ClientConn>
	{
		private:
			tcp::resolver resolver;
			websocket::stream<tcp::socket> ws;
			beast::multi_buffer read_buf;
			std::string host;
			std::function<void()> conn_callback;
			std::function<void(std::string&)> read_callback;

		public:
			explicit ClientConn(std::function<void()>&& conn_callback,
				net::io_context& io_ctx)
					: resolver(io_ctx), ws(io_ctx),
						conn_callback(std::move(conn_callback)) {}

			void run(std::string host, std::string port)
			{
				this->host = host;

				resolver.async_resolve(host, port, std::bind(&ClientConn::on_resolve,
					shared_from_this(), std::placeholders::_1, std::placeholders::_2));
			}

			void on_resolve(boost::system::error_code err,
				tcp::resolver::results_type results)
			{
				if (err)
				{
					fprintf(stderr, "error on resolve: %s\n",
						err.message().c_str());
					return;
				}

				net::async_connect(ws.next_layer(), results.begin(), results.end(),
					std::bind(&ClientConn::on_connect, shared_from_this(), std::placeholders::_1));
			}

			void on_connect(boost::system::error_code err)
			{
				if (err)
				{
					fprintf(stderr, "error on connect: %s\n",
						err.message().c_str());
					return;
				}

				ws.async_handshake(host, "/", std::bind(conn_callback));
			}

			void read(const std::function<void(std::string&)>& callback)
			{
				read_callback = callback;

				// Read a message into our buffer

				ws.async_read(read_buf, std::bind(&ClientConn::on_read,
					shared_from_this(), std::placeholders::_1, std::placeholders::_2));
			}

			void on_read(boost::system::error_code err, size_t bytes_transferred)
			{
				boost::ignore_unused(bytes_transferred);

				if (err)
				{
					fprintf(stderr, "error on read: %s\n",
						err.message().c_str());
					return;
				}

				std::string message(beast::buffers_to_string(read_buf.data()));
				debug("< %s", message.c_str());
				read_callback(message);

				read_buf.consume(read_buf.size());
			}

			void write(const std::string& message)
			{
				debug("> %s", message.c_str());

				ws.text(true);

				beast::flat_buffer send_buf;
				size_t n = boost::asio::buffer_copy(send_buf.prepare(message.size()),
					boost::asio::buffer(message));
				send_buf.commit(n);

				ws.async_write(send_buf.data(), beast::bind_front_handler(
					&ClientConn::on_write, shared_from_this()));
			}

			void on_write(boost::system::error_code err, size_t bytes_transferred)
			{
				boost::ignore_unused(bytes_transferred);

				if (err)
				{
					fprintf(stderr, "error on write: %s\n",
						err.message().c_str());
					return;
				}
			}

			void close(websocket::close_code close_code = websocket::close_code::normal)
			{
				ws.close(close_code);
			}
	};

	class WebsocketClient
	{
		private:
			std::shared_ptr<ClientConn> conn;

		public:
			WebsocketClient(std::string host, uint16_t port,
				std::function<void()>&& conn_callback)
			{
				net::io_context io_ctx;
				conn = std::make_shared<ClientConn>(std::move(conn_callback), io_ctx);
				conn->run(host, std::to_string(port));
				io_ctx.run();
			}

			void write(const std::string& message)
			{
				conn->write(message);
			}

			void read(const std::function<void(std::string&)>& callback)
			{
				conn->read(callback);
			}

			void close(websocket::close_code close_code = websocket::close_code::normal)
			{
				conn->close(close_code);
			}
	};
};

#endif