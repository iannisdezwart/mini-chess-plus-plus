#ifndef BOARD_HEADER
#define BOARD_HEADER

#include <bits/stdc++.h>

#define BLACK_SQUARE "\e[48;2;100;100;100m"
#define WHITE_SQUARE "\e[48;2;140;140;140m"
#define SEL_SQUARE "\e[48;2;120;120;255m"
#define CURSOR_SQUARE "\e[48;2;255;120;120m"
#define HIGHL_COLOUR "\e[38;2;120;255;120m"
#define HIGHL_CHAR "*"
#define BLACK_PIECE "\e[38;2;0;0;0m"
#define WHITE_PIECE "\e[38;2;255;255;255m"
#define KING "♚"
#define QUEEN "♛"
#define ROOK "♜"
#define KNIGHT "♞"
#define BISHOP "♝"
#define PAWN "♟︎"

namespace chess {
	struct Square {
		uint8_t x;
		uint8_t y;

		Square(uint8_t x, uint8_t y) : x(x), y(y) {}
	};

	enum class Pieces : uint8_t {
		WHITE_KING,
		WHITE_QUEEN,
		WHITE_ROOK,
		WHITE_KNIGHT,
		WHITE_BISHOP,
		WHITE_PAWN,
		BLACK_KING,
		BLACK_QUEEN,
		BLACK_ROOK,
		BLACK_KNIGHT,
		BLACK_BISHOP,
		BLACK_PAWN,
		UNOCCUPIED
	};

	const char *piece_to_str(enum Pieces piece)
	{
		switch (piece)
		{
			case Pieces::WHITE_KING:   return WHITE_PIECE KING;
			case Pieces::WHITE_QUEEN:  return WHITE_PIECE QUEEN;
			case Pieces::WHITE_ROOK:   return WHITE_PIECE ROOK;
			case Pieces::WHITE_KNIGHT: return WHITE_PIECE KNIGHT;
			case Pieces::WHITE_BISHOP: return WHITE_PIECE BISHOP;
			case Pieces::WHITE_PAWN:   return WHITE_PIECE PAWN;
			case Pieces::BLACK_KING:   return BLACK_PIECE KING;
			case Pieces::BLACK_QUEEN:  return BLACK_PIECE QUEEN;
			case Pieces::BLACK_ROOK:   return BLACK_PIECE ROOK;
			case Pieces::BLACK_KNIGHT: return BLACK_PIECE KNIGHT;
			case Pieces::BLACK_BISHOP: return BLACK_PIECE BISHOP;
			case Pieces::BLACK_PAWN:   return BLACK_PIECE PAWN;
			case Pieces::UNOCCUPIED:   return " ";
			default: return "?";
		}
	}

	class Board {
		public:
			enum Pieces squares[8][8];

			Board()
			{
				squares[0][0] = Pieces::WHITE_ROOK;
				squares[0][1] = Pieces::WHITE_KNIGHT;
				squares[0][2] = Pieces::WHITE_BISHOP;
				squares[0][3] = Pieces::WHITE_QUEEN;
				squares[0][4] = Pieces::WHITE_KING;
				squares[0][5] = Pieces::WHITE_BISHOP;
				squares[0][6] = Pieces::WHITE_KNIGHT;
				squares[0][7] = Pieces::WHITE_ROOK;

				squares[1][0] = Pieces::WHITE_PAWN;
				squares[1][1] = Pieces::WHITE_PAWN;
				squares[1][2] = Pieces::WHITE_PAWN;
				squares[1][3] = Pieces::WHITE_PAWN;
				squares[1][4] = Pieces::WHITE_PAWN;
				squares[1][5] = Pieces::WHITE_PAWN;
				squares[1][6] = Pieces::WHITE_PAWN;
				squares[1][7] = Pieces::WHITE_PAWN;

				for (uint8_t row = 2; row < 6; row++)
				{
					for (uint8_t col = 0; col < 8; col++)
					{
						squares[row][col] = Pieces::UNOCCUPIED;
					}
				}

				squares[6][0] = Pieces::BLACK_PAWN;
				squares[6][1] = Pieces::BLACK_PAWN;
				squares[6][2] = Pieces::BLACK_PAWN;
				squares[6][3] = Pieces::BLACK_PAWN;
				squares[6][4] = Pieces::BLACK_PAWN;
				squares[6][5] = Pieces::BLACK_PAWN;
				squares[6][6] = Pieces::BLACK_PAWN;
				squares[6][7] = Pieces::BLACK_PAWN;

				squares[7][0] = Pieces::BLACK_ROOK;
				squares[7][1] = Pieces::BLACK_KNIGHT;
				squares[7][2] = Pieces::BLACK_BISHOP;
				squares[7][3] = Pieces::BLACK_QUEEN;
				squares[7][4] = Pieces::BLACK_KING;
				squares[7][5] = Pieces::BLACK_BISHOP;
				squares[7][6] = Pieces::BLACK_KNIGHT;
				squares[7][7] = Pieces::BLACK_ROOK;
			}

			void print(Square cursor, Square sel = { 8, 8 },
				std::vector<Square> highl = std::vector<Square>())
			{
				for (uint8_t row = 8; row != 0; row--)
				{
					printf("%hhu  ", row);

					for (uint8_t col = 0; col < 8; col++)
					{
						if (row % 2 == 0 && col % 2 == 0 || row % 2 && col % 2)
						{
							printf(WHITE_SQUARE);
						}
						else
						{
							printf(BLACK_SQUARE);
						}

						// Selected square

						if (sel.x == col && sel.y == row - 1)
						{
							printf(SEL_SQUARE);
						}

						// Cursor square

						if (cursor.x == col && cursor.y == row - 1)
						{
							printf(CURSOR_SQUARE);
						}

						// Print the piece

						printf("%s", piece_to_str(squares[row - 1][col]));

						// Print highlighting

						for (size_t i = 0; i < highl.size(); i++)
						{
							if (highl[i].x == col && highl[i].y == row - 1)
							{
								printf(HIGHL_COLOUR HIGHL_CHAR);
								goto end_highl;
							}
						}

						printf(" ");
						end_highl:;
					}

					printf("\e[m\n");
				}

				printf("\n   ");

				for (uint8_t i = 0; i < 8; i++)
				{
					printf("%c ", 'A' + i);
				}

				printf("\n");
			}

			std::vector<Square> possible_moves(uint8_t x, uint8_t y)
			{
				std::vector<Square> moves;
				if (x >= 8 || y >= 8) return moves;

				enum Pieces piece = squares[y][x];

				switch (piece)
				{
					case Pieces::UNOCCUPIED: return moves;

					case Pieces::WHITE_PAWN:
						moves.push_back(Square(x, y + 1));

						if (y == 1)
						{
							moves.push_back(Square(x, y + 2));
						}

						break;

					case Pieces::BLACK_PAWN:
						moves.push_back(Square(x, y - 1));

						if (y == 6)
						{
							moves.push_back(Square(x, y - 2));
						}

						break;
				}

				return moves;
			}

			void move(Square from, Square to)
			{
				enum Pieces moved_piece = squares[from.y][from.x];
				squares[from.y][from.x] = Pieces::UNOCCUPIED;
				squares[to.y][to.x] = moved_piece;
			}
	};
};

#endif