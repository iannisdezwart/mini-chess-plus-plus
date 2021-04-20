#include <bits/stdc++.h>

#define DEBUG

#include "WebSocket/websocket-server.hpp"
#include "Chess/board.hpp"
#include "Chess/server-game.hpp"
#include "WebSocket/ws_messages.hpp"
#include "Util/util.hpp"
#include "Util/debug.hpp"

#define PORT 1337
#define THREAD_COUNT 1 // std::thread::hardware_concurrency()

int main()
{
	ws::WebsocketServer server(THREAD_COUNT);
	std::unordered_map<std::string, chess::ServerGame> games;

	server.conn_event.set_listener([&games](ws::ServerConn& conn)
	{
		conn.message_event.set_listener([&games, &conn](std::string message)
		{
			if (util::starts_with(message, "set-nickname "))
			{
				try
				{
					conn.nickname = ws_messages::set_nickname
						::decode_client_message(message);

					conn.write(ws_messages::general::ok_message);
				}
				catch (std::string err)
				{
					conn.write(err);
				}

				return;
			}

			if (util::starts_with(message, "create-room "))
			{
				try
				{
					std::string room_name = ws_messages::create_room
						::decode_client_message(message);

					if (games.count(room_name))
					{
						conn.write(ws_messages::create_room::err_room_already_exists);
						return;
					}

					games[room_name] = chess::ServerGame();
					debug("created game %s", room_name.c_str());

					chess::ServerGame& game = games[room_name];
					game.connect_white(&conn);
					conn.write(ws_messages::general::ok_message);

					game.end_event.set_listener([&games, room_name]()
					{
						debug("stopped game %s", room_name.c_str());
						games.erase(room_name);
					});
				}
				catch (std::string err)
				{
					conn.write(err);
				}

				return;
			}

			if (util::starts_with(message, "join-room "))
			{
				try
				{
					std::string room_name = ws_messages::join_room
						::decode_client_message(message);

					if (!games.count(room_name))
					{
						conn.write(ws_messages::join_room::err_room_does_not_exist);
						return;
					}

					chess::ServerGame& game = games[room_name];

					if (game.black_connected())
					{
						conn.write(ws_messages::join_room::err_room_is_full);
						return;
					}

					game.connect_black(&conn);
					conn.write(ws_messages::join_room
						::create_server_message(game.white_nickname()));
				}
				catch (std::string err)
				{
					conn.write(err);
				}

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

					chess::ServerGame& game = games[move.room_name];
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

					const chess::ServerGame& game = games[room_name];
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