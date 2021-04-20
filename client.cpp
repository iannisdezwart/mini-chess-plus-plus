#include <bits/stdc++.h>

#define DEBUG

#include "Chess/board.hpp"
#include "Chess/game.hpp"
#include "Util/event-emitter.hpp"
#include "WebSocket/websocket-client.hpp"
#include "Util/util.hpp"
#include "WebSocket/ws_messages.hpp"

#define HOST "86.91.151.176"
#define PORT 1337

void show_main_menu(WebsocketClient& ws_client);

void fetch_board_state(chess::ClientGame *game, const std::function<void()>& callback)
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

void on_message_receive(chess::ClientGame *game, std::string& message)
{
	if (util::starts_with(message, "move "))
	{
		try
		{
			ws_messages::move::Move move =
				ws_messages::move::decode_message(message);

			game->move(move.from, move.to, move.promotion);
		}
		catch (...)
		{
			debug("invalid server move message");
		}

		goto read_next;
	}

	if (util::starts_with(message, "chat "))
	{
		// Todo: implement

		goto read_next;
	}

	if (message == ws_messages::connect::opponent_connected)
	{
		printf("Opponent connected!\n");

		goto read_next;
	}

	if (message == ws_messages::disconnect::opponent_disconnected)
	{
		game->end();
		printf("Opponent disconnected, you win!\n");

		goto read_next;
	}

	read_next:

	if (game->finished())
	{
		// idk if this will work

		WebsocketClient& ws_client = game->ws_client;
		delete game;
		show_main_menu(ws_client);
	}
	else
	{
		game->ws_client.read(std::bind(&on_message_receive,
			game, std::placeholders::_1));
	}
}

void start_game(WebsocketClient& ws_client, std::string&& room_name,
	enum chess::Players player)
{
	chess::ClientGame *game = new chess::ClientGame(player, ws_client, std::move(room_name));

	fetch_board_state(game, [game]()
	{
		game->start();
		game->print(true);
		game->ws_client.read(std::bind(&on_message_receive,
			game, std::placeholders::_1));
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

void show_main_menu(WebsocketClient& ws_client)
{
	printf("1. Create room (play as white)\n");
	printf("2. Join room (play as black)\n");
	printf("3. Exit MiniChess++\n");
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

		case '3':
			exit(0);

		default:
			goto try_again;
	}
}

int main()
{
	WebsocketClient ws_client(HOST, PORT, [&ws_client]()
	{
		show_main_menu(ws_client);
	});
}