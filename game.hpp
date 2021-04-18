#ifndef GAME_HEADER
#define GAME_HEADER

#include <bits/stdc++.h>
#include "board.hpp"
#include "keypress.hpp"
#include "websocket-client.hpp"

namespace chess
{
	class Game
	{
		private:
			bool started;
			enum Players player;
			Board board;
			Square cursor;
			Square sel;
			Square prev_move_from;
			Square prev_move_to;
			std::vector<Square> moves;
			std::vector<Square> warn;
			bool upsd;
			std::thread keypress_thread;

		public:
			WebsocketClient& ws_client;
			std::string room_name;

			Game(enum Players player, WebsocketClient& ws_client,
				std::string&& room_name)
					: started(false), player(player), ws_client(ws_client),
						room_name(std::move(room_name)), cursor(3, 3), sel(-1, -1),
						prev_move_from({ -1, -1 }), prev_move_to({ -1, -1 }),
						upsd(player == Players::BLACK),
						keypress_thread(std::bind(&Game::keypress_handler, this))
			{
				board.initialise_blank();
				print(true);
			}

			void start()
			{
				started = true;
			}

			void print(bool new_move)
			{
				printf("\e[2J\e[H"); // Clears screen

				if (new_move)
				{
					warn.clear();

					if (board.white_in_check())
					{
						warn.push_back(board.white_king);
					}

					if (board.black_in_check())
					{
						warn.push_back(board.black_king);
					}
				}

				if (upsd)
				{
					board.print_upsd(cursor, sel, prev_move_from, prev_move_to, moves, warn);
				}
				else
				{
					board.print(cursor, sel, prev_move_from, prev_move_to, moves, warn);
				}

				if (new_move)
				{
					if (board.turn == Players::WHITE && !board.white_can_move() && started)
					{
						if (board.white_in_check())
						{
							printf("Black won because white is in checkmate\n");
							end();
						}
						else
						{
							printf("Tie because white is in stalemate\n");
							end();
						}
					}

					if (board.turn == Players::BLACK && !board.black_can_move() && started)
					{
						if (board.black_in_check())
						{
							printf("White won because black is in checkmate\n");
							end();
						}
						else
						{
							printf("Tie because black is in stalemate\n");
							end();
						}
					}
				}
			}

			void end()
			{
				keypress_thread.detach();
			}

			void keypress_handler()
			{
				while (true)
				{
					uint8_t key = keypress::get_key();

					switch (key)
					{
						case keypress::ARROW_UP:
							if (upsd)
							{
								cursor.y--;
								if (cursor.y < 0) cursor.y += 8;
							}
							else
							{
								cursor.y++;
								if (cursor.y > 7) cursor.y -= 8;
							}

							break;

						case keypress::ARROW_DOWN:
							if (upsd)
							{
								cursor.y++;
								if (cursor.y > 7) cursor.y -= 8;
							}
							else
							{
								cursor.y--;
								if (cursor.y < 0) cursor.y += 8;
							}

							break;

						case keypress::ARROW_RIGHT:
							if (upsd)
							{
								cursor.x--;
								if (cursor.x < 0) cursor.x += 8;
							}
							else
							{
								cursor.x++;
								if (cursor.x > 7) cursor.x -= 8;
							}

							break;

						case keypress::ARROW_LEFT:
							if (upsd)
							{
								cursor.x++;
								if (cursor.x > 7) cursor.x -= 8;
							}
							else
							{
								cursor.x--;
								if (cursor.x < 0) cursor.x += 8;
							}

							break;

						case keypress::SPACE:
							for (size_t i = 0; i < moves.size(); i++)
							{
								if (cursor.x == moves[i].x && cursor.y == moves[i].y)
								{
									// Move the piece to the selected square

									std::string message = "move XX XX " + room_name;

									message[5] = 'A' + sel.x;
									message[6] = '1' + sel.y;
									message[8] = 'A' + cursor.x;
									message[9] = '1' + cursor.y;

									ws_client.write(message);

									moves.clear();
									sel = Square(-1, -1);
									goto print_label;
								}
							}

							// Select this square

							sel = Square(cursor.x, cursor.y);

							if (player == Players::WHITE
								&& board.turn == Players::WHITE
								&& is_white(board.squares[sel.y][sel.x])
								|| player == Players::BLACK
								&& board.turn == Players::BLACK
								&& is_black(board.squares[sel.y][sel.x]))
							{
								moves = board.possible_moves(cursor.x, cursor.y, true);
							}
							else
							{
								moves.clear();
							}

							break;

						case keypress::BACKSPACE:
							moves.clear();
							sel = Square(-1, -1);
							break;

						case '>':
						{
							printf("chat > ");
							std::string msg;
							std::getline(std::cin, msg);

							ws_client.write("chat " + msg);
							break;
						}

						default:
							continue;
					}

					print_label:

					print(false);
				}
			}

			void move(Square from, Square to)
			{
				board.move(from, to);

				prev_move_from = from;
				prev_move_to = to;

				print(true);
			}

			void load_from_str(const std::string& str)
			{
				uint8_t x = 0;
				uint8_t y = 0;

				for (size_t i = 0; i < str.size(); i++)
				{
					enum Pieces piece = char_to_piece(str[i]);
					board.squares[y][x] = piece;

					x++;

					if (x == 8)
					{
						x = 0;
						y++;
					}
				}
			}
	};
};

#endif