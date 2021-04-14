#include <bits/stdc++.h>

#include "keypress.hpp"
#include "board.hpp"

int main()
{
	chess::Board board;
	chess::Square cursor(3, 3);
	chess::Square sel((uint8_t) -1, (uint8_t) -1);
	std::vector<chess::Square> moves;

	while (true)
	{
		printf("\e[2J\e[H"); // Clears screen
		board.print(cursor, sel, moves);
		uint8_t key = keypress::get_key();

		switch (key)
		{
			case keypress::ARROW_UP:
				cursor.y++;
				cursor.y %= 8;
				break;

			case keypress::ARROW_DOWN:
				cursor.y--;
				cursor.y %= 8;
				break;

			case keypress::ARROW_RIGHT:
				cursor.x++;
				cursor.x %= 8;
				break;

			case keypress::ARROW_LEFT:
				cursor.x--;
				cursor.x %= 8;
				break;

			case keypress::SPACE:
				for (size_t i = 0; i < moves.size(); i++)
				{
					if (cursor.x == moves[i].x && cursor.y == moves[i].y)
					{
						// Move the piece to the selected square

						board.move(chess::Square(sel.x, sel.y), cursor);
						moves.clear();
						sel = chess::Square((uint8_t) -1, (uint8_t) -1);
						goto _break;
					}
				}

				// Select this square

				sel = chess::Square(cursor.x, cursor.y);
				moves = board.possible_moves(cursor.x, cursor.y);
				_break: break;

			case keypress::BACKSPACE:
				moves.clear();
				sel = chess::Square((uint8_t) -1, (uint8_t) -1);
				break;

			default:
				continue;
		}
	}
}