#include <bits/stdc++.h>

#define DEBUG

#include "websocket-server.hpp"
#include "board.hpp"
#include "ws_messages.hpp"
#include "util.hpp"

#define PORT 1337
#define THREAD_COUNT 1 // std::thread::hardware_concurrency()

namespace ws_server = websocket_server;

class ServerGame
{
	private:
		chess::Board board;
		ws_server::Conn *white = NULL;
		ws_server::Conn *black = NULL;

		void on_white_disconnect()
		{
			if (!black_connected())
			{
				end();
				return;
			}

			white = NULL;
			black->write(ws_messages::disconnect::opponent_disconnected);
			end();
		}

		void on_black_disconnect()
		{
			if (!white_connected())
			{
				end();
				return;
			}

			black = NULL;
			white->write(ws_messages::disconnect::opponent_disconnected);
			end();
		}

		void end()
		{
			end_event.trigger();
		}

	public:
		events::EventEmitter<> end_event;

		ServerGame()
		{
			board.initialise_standard();
		}

		bool white_connected()
		{
			return white != NULL;
		}

		bool black_connected()
		{
			return black != NULL;
		}

		void connect_white(ws_server::Conn *conn)
		{
			white = conn;

			if (black_connected())
			{
				black->write(ws_messages::connect::opponent_connected);
			}

			conn->close_event.set_listener(std::bind(
				&ServerGame::on_white_disconnect, this));
		}

		void connect_black(ws_server::Conn *conn)
		{
			black = conn;

			if (white_connected())
			{
				white->write(ws_messages::connect::opponent_connected);
			}

			conn->close_event.set_listener(std::bind(
				&ServerGame::on_black_disconnect, this));
		}

		void handle_move(ws_server::Conn& conn,
			chess::Square from, chess::Square to, char promotion)
		{
			// Authenticate

			if (&conn == white && board.turn == chess::Players::BLACK
				|| &conn == black && board.turn == chess::Players::WHITE)
			{
				conn.write(ws_messages::move::err_not_your_turn);
				return;
			}

			if (&conn != white && &conn != black)
			{
				conn.write(ws_messages::move::err_not_your_board);
				return;
			}

			if (!board.is_legal_move(board.turn, from.x, from.y, to.x, to.y))
			{
				conn.write(ws_messages::move::err_illegal_move);
				return;
			}

			board.move(from, to, promotion);

			std::string message = ws_messages::move::create_message(
				from, to, promotion);

			if (white != NULL) white->write(message);
			if (black != NULL) black->write(message);

			if (board.ended())
			{
				end();
			}
		}

		std::string to_str() const
		{
			return board.to_str();
		}
};

int main()
{
	ws_server::WebsocketServer server(THREAD_COUNT);
	std::unordered_map<std::string, ServerGame> games;

	server.conn_event.set_listener([&games](ws_server::Conn& conn)
	{
		conn.message_event.set_listener([&games, &conn](std::string message)
		{
			if (util::starts_with(message, "create-room "))
			{
				std::string room_name = message.substr(12);

				if (games.count(room_name))
				{
					conn.write(ws_messages::create_room::err_room_already_exists);
					return;
				}

				games[room_name] = ServerGame();
				debug("created game %s", room_name.c_str());

				ServerGame& game = games[room_name];
				game.connect_white(&conn);
				conn.write(ws_messages::general::ok_message);

				game.end_event.set_listener([&games, room_name]()
				{
					games.erase(room_name);
					debug("stopped game %s", room_name.c_str());
				});

				return;
			}

			if (util::starts_with(message, "join-room "))
			{
				std::string room_name = message.substr(10);

				if (!games.count(room_name))
				{
					conn.write(ws_messages::join_room::err_room_does_not_exist);
					return;
				}

				ServerGame& game = games[room_name];

				if (game.black_connected())
				{
					conn.write(ws_messages::join_room::err_room_is_full);
					return;
				}

				game.connect_black(&conn);
				conn.write(ws_messages::general::ok_message);
				return;
			}

			if (util::starts_with(message, "move "))
			{
				try
				{
					ws_messages::move::MoveAndRoomName move =
						ws_messages::move::decode_mesage_with_room_name(message);

					if (!games.count(move.room_name))
					{
						conn.write(ws_messages::move::err_room_does_not_exist);
						return;
					}

					ServerGame& game = games[move.room_name];
					game.handle_move(conn, move.from, move.to, move.promotion);
					return;
				}
				catch (std::string err)
				{
					conn.write(err);
					return;
				}
			}

			if (util::starts_with(message, "chat "))
			{
				// Todo: implement
			}

			if (util::starts_with(message, "fetch-board-state "))
			{
				try
				{
					std::string room_name =
						ws_messages::fetch_board_state::decode_client_message(message);

					if (!games.count(room_name))
					{
						conn.write(ws_messages::fetch_board_state::err_room_does_not_exist);
						return;
					}

					const ServerGame& game = games[room_name];
					std::string board_str = game.to_str();

					conn.write(ws_messages::fetch_board_state
						::create_server_message(board_str));
				}
				catch (std::string err)
				{
					conn.write(err);
				}

				return;
			}

			conn.write(ws_messages::general::err_unknown_command);
		});
	});

	server.listen(PORT);
}