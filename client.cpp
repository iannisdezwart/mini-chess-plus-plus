#include <bits/stdc++.h>

#define DEBUG

#include "Chess/board.hpp"
#include "Chess/client-game.hpp"
#include "Util/event-emitter.hpp"
#include "WebSocket/websocket-client.hpp"
#include "Util/util.hpp"
#include "WebSocket/ws_messages.hpp"
#include "Util/debug.hpp"

#define HOST "86.91.151.176"
#define PORT 1337

std::string nickname;

void show_main_menu(ws::WebsocketClient& ws_client);

void fetch_board_state(chess::ClientGame *game,
	const std::function<void()>& callback)
{
	std::function<void()> saved_callback = callback;
	game->ws_client.write(ws_messages::fetch_board_state
		::create_client_message(game->room_name));

	game->ws_client.read([game, saved_callback](std::string& message)
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
			debug("invalid server fetch-board-state response");
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

	if (util::starts_with(message, "opponent-connected "))
	{
		try
		{
			game->set_opponent_nickname(ws_messages::opponent_connected
				::decode_server_message(message));

			game->print(false);
		}
		catch (std::string err)
		{
			debug("invalid server set-opponent-nickname message");
		}

		goto read_next;
	}

	if (util::starts_with(message, "opponent-disconnected"))
	{
		game->end();
		printf("Opponent disconnected, you win!\n");

		goto read_next;
	}

	read_next:

	if (game->finished())
	{
		// idk if this will work

		ws::WebsocketClient& ws_client = game->ws_client;
		delete game;
		show_main_menu(ws_client);
	}
	else
	{
		game->ws_client.read(std::bind(&on_message_receive,
			game, std::placeholders::_1));
	}
}

void start_game(ws::WebsocketClient& ws_client, std::string&& room_name,
	enum chess::Players player, std::string&& opponent_nickname)
{
	chess::ClientGame *game = new chess::ClientGame(
		player, ws_client, std::move(room_name), nickname);

	game->set_opponent_nickname(std::move(opponent_nickname));

	fetch_board_state(game, [game]()
	{
		game->start();
		game->print(true);
		game->ws_client.read(std::bind(&on_message_receive,
			game, std::placeholders::_1));
	});
}

void create_room(ws::WebsocketClient& ws_client)
{
	printf("Room name > ");
	std::string room_name;
	std::cin >> room_name;

	ws_client.write(ws_messages::create_room::create_client_message(room_name));

	ws_client.read([&ws_client, room_name](std::string& message) mutable
	{
		if (message == ws_messages::general::ok_message)
		{
			start_game(ws_client, std::move(room_name),
				chess::Players::WHITE, "opponent");
		}
		else
		{
			printf("error creating room: %s\n", message.c_str());
		}
	});
}

void join_room(ws::WebsocketClient& ws_client)
{
	printf("Room name > ");
	std::string room_name;
	std::cin >> room_name;

	ws_client.write(ws_messages::join_room::create_client_message(room_name));

	ws_client.read([&ws_client, room_name](std::string& message) mutable
	{
		if (util::starts_with(message, "opponent-nickname "))
		{
			try
			{
				start_game(ws_client, std::move(room_name), chess::Players::BLACK,
					ws_messages::join_room::decode_server_message(message));
			}
			catch (std::string err)
			{
				debug("invalid join-room server response");
			}
		}
		else
		{
			printf("error joining room: %s\n", message.c_str());
		}
	});
}

void show_main_menu(ws::WebsocketClient& ws_client)
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

void set_nickname(ws::WebsocketClient& ws_client,
	std::function<void()> callback)
{
	printf("Enter nickname > ");
	std::getline(std::cin, nickname);

	ws_client.write(ws_messages::set_nickname::create_client_message(nickname));

	ws_client.read([&ws_client, callback](std::string& message)
	{
		if (message != ws_messages::general::ok_message)
		{
			if (message == ws_messages::set_nickname::err_nickname_is_taken)
			{
				printf("That nickname is already taken, try another nickname...\n");
				set_nickname(ws_client, callback);
				return;
			}

			printf("error setting nickname: %s\n", message.c_str());
			return;
		}

		callback();
	});
}

int main()
{
	ws::WebsocketClient ws_client(HOST, PORT, [&ws_client]()
	{
		set_nickname(ws_client, [&ws_client]()
		{
			show_main_menu(ws_client);
		});
	});
}