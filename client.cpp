#include <bits/stdc++.h>

#define DEBUG

#include "keypress.hpp"
#include "board.hpp"
#include "game.hpp"
#include "event-emitter.hpp"
#include "websocket-client.hpp"
#include "util.hpp"
#include "ws_messages.hpp"

#define HOST "86.91.151.176"
#define PORT 1337

void fetch_board_state(chess::Game *game, const std::function<void()>& callback)
{
	std::function<void()> saved_callback = callback;
	game->ws_client.write(ws_messages::fetch_board_state::create_client_message(game->room_name));

	game->ws_client.read([game, saved_callback](std::string message)
	{
		game->print(true);

		if (ws_messages::general::is_err(message))
		{
			printf("error fetching board state: %s\n", message.c_str());
			return;
		}

		try
		{
			game->load_from_str(ws_messages::fetch_board_state
				::decode_server_message(message));

			saved_callback();
		}
		catch (std::string err)
		{
			printf("error fetching board state: %s\n", err.c_str());
		}
	});
}

void start_game(WebsocketClient& ws_client, std::string&& room_name,
	enum chess::Players player)
{
	chess::Game *game = new chess::Game(player, ws_client, std::move(room_name));

	fetch_board_state(game, [game]()
	{
		game->start();

		while (true)
		{
			game->ws_client.read([game](std::string& message)
			{
				if (util::starts_with(message, "move "))
				{
					try
					{
						ws_messages::move::Move move =
							ws_messages::move::decode_message(message);

						game->move(move.from, move.to);
					}
					catch (...)
					{
						debug("invalid server move message");
					}

					return;
				}

				if (util::starts_with(message, "chat "))
				{
					// Todo: implement
				}
			});
		}
	});
}

void create_room(WebsocketClient& ws_client)
{
	printf("Room name > ");
	std::string room_name;
	std::cin >> room_name;

	ws_client.write("create-room " + room_name);

	ws_client.read([&ws_client, room_name](std::string& message) mutable
	{
		if (message == ws_messages::general::ok_message)
		{
			start_game(ws_client, std::move(room_name), chess::Players::WHITE);
		}
		else
		{
			printf("error creating room: %s\n", message.c_str());
		}
	});
}

void join_room(WebsocketClient& ws_client)
{
	printf("Room name > ");
	std::string room_name;
	std::cin >> room_name;

	ws_client.write("join-room " + room_name);

	ws_client.read([&ws_client, room_name](std::string& message) mutable
	{
		if (message == ws_messages::general::ok_message)
		{
			start_game(ws_client, std::move(room_name), chess::Players::BLACK);
		}
		else
		{
			printf("error joining room: %s\n", message.c_str());
		}
	});
}

int main()
{
	WebsocketClient ws_client(HOST, PORT, [&ws_client]()
	{
		printf("1. Create room (play as white)\n");
		printf("2. Join room (play as black)\n");
		try_again:
		printf("> ");

		char choice;
		std::cin >> choice;

		switch (choice)
		{
			case '1':
				create_room(ws_client);
				break;

			case '2':
				join_room(ws_client);
				break;

			default:
				goto try_again;
		}
	});
}