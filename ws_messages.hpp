#ifndef WS_MESSAGES_HEADER
#define WS_MESSAGES_HEADER

#include <bits/stdc++.h>
#include "board.hpp"
#include "util.hpp"

namespace ws_messages
{
	namespace move
	{
		std::string err_invalid_arguments = "err: invalid arguments";
		std::string err_room_does_not_exist = "err: room does not exist";
		std::string err_not_your_turn = "err: not your turn";
		std::string err_not_your_board = "err: not your board";
		std::string err_illegal_move = "err: illegal move";

		std::string create_message(chess::Square from, chess::Square to)
		{
			std::string message = "move XX XX";

			message[5] = 'A' + from.x;
			message[6] = '1' + from.y;
			message[8] = 'A' + to.x;
			message[9] = '1' + to.y;

			return message;
		}

		std::string create_message_with_room_name(chess::Square from,
			chess::Square to, const std::string& room_name)
		{
			std::string message = create_message(from, to);
			message += " " + room_name;
			return message;
		}

		struct Move
		{
			chess::Square from;
			chess::Square to;

			Move(uint8_t x_from, uint8_t y_from, uint8_t x_to, uint8_t y_to)
				: from(x_from, y_from), to(x_to, y_to) {}
		};

		Move decode_message(const std::string& message)
		{
			if (message.size() < 10)
			{
				debug("message size = %ld", message.size());
				throw err_invalid_arguments;
			}

			uint8_t col_from = message[5] - 'A';
			uint8_t row_from = message[6] - '1';

			char space = message[7];

			uint8_t col_to = message[8] - 'A';
			uint8_t row_to = message[9] - '1';

			if (col_from >= 8 || row_from >= 8 || col_to >= 8
				|| row_to >= 8 || space != ' ')
			{
				throw err_invalid_arguments;
			}

			return Move(col_from, row_from, col_to, row_to);
		}

		struct MoveAndRoomName
		{
			chess::Square from;
			chess::Square to;
			std::string room_name;

			MoveAndRoomName(uint8_t x_from, uint8_t y_from, uint8_t x_to, uint8_t y_to,
				std::string&& room_name)
				: from(x_from, y_from), to(x_to, y_to),
					room_name(std::move(room_name)) {}
		};

		MoveAndRoomName decode_mesage_with_room_name(const std::string& message)
		{
			if (message.size() < 12)
			{
				debug("message size = %ld", message.size());
				throw err_invalid_arguments;
			}

			uint8_t col_from = message[5] - 'A';
			uint8_t row_from = message[6] - '1';

			char space = message[7];

			uint8_t col_to = message[8] - 'A';
			uint8_t row_to = message[9] - '1';

			if (col_from >= 8 || row_from >= 8 || col_to >= 8
				|| row_to >= 8 || space != ' ')
			{
				throw err_invalid_arguments;
			}

			std::string room_name = message.substr(11);

			return MoveAndRoomName(col_from, row_from, col_to, row_to,
				std::move(room_name));
		}
	};

	namespace create_room
	{
		std::string err_room_already_exists = "err: room already exists";
	};

	namespace join_room
	{
		std::string err_room_does_not_exist = "err: room does not exist";
		std::string err_room_is_full = "err: room is full";
	};

	namespace fetch_board_state
	{
		std::string err_room_does_not_exist = "err: room does not exist";
		std::string err_invalid_arguments = "err: invalid arguments";

		std::string create_client_message(const std::string& room_name)
		{
			std::string message = "fetch-board-state " + room_name;
			return message;
		}

		std::string create_server_message(const std::string& board)
		{
			std::string message = "ok " + board;
			return message;
		}

		std::string decode_client_message(const std::string& message)
		{
			if (message.size() < 19)
			{
				throw err_invalid_arguments;
			}

			return message.substr(18);
		}

		std::string decode_server_message(const std::string& message)
		{
			if (message.size() != 67)
			{
				debug("size of message = %lu, message = %s", message.size(), message.c_str());
				throw err_invalid_arguments;
			}

			return message.substr(3);
		}
	};

	namespace general
	{
		std::string err_unknown_command = "err: unknown command";
		std::string ok_message = "ok";

		bool is_err(std::string message)
		{
			return util::starts_with(message, "err:");
		}
	};
};

#endif