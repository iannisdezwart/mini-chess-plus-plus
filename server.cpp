#include <bits/stdc++.h>

#define DEBUG
#include "websocket-server.hpp"
#include "board.hpp"

#define PORT 1337
#define THREAD_COUNT 1 // std::thread::hardware_concurrency()

namespace ws_server = websocket_server;

bool starts_with(std::string str, std::string search)
{
	if (search.size() > str.size()) return false;

	for (size_t i = 0; i < search.size(); i++)
	{
		if (str[i] != search[i]) return false;
	}

	return true;
}

class Game
{
	private:
		chess::Board board;

	public:
		ws_server::Conn *white = NULL;
		ws_server::Conn *black = NULL;

		void handle_move(ws_server::Conn& conn, uint8_t col_from,
			uint8_t row_from, uint8_t col_to, uint8_t row_to)
		{
			// Authenticate

			if (&conn == white && board.turn == chess::Players::BLACK
				|| &conn == black && board.turn == chess::Players::WHITE)
			{
				conn.write("err: not your turn");
				return;
			}

			if (&conn != white && &conn != black)
			{
				conn.write("err: not your board");
				return;
			}

			if (!board.is_legal_move(board.turn, col_from, row_from, col_to, row_to))
			{
				conn.write("err: illegal move");
				return;
			}

			board.move(chess::Square(col_from, row_from),
				chess::Square(col_to, row_to));
			conn.write("ok");
		}
};

int main()
{
	ws_server::WebsocketServer server(THREAD_COUNT);
	std::unordered_map<std::string, Game> games;

	server.conn_event.add_listener([&games](ws_server::Conn& conn)
	{
		printf("new conn\n");

		conn.message_event.add_listener([&games, &conn](std::string message)
		{
			printf("new message: %s\n", message.c_str());

			if (starts_with(message, "create-room "))
			{
				std::string room_name = message.substr(12);

				if (games.count(room_name))
				{
					conn.write("err: room already exists");
					return;
				}

				games[room_name] = Game();
				Game& game = games[room_name];
				game.white = &conn;
				conn.write("ok");
				return;
			}

			if (starts_with(message, "join-room "))
			{
				std::string room_name = message.substr(10);

				if (!games.count(room_name))
				{
					conn.write("err: oom does not exist");
					return;
				}

				Game& game = games[room_name];

				if (game.black != NULL)
				{
					conn.write("err: room is full");
					return;
				}

				game.black = &conn;
				conn.write("ok");
				return;
			}

			if (starts_with(message, "move "))
			{
				if (message.size() < 10)
				{
					conn.write("err: invalid move arguments");
					return;
				}

				// Expects "A1 B2"

				uint8_t col_from = message[5] - 'A';
				uint8_t row_from = message[6] - '1';

				char space = message[7];

				uint8_t col_to = message[8] - 'A';
				uint8_t row_to = message[9] - '1';

				if (col_from >= 8 || row_from >= 8 || col_to >= 8
					|| row_to >= 8 || space != ' ')
				{
					conn.write("err: invalid input");
					return;
				}

				std::string room_name = message.substr(11);

				if (!games.count(room_name))
				{
					conn.write("err: room does not exist");
					return;
				}

				Game& game = games[room_name];
				game.handle_move(conn, col_from, row_from, col_to, row_to);
				return;
			}

			conn.write("err: unknown command");
		});
	});

	server.listen(PORT);
}