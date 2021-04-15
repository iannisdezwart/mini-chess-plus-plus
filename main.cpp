#include <bits/stdc++.h>

#define DEBUG

#include "keypress.hpp"
#include "board.hpp"

int main()
{
	chess::Board board;
	chess::Square cursor(3, 3);
	chess::Square sel(-1, -1);
	std::vector<chess::Square> moves;
	std::vector<chess::Square> warn;
	bool new_move = true;

	while (true)
	{
		printf("\e[2J\e[H"); // Clears screen

		warn.clear();

		if (new_move)
		{
			if (board.white_in_check())
			{
				warn.push_back(board.white_king);
			}

			if (board.black_in_check())
			{
				warn.push_back(board.black_king);
			}
		}

		board.print(cursor, sel, moves, warn);

		if (new_move)
		{
			if (board.turn == chess::Players::WHITE && !board.white_can_move())
			{
				if (board.white_in_check())
				{
					printf("Black won because white is in checkmate\n");
					exit(0);
				}
				else
				{
					printf("Tie because white is in stalemate\n");
					exit(0);
				}
			}

			if (board.turn == chess::Players::BLACK && !board.black_can_move())
			{
				if (board.black_in_check())
				{
					printf("White won because black is in checkmate\n");
					exit(0);
				}
				else
				{
					printf("Tie because black is in stalemate\n");
					exit(0);
				}
			}
		}

		uint8_t key = keypress::get_key();

		switch (key)
		{
			case keypress::ARROW_UP:
				cursor.y++;
				if (cursor.y > 7) cursor.y -= 8;
				break;

			case keypress::ARROW_DOWN:
				cursor.y--;
				if (cursor.y < 0) cursor.y += 8;
				break;

			case keypress::ARROW_RIGHT:
				cursor.x++;
				if (cursor.x > 7) cursor.x -= 8;
				break;

			case keypress::ARROW_LEFT:
				cursor.x--;
				if (cursor.x < 0) cursor.x += 8;
				break;

			case keypress::SPACE:
				for (size_t i = 0; i < moves.size(); i++)
				{
					if (cursor.x == moves[i].x && cursor.y == moves[i].y)
					{
						// Move the piece to the selected square

						board.move(chess::Square(sel.x, sel.y), cursor);
						moves.clear();
						sel = chess::Square(-1, -1);
						new_move = true;
						goto break_new_move;
					}
				}

				// Select this square

				sel = chess::Square(cursor.x, cursor.y);

				if (board.turn == chess::Players::WHITE
					&& chess::is_white(board.squares[sel.y][sel.x])
					|| board.turn == chess::Players::BLACK
					&& chess::is_black(board.squares[sel.y][sel.x]))
				{
					bool in_check = board.turn == chess::Players::WHITE
						? board.white_in_check() : board.black_in_check();
					moves = board.possible_moves(cursor.x, cursor.y, true, in_check);
				}
				else
				{
					moves.clear();
				}

				break;

			case keypress::BACKSPACE:
				moves.clear();
				sel = chess::Square(-1, -1);
				break;

			default:
				continue;
		}

		new_move = false;
		break_new_move:;
	}
}