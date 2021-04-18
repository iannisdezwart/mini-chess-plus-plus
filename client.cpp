#include <bits/stdc++.h>

#define DEBUG

#include "keypress.hpp"
#include "board.hpp"
#include "game.hpp"
#include "event-emitter.hpp"
#include "websocket-client.hpp"
#include "util.hpp"
#include "ws_messages.hpp"

void game(WebsocketClient& ws_client, std::string&& room_name,
	enum chess::Players player)
{
	chess::Game *game = new chess::Game(player, ws_client, std::move(room_name));

	ws_client.message_event.set_listener([&ws_client, game](std::string& message)
	{
		if (util::starts_with(message, "move "))
		{
			if (message.size() < 10)
			{
				debug("invalid length in server move message");
				goto read_next;
			}

			try
			{
				ws_messages::move::Move move = ws_messages::move::decode_message(message);

				game->move(move.from, move.to);
				goto read_next;
			}
			catch (...)
			{
				debug("invalid server move message");
				goto read_next;
			}
		}

		read_next:
		ws_client.do_read();
	});

	ws_client.do_read();
}

void create_room(WebsocketClient& ws_client)
{
	printf("Room name > ");
	std::string room_name;
	std::cin >> room_name;

	ws_client.message_event.set_listener(
		[&ws_client, room_name](std::string& message) mutable
	{
		if (message == ws_messages::general::ok_message)
		{
			game(ws_client, std::move(room_name), chess::Players::WHITE);
		}
		else
		{
			printf("error creating room: %s\n", message.c_str());
		}
	});

	ws_client.write("create-room " + room_name);
	ws_client.do_read();
}

void join_room(WebsocketClient& ws_client)
{
	printf("Room name > ");
	std::string room_name;
	std::cin >> room_name;

	ws_client.message_event.set_listener(
		[&ws_client, room_name](std::string& message) mutable
	{
		if (message == ws_messages::general::ok_message)
		{
			game(ws_client, std::move(room_name), chess::Players::BLACK);
		}
		else
		{
			printf("error joining room: %s\n", message.c_str());
		}
	});

	ws_client.write("join-room " + room_name);
	ws_client.do_read();
}

int main()
{
	WebsocketClient ws_client("localhost", 1337, [&ws_client]()
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