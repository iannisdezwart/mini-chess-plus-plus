#include <bits/stdc++.h>

#define DEBUG

#include "websocket-server.hpp"
#include "board.hpp"
#include "ws_messages.hpp"
#include "util.hpp"

#define PORT 1337
#define THREAD_COUNT 1 // std::thread::hardware_concurrency()

namespace ws_server = websocket_server;

class Game
{
	private:
		chess::Board board;

	public:
		ws_server::Conn *white = NULL;
		ws_server::Conn *black = NULL;

		void handle_move(ws_server::Conn& conn,
			chess::Square from, chess::Square to)
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

			board.move(from, to);
			std::string message = ws_messages::move::create_message(from, to);

			if (white != NULL) white->write(message);
			if (black != NULL) black->write(message);
		}
};

int main()
{
	ws_server::WebsocketServer server(THREAD_COUNT);
	std::unordered_map<std::string, Game> games;

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

				games[room_name] = Game();
				Game& game = games[room_name];
				game.white = &conn;
				conn.write(ws_messages::general::ok_message);
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

				Game& game = games[room_name];

				if (game.black != NULL)
				{
					conn.write(ws_messages::join_room::err_room_is_full);
					return;
				}

				game.black = &conn;
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

					Game& game = games[move.room_name];
					game.handle_move(conn, move.from, move.to);
					return;
				}
				catch (std::string err)
				{
					conn.write(err);
					return;
				}
			}

			conn.write(ws_messages::general::err_unknown_command);
		});
	});

	server.listen(PORT);
}