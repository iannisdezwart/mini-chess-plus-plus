#ifndef SERVER_GAME_HEADER
#define SERVER_GAME_HEADER

#include <bits/stdc++.h>
#include "board.hpp"
#include "../WebSocket/websocket-server.hpp"
#include "../WebSocket/ws_messages.hpp"

const std::string empty_nickname = "";

namespace chess
{
	class ServerGame
	{
		private:
			chess::Board board;
			ws::ServerConn *white = NULL;
			ws::ServerConn *black = NULL;

			void on_white_disconnect()
			{
				if (!black_connected())
				{
					end();
					return;
				}

				white = NULL;
				black->write(ws_messages::opponent_disconnected
					::create_server_message());
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
				white->write(ws_messages::opponent_disconnected
					::create_server_message());
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

			const std::string& white_nickname()
			{
				if (white == NULL) return empty_nickname;
				return white->nickname;
			}

			const std::string& black_nickname()
			{
				if (white == NULL) return empty_nickname;
				return black->nickname;
			}

			void connect_white(ws::ServerConn *conn)
			{
				white = conn;

				if (black_connected())
				{
					black->write(ws_messages::opponent_connected
						::create_server_message(white->nickname));
				}

				conn->close_event.set_listener(std::bind(
					&ServerGame::on_white_disconnect, this));
			}

			void connect_black(ws::ServerConn *conn)
			{
				black = conn;

				if (white_connected())
				{
					white->write(ws_messages::opponent_connected
						::create_server_message(black->nickname));
				}

				conn->close_event.set_listener(std::bind(
					&ServerGame::on_black_disconnect, this));
			}

			void handle_move(ws::ServerConn& conn,
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
};

#endif