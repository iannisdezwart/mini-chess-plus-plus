#ifndef CLIENT_GAME_HEADER
#define CLIENT_GAME_HEADER

#include <bits/stdc++.h>
#include "board.hpp"
#include "../Util/keypress.hpp"
#include "../Util/util.hpp"
#include "../WebSocket/websocket-client.hpp"
#include "../WebSocket/ws_messages.hpp"

#define NAME_COLOUR "\e[38;2;225;204;92m"

namespace chess
{
	class ClientGame
	{
		private:
			bool started;
			bool ended;
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
			std::string client_nickname;
			std::string opponent_nickname;

		public:
			ws::WebsocketClient& ws_client;
			std::string room_name;

			ClientGame(enum Players player, ws::WebsocketClient& ws_client,
				std::string&& room_name, const std::string& client_nickname)
					: started(false), ended(false), player(player), ws_client(ws_client),
						client_nickname(client_nickname),	room_name(std::move(room_name)),
						cursor(3, 3), sel(-1, -1), prev_move_from({ -1, -1 }),
						prev_move_to({ -1, -1 }), upsd(player == Players::BLACK)
			{
				keypress::start_getch_mode();
				keypress_thread = std::thread(std::bind(&ClientGame::keypress_handler, this));
				board.initialise_blank();
				print(true);
			}

			void start()
			{
				started = true;
			}

			void set_opponent_nickname(std::string&& nickname)
			{
				opponent_nickname = std::move(nickname);
			}

			void print(bool new_move)
			{
				compute_score();

				printf("\e[2J\e[H"); // Clears screen

				printf("Room: %s\n\n", room_name.c_str());
				printf(NAME_COLOUR "%s " ANSI_RESET, opponent_nickname.c_str());
				print_opponent_score();
				printf("\n\n");

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

				printf("\n" NAME_COLOUR "%s " ANSI_RESET, client_nickname.c_str());
				print_player_score();
				printf("\n");

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

			void compute_score()
			{
				board.compute_score();
			}

			void print_player_score()
			{
				if (player == Players::WHITE) board.print_white_score();
				else board.print_black_score();
			}

			void print_opponent_score()
			{
				if (player == Players::WHITE) board.print_black_score();
				else board.print_white_score();
			}

			void end()
			{
				ended = true;
				keypress_thread.join();
				keypress::stop_getch_mode();
			}

			bool finished()
			{
				return ended;
			}

			void keypress_handler()
			{
				bool stop_flag = false;

				while (true)
				{
					uint8_t key = keypress::get_key([&stop_flag, this]()
					{
						if (finished())
						{
							stop_flag = true;
							return false;
						}

						return true;
					});

					if (stop_flag)
					{
						debug("stopped keypress handler");
						break;
					}

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
									char promotion = '\0';

									if (board.squares[sel.y][sel.x] == Pieces::WHITE_PAWN
										&& cursor.y == 7
										|| board.squares[sel.y][sel.x] == Pieces::BLACK_PAWN
										&& cursor.y == 0)
									{
										printf("select promotion:\n");
										printf("1: " BISHOP ", ");
										printf("2: " KNIGHT ", ");
										printf("3: " ROOK ", ");
										printf("4: " QUEEN "\n");
										try_again:
										printf("> ");

										char choice;
										std::cin >> choice;

										switch (choice)
										{
											case '1':
												promotion = 'B';
												break;

											case '2':
												promotion = 'N';
												break;

											case '3':
												promotion = 'R';
												break;

											case '4':
												promotion = 'Q';
												break;

											default:
												goto try_again;
										}
									}

									ws_client.write(ws_messages::move
										::create_message_with_room_name(
											Square(sel.x, sel.y), Square(cursor.x, cursor.y),
											room_name, promotion));

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

			void move(Square from, Square to, char promotion = '\0')
			{
				board.move(from, to, promotion);

				prev_move_from = from;
				prev_move_to = to;

				print(true);
			}

			void load_from_str(const std::string& str)
			{
				char turn_char = str[0];

				if (turn_char == 'W') board.turn = Players::WHITE;
				else board.turn = Players::BLACK;

				uint8_t x = 0;
				uint8_t y = 0;

				for (size_t i = 1; i < str.size(); i++)
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