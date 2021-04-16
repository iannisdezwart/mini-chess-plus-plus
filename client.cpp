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

int main()
{
	printf("1. Create room (play as white)\n");
	printf("2. Join room (play as black)\n");
	try_again:
	printf("> ");

	uint8_t choice;
	std::cin >> choice;

	switch (choice)
	{
		case 1:
			create_room(); // Todo: make
			break;

		case 2:
			join_room(); // Todo: make
			break;

		default:
			goto try_again;
	}
}