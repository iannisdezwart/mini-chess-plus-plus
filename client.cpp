#include <bits/stdc++.h>

#define DEBUG

#include "keypress.hpp"
#include "board.hpp"
#include "event-emitter.hpp"
#include "websocket-client.hpp"

events::EventEmitter<uint8_t> key_pressed;

void keypress_thread()
{
	while (true)
	{
		uint8_t key = keypress::get_key();
		key_pressed.trigger(key);
	}
}

void create_room(WebsocketClient& ws_client)
{
	printf("Room name > ");
	std::string room_name;
	std::cin >> room_name;

	ws_client.write("create-room " + room_name);

	ws_client.message_event.add_listener([&ws_client](std::string& message) {
		if (message == "ok")
		{
			printf("successfully joined room\n");
		}
		else
		{
			printf("error joining room\n");
		}

		ws_client.close();
	});

	// ws_client.do_read();
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
				// join_room();
				break;

			default:
				goto try_again;
		}
	});
}