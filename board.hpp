#ifndef BOARD_HEADER
#define BOARD_HEADER

#include <bits/stdc++.h>

#define BLACK_SQUARE "\e[48;2;100;100;100m"
#define WHITE_SQUARE "\e[48;2;140;140;140m"
#define SEL_SQUARE "\e[48;2;120;120;255m"
#define WARN_SQUARE "\e[48;2;255;120;120m"
#define CURSOR_SQUARE "\e[48;2;255;120;255m"
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

namespace chess
{
	struct Square
	{
		int8_t x;
		int8_t y;

		Square(int8_t x, int8_t y) : x(x), y(y) {}
	};

	enum class Pieces : uint8_t
	{
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

	enum class Players : uint8_t
	{
		WHITE,
		BLACK
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

	bool is_white(enum Pieces piece)
	{
		switch (piece)
		{
			case Pieces::WHITE_KING:
			case Pieces::WHITE_QUEEN:
			case Pieces::WHITE_ROOK:
			case Pieces::WHITE_KNIGHT:
			case Pieces::WHITE_BISHOP:
			case Pieces::WHITE_PAWN:
				return true;

			default:
				return false;
		}
	}

	bool is_black(enum Pieces piece)
	{
		switch (piece)
		{
			case Pieces::BLACK_KING:
			case Pieces::BLACK_QUEEN:
			case Pieces::BLACK_ROOK:
			case Pieces::BLACK_KNIGHT:
			case Pieces::BLACK_BISHOP:
			case Pieces::BLACK_PAWN:
				return true;

			default:
				return false;
		}
	}

	class Board {
		public:
			enum Pieces squares[8][8];
			enum Players turn;

			uint8_t castling_flags = 0;

			#define WKM  0b00000001
			#define WLRM 0b00000010
			#define WRRM 0b00000100
			#define BKM  0b00001000
			#define BLRM 0b00010000
			#define BRRM 0b00100000

			#define WHITE_KING_MOVED       (castling_flags & WKM)
			#define WHITE_LEFT_ROOK_MOVED  (castling_flags & WLRM)
			#define WHITE_RIGHT_ROOK_MOVED (castling_flags & WRRM)
			#define BLACK_KING_MOVED       (castling_flags & BKM)
			#define BLACK_LEFT_ROOK_MOVED  (castling_flags & BLRM)
			#define BLACK_RIGHT_ROOK_MOVED (castling_flags & BRRM)

			#define SET_WHITE_KING_MOVED       castling_flags |= WKM
			#define SET_WHITE_LEFT_ROOK_MOVED  castling_flags |= WLRM
			#define SET_WHITE_RIGHT_ROOK_MOVED castling_flags |= WRRM
			#define SET_BLACK_KING_MOVED       castling_flags |= BKM
			#define SET_BLACK_LEFT_ROOK_MOVED  castling_flags |= BLRM
			#define SET_BLACK_RIGHT_ROOK_MOVED castling_flags |= BRRM

			uint8_t white_en_passant_flags = 0;
			uint8_t black_en_passant_flags = 0;

			Square white_king;
			Square black_king;

			Board() : turn(Players::WHITE), white_king(-1, -1), black_king(-1, -1)
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
				std::vector<Square> highl = std::vector<Square>(),
				std::vector<Square> warn = std::vector<Square>())
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

						// Warn square

						for (size_t i = 0; i < warn.size(); i++)
						{
							if (warn[i].x == col && warn[i].y == row - 1)
							{
								printf(WARN_SQUARE);
							}
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

			bool white_in_check()
			{
				for (uint8_t row = 0; row < 8; row++)
				{
					for (uint8_t col = 0; col < 8; col++)
					{
						if (is_black(squares[row][col]))
						{
							std::vector<Square> moves = possible_moves(col, row, false);

							for (const Square& move : moves)
							{
								if (squares[move.y][move.x] == Pieces::WHITE_KING)
								{
									white_king = Square(move.x, move.y);
									return true;
								}
							}
						}
					}
				}

				return false;
			}

			bool black_in_check()
			{
				for (uint8_t row = 0; row < 8; row++)
				{
					for (uint8_t col = 0; col < 8; col++)
					{
						if (is_white(squares[row][col]))
						{
							std::vector<Square> moves = possible_moves(col, row, false);

							for (const Square& move : moves)
							{
								if (squares[move.y][move.x] == Pieces::BLACK_KING)
								{
									black_king = Square(move.x, move.y);
									return true;
								}
							}
						}
					}
				}

				return false;
			}

			bool white_can_move()
			{
				for (uint8_t row = 0; row < 8; row++)
				{
					for (uint8_t col = 0; col < 8; col++)
					{
						if (is_white(squares[row][col]))
						{
							std::vector<Square> moves = possible_moves(col, row, true, true);
							if (moves.size()) return true;
						}
					}
				}

				return false;
			}

			bool black_can_move()
			{
				for (uint8_t row = 0; row < 8; row++)
				{
					for (uint8_t col = 0; col < 8; col++)
					{
						if (is_black(squares[row][col]))
						{
							std::vector<Square> moves = possible_moves(col, row, true, true);
							if (moves.size()) return true;
						}
					}
				}

				return false;
			}

			std::vector<Square> possible_moves(uint8_t x, uint8_t y,
				bool check_check, bool in_check = false)
			{
				std::vector<Square> moves;
				if (x >= 8 || y >= 8) return moves;

				enum Pieces piece = squares[y][x];

				switch (piece)
				{
					case Pieces::UNOCCUPIED:
						break;

					case Pieces::WHITE_PAWN:
					{
						if (squares[y + 1][x] == Pieces::UNOCCUPIED)
						{
							if (!in_check || is_legal(x, y, x, y + 1))
							{
								moves.push_back(Square(x, y + 1));
							}

							if (y == 1 && squares[y + 2][x] == Pieces::UNOCCUPIED)
							{
								if (!in_check || is_legal(x, y, x, y + 2))
								{
									moves.push_back(Square(x, y + 2));
								}
							}
						}

						if (is_black(squares[y + 1][x - 1]))
						{
							if (!in_check || is_legal(x, y, x - 1, y + 1))
							{
								moves.push_back(Square(x - 1, y + 1));
							}
						}

						if (is_black(squares[y + 1][x + 1]))
						{
							if (!in_check || is_legal(x, y, x + 1, y + 1))
							{
								moves.push_back(Square(x + 1, y + 1));
							}
						}

						// En passant

						if (squares[y][x - 1] == Pieces::BLACK_PAWN
							&& black_en_passant_flags & (x - 1))
						{
							if (!in_check || is_legal(x, y, x - 1, y + 1))
							{
								moves.push_back(Square(x - 1, y + 1));
							}
						}

						if (squares[y][x + 1] == Pieces::BLACK_PAWN
							&& black_en_passant_flags & (x + 1))
						{
							if (!in_check || is_legal(x, y, x + 1, y + 1))
							{
								moves.push_back(Square(x + 1, y + 1));
							}
						}

						break;
					}

					case Pieces::BLACK_PAWN:
					{
						if (squares[y - 1][x] == Pieces::UNOCCUPIED)
						{
							if (!in_check || is_legal(x, y, x, y - 1))
							{
								moves.push_back(Square(x, y - 1));
							}

							if (y == 6 && squares[y - 2][x] == Pieces::UNOCCUPIED)
							{
								if (!in_check || is_legal(x, y, x, y - 2))
								{
									moves.push_back(Square(x, y - 2));
								}
							}
						}

						if (is_white(squares[y - 1][x - 1]))
						{
							if (!in_check || is_legal(x, y, x - 1, y - 1))
							{
								moves.push_back(Square(x - 1, y - 1));
							}
						}

						if (is_white(squares[y - 1][x + 1]))
						{
							if (!in_check || is_legal(x, y, x + 1, y - 1))
							{
								moves.push_back(Square(x + 1, y - 1));
							}
						}

						// En passant

						if (squares[y][x - 1] == Pieces::WHITE_PAWN
							&& white_en_passant_flags & (x - 1))
						{
							if (!in_check || is_legal(x, y, x - 1, y - 1))
							{
								moves.push_back(Square(x - 1, y - 1));
							}
						}

						if (squares[y][x + 1] == Pieces::WHITE_PAWN
							&& white_en_passant_flags & (x + 1))
						{
							if (!in_check || is_legal(x, y, x + 1, y - 1))
							{
								moves.push_back(Square(x + 1, y - 1));
							}
						}

						break;
					}

					case Pieces::WHITE_BISHOP:
					{
						Square diag(x + 1, y + 1);

						while (diag.x < 8 && diag.y < 8)
						{
							if (is_white(squares[diag.y][diag.x])) break;
							if (!in_check || is_legal(x, y, diag.x, diag.y))
							{
								moves.push_back(diag);
							}
							if (is_black(squares[diag.y][diag.x])) break;
							diag.x++;
							diag.y++;
						}

						diag = Square(x + 1, y - 1);

						while (diag.x < 8 && diag.y >= 0)
						{
							if (is_white(squares[diag.y][diag.x])) break;
							if (!in_check || is_legal(x, y, diag.x, diag.y))
							{
								moves.push_back(diag);
							}
							if (is_black(squares[diag.y][diag.x])) break;
							diag.x++;
							diag.y--;
						}

						diag = Square(x - 1, y - 1);

						while (diag.x >= 0 && diag.y >= 0)
						{
							if (is_white(squares[diag.y][diag.x])) break;
							if (!in_check || is_legal(x, y, diag.x, diag.y))
							{
								moves.push_back(diag);
							}
							if (is_black(squares[diag.y][diag.x])) break;
							diag.x--;
							diag.y--;
						}

						diag = Square(x - 1, y + 1);

						while (diag.x >= 0 && diag.y < 8)
						{
							if (is_white(squares[diag.y][diag.x])) break;
							if (!in_check || is_legal(x, y, diag.x, diag.y))
							{
								moves.push_back(diag);
							}
							if (is_black(squares[diag.y][diag.x])) break;
							diag.x--;
							diag.y++;
						}

						break;
					}

					case Pieces::BLACK_BISHOP:
					{
						Square diag(x + 1, y + 1);

						while (diag.x < 8 && diag.y < 8)
						{
							if (is_black(squares[diag.y][diag.x])) break;
							if (!in_check || is_legal(x, y, diag.x, diag.y))
							{
								moves.push_back(diag);
							}
							if (is_white(squares[diag.y][diag.x])) break;
							diag.x++;
							diag.y++;
						}

						diag = Square(x + 1, y - 1);

						while (diag.x < 8 && diag.y >= 0)
						{
							if (is_black(squares[diag.y][diag.x])) break;
							if (!in_check || is_legal(x, y, diag.x, diag.y))
							{
								moves.push_back(diag);
							}
							if (is_white(squares[diag.y][diag.x])) break;
							diag.x++;
							diag.y--;
						}

						diag = Square(x - 1, y - 1);

						while (diag.x >= 0 && diag.y >= 0)
						{
							if (is_black(squares[diag.y][diag.x])) break;
							if (!in_check || is_legal(x, y, diag.x, diag.y))
							{
								moves.push_back(diag);
							}
							if (is_white(squares[diag.y][diag.x])) break;
							diag.x--;
							diag.y--;
						}

						diag = Square(x - 1, y + 1);

						while (diag.x >= 0 && diag.y < 8)
						{
							if (is_black(squares[diag.y][diag.x])) break;
							if (!in_check || is_legal(x, y, diag.x, diag.y))
							{
								moves.push_back(diag);
							}
							if (is_white(squares[diag.y][diag.x])) break;
							diag.x--;
							diag.y++;
						}

						break;
					}

					case Pieces::WHITE_KNIGHT:
					{
						if (x - 1 >= 0 && y - 2 >= 0 && !is_white(squares[y - 2][x - 1]))
						{
							if (!in_check || is_legal(x, y, x - 1, y - 2))
							{
								moves.push_back(Square(x - 1, y - 2));
							}
						}

						if (x - 2 >= 0 && y - 1 >= 0 && !is_white(squares[y - 1][x - 2]))
						{
							if (!in_check || is_legal(x, y, x - 2, y - 1))
							{
								moves.push_back(Square(x - 2, y - 1));
							}
						}

						if (x + 1 < 8 && y - 2 >= 0 && !is_white(squares[y - 2][x + 1]))
						{
							if (!in_check || is_legal(x, y, x + 1, y - 2))
							{
								moves.push_back(Square(x + 1, y - 2));
							}
						}

						if (x + 2 < 8 && y - 1 >= 0 && !is_white(squares[y - 1][x + 2]))
						{
							if (!in_check || is_legal(x, y, x + 2, y - 1))
							{
								moves.push_back(Square(x + 2, y - 1));
							}
						}

						if (x - 1 >= 0 && y + 2 < 8 && !is_white(squares[y + 2][x - 1]))
						{
							if (!in_check || is_legal(x, y, x - 1, y + 2))
							{
								moves.push_back(Square(x - 1, y + 2));
							}
						}

						if (x - 2 >= 0 && y + 1 < 8 && !is_white(squares[y + 1][x - 2]))
						{
							if (!in_check || is_legal(x, y, x - 2, y + 1))
							{
								moves.push_back(Square(x - 2, y + 1));
							}
						}

						if (x + 1 < 8 && y + 2 < 8 && !is_white(squares[y + 2][x + 1]))
						{
							if (!in_check || is_legal(x, y, x + 1, y + 2))
							{
								moves.push_back(Square(x + 1, y + 2));
							}
						}

						if (x + 2 < 8 && y + 1 < 8 && !is_white(squares[y + 1][x + 2]))
						{
							if (!in_check || is_legal(x, y, x + 2, y + 1))
							{
								moves.push_back(Square(x + 2, y + 1));
							}
						}

						break;
					}

					case Pieces::BLACK_KNIGHT:
					{
						if (x - 1 >= 0 && y - 2 >= 0 && !is_black(squares[y - 2][x - 1]))
						{
							if (!in_check || is_legal(x, y, x - 1, y - 2))
							{
								moves.push_back(Square(x - 1, y - 2));
							}
						}

						if (x - 2 >= 0 && y - 1 >= 0 && !is_black(squares[y - 1][x - 2]))
						{
							if (!in_check || is_legal(x, y, x - 2, y - 1))
							{
								moves.push_back(Square(x - 2, y - 1));
							}
						}

						if (x + 1 < 8 && y - 2 >= 0 && !is_black(squares[y - 2][x + 1]))
						{
							if (!in_check || is_legal(x, y, x + 1, y - 2))
							{
								moves.push_back(Square(x + 1, y - 2));
							}
						}

						if (x + 2 < 8 && y - 1 >= 0 && !is_black(squares[y - 1][x + 2]))
						{
							if (!in_check || is_legal(x, y, x + 2, y - 1))
							{
								moves.push_back(Square(x + 2, y - 1));
							}
						}

						if (x - 1 >= 0 && y + 2 < 8 && !is_black(squares[y + 2][x - 1]))
						{
							if (!in_check || is_legal(x, y, x - 1, y + 2))
							{
								moves.push_back(Square(x - 1, y + 2));
							}
						}

						if (x - 2 >= 0 && y + 1 < 8 && !is_black(squares[y + 1][x - 2]))
						{
							if (!in_check || is_legal(x, y, x - 2, y + 1))
							{
								moves.push_back(Square(x - 2, y + 1));
							}
						}

						if (x + 1 < 8 && y + 2 < 8 && !is_black(squares[y + 2][x + 1]))
						{
							if (!in_check || is_legal(x, y, x + 1, y + 2))
							{
								moves.push_back(Square(x + 1, y + 2));
							}
						}

						if (x + 2 < 8 && y + 1 < 8 && !is_black(squares[y + 1][x + 2]))
						{
							if (!in_check || is_legal(x, y, x + 2, y + 1))
							{
								moves.push_back(Square(x + 2, y + 1));
							}
						}

						break;
					}

					case Pieces::WHITE_ROOK:
					{
						Square line(x + 1, y);

						while (line.x < 8)
						{
							if (is_white(squares[line.y][line.x])) break;
							if (!in_check || is_legal(x, y, line.x, line.y))
							{
								moves.push_back(line);
							}
							if (is_black(squares[line.y][line.x])) break;
							line.x++;
						}

						line = Square(x - 1, y);

						while (line.x >= 0)
						{
							if (is_white(squares[line.y][line.x])) break;
							if (!in_check || is_legal(x, y, line.x, line.y))
							{
								moves.push_back(line);
							}
							if (is_black(squares[line.y][line.x])) break;
							line.x--;
						}

						line = Square(x, y + 1);

						while (line.y < 8)
						{
							if (is_white(squares[line.y][line.x])) break;
							if (!in_check || is_legal(x, y, line.x, line.y))
							{
								moves.push_back(line);
							}
							if (is_black(squares[line.y][line.x])) break;
							line.y++;
						}

						line = Square(x, y - 1);

						while (line.y >= 0)
						{
							if (is_white(squares[line.y][line.x])) break;
							if (!in_check || is_legal(x, y, line.x, line.y))
							{
								moves.push_back(line);
							}
							if (is_black(squares[line.y][line.x])) break;
							line.y--;
						}

						break;
					}

					case Pieces::BLACK_ROOK:
					{
						Square line(x + 1, y);

						while (line.x < 8)
						{
							if (is_black(squares[line.y][line.x])) break;
							if (!in_check || is_legal(x, y, line.x, line.y))
							{
								moves.push_back(line);
							}
							if (is_white(squares[line.y][line.x])) break;
							line.x++;
						}

						line = Square(x - 1, y);

						while (line.x >= 0)
						{
							if (is_black(squares[line.y][line.x])) break;
							if (!in_check || is_legal(x, y, line.x, line.y))
							{
								moves.push_back(line);
							}
							if (is_white(squares[line.y][line.x])) break;
							line.x--;
						}

						line = Square(x, y + 1);

						while (line.y < 8)
						{
							if (is_black(squares[line.y][line.x])) break;
							if (!in_check || is_legal(x, y, line.x, line.y))
							{
								moves.push_back(line);
							}
							if (is_white(squares[line.y][line.x])) break;
							line.y++;
						}

						line = Square(x, y - 1);

						while (line.y >= 0)
						{
							if (is_black(squares[line.y][line.x])) break;
							if (!in_check || is_legal(x, y, line.x, line.y))
							{
								moves.push_back(line);
							}
							if (is_white(squares[line.y][line.x])) break;
							line.y--;
						}

						break;
					}

					case Pieces::WHITE_QUEEN:
					{
						Square diag(x + 1, y + 1);

						while (diag.x < 8 && diag.y < 8)
						{
							if (is_white(squares[diag.y][diag.x])) break;
							if (!in_check || is_legal(x, y, diag.x, diag.y))
							{
								moves.push_back(diag);
							}
							if (is_black(squares[diag.y][diag.x])) break;
							diag.x++;
							diag.y++;
						}

						diag = Square(x + 1, y - 1);

						while (diag.x < 8 && diag.y >= 0)
						{
							if (is_white(squares[diag.y][diag.x])) break;
							if (!in_check || is_legal(x, y, diag.x, diag.y))
							{
								moves.push_back(diag);
							}
							if (is_black(squares[diag.y][diag.x])) break;
							diag.x++;
							diag.y--;
						}

						diag = Square(x - 1, y - 1);

						while (diag.x >= 0 && diag.y >= 0)
						{
							if (is_white(squares[diag.y][diag.x])) break;
							if (!in_check || is_legal(x, y, diag.x, diag.y))
							{
								moves.push_back(diag);
							}
							if (is_black(squares[diag.y][diag.x])) break;
							diag.x--;
							diag.y--;
						}

						diag = Square(x - 1, y + 1);

						while (diag.x >= 0 && diag.y < 8)
						{
							if (is_white(squares[diag.y][diag.x])) break;
							if (!in_check || is_legal(x, y, diag.x, diag.y))
							{
								moves.push_back(diag);
							}
							if (is_black(squares[diag.y][diag.x])) break;
							diag.x--;
							diag.y++;
						}

						Square line(x + 1, y);

						while (line.x < 8)
						{
							if (is_white(squares[line.y][line.x])) break;
							if (!in_check || is_legal(x, y, line.x, line.y))
							{
								moves.push_back(line);
							}
							if (is_black(squares[line.y][line.x])) break;
							line.x++;
						}

						line = Square(x - 1, y);

						while (line.x >= 0)
						{
							if (is_white(squares[line.y][line.x])) break;
							if (!in_check || is_legal(x, y, line.x, line.y))
							{
								moves.push_back(line);
							}
							if (is_black(squares[line.y][line.x])) break;
							line.x--;
						}

						line = Square(x, y + 1);

						while (line.y < 8)
						{
							if (is_white(squares[line.y][line.x])) break;
							if (!in_check || is_legal(x, y, line.x, line.y))
							{
								moves.push_back(line);
							}
							if (is_black(squares[line.y][line.x])) break;
							line.y++;
						}

						line = Square(x, y - 1);

						while (line.y >= 0)
						{
							if (is_white(squares[line.y][line.x])) break;
							if (!in_check || is_legal(x, y, line.x, line.y))
							{
								moves.push_back(line);
							}
							if (is_black(squares[line.y][line.x])) break;
							line.y--;
						}

						break;
					}

					case Pieces::BLACK_QUEEN:
					{
						Square diag(x + 1, y + 1);

						while (diag.x < 8 && diag.y < 8)
						{
							if (is_black(squares[diag.y][diag.x])) break;
							if (!in_check || is_legal(x, y, diag.x, diag.y))
							{
								moves.push_back(diag);
							}
							if (is_white(squares[diag.y][diag.x])) break;
							diag.x++;
							diag.y++;
						}

						diag = Square(x + 1, y - 1);

						while (diag.x < 8 && diag.y >= 0)
						{
							if (is_black(squares[diag.y][diag.x])) break;
							if (!in_check || is_legal(x, y, diag.x, diag.y))
							{
								moves.push_back(diag);
							}
							if (is_white(squares[diag.y][diag.x])) break;
							diag.x++;
							diag.y--;
						}

						diag = Square(x - 1, y - 1);

						while (diag.x >= 0 && diag.y >= 0)
						{
							if (is_black(squares[diag.y][diag.x])) break;
							if (!in_check || is_legal(x, y, diag.x, diag.y))
							{
								moves.push_back(diag);
							}
							if (is_white(squares[diag.y][diag.x])) break;
							diag.x--;
							diag.y--;
						}

						diag = Square(x - 1, y + 1);

						while (diag.x >= 0 && diag.y < 8)
						{
							if (is_black(squares[diag.y][diag.x])) break;
							if (!in_check || is_legal(x, y, diag.x, diag.y))
							{
								moves.push_back(diag);
							}
							if (is_white(squares[diag.y][diag.x])) break;
							diag.x--;
							diag.y++;
						}

						Square line(x + 1, y);

						while (line.x < 8)
						{
							if (is_black(squares[line.y][line.x])) break;
							if (!in_check || is_legal(x, y, line.x, line.y))
							{
								moves.push_back(line);
							}
							if (is_white(squares[line.y][line.x])) break;
							line.x++;
						}

						line = Square(x - 1, y);

						while (line.x >= 0)
						{
							if (is_black(squares[line.y][line.x])) break;
							if (!in_check || is_legal(x, y, line.x, line.y))
							{
								moves.push_back(line);
							}
							if (is_white(squares[line.y][line.x])) break;
							line.x--;
						}

						line = Square(x, y + 1);

						while (line.y < 8)
						{
							if (is_black(squares[line.y][line.x])) break;
							if (!in_check || is_legal(x, y, line.x, line.y))
							{
								moves.push_back(line);
							}
							if (is_white(squares[line.y][line.x])) break;
							line.y++;
						}

						line = Square(x, y - 1);

						while (line.y >= 0)
						{
							if (is_black(squares[line.y][line.x])) break;
							if (!in_check || is_legal(x, y, line.x, line.y))
							{
								moves.push_back(line);
							}
							if (is_white(squares[line.y][line.x])) break;
							line.y--;
						}

						break;
					}

					case Pieces::WHITE_KING:
					{
						Square orig(x, y);
						Square diag(x + 1, y + 1);

						if (diag.x < 8 && diag.y < 8
							&& !is_white(squares[diag.y][diag.x]))
						{
							if (check_check)
							{
								enum Pieces old_piece = pretend(orig, diag);
								if (!white_in_check()) moves.push_back(diag);
								unpretend(orig, diag, old_piece);
							}
							else
							{
								moves.push_back(diag);
							}
						}

						diag = Square(x - 1, y + 1);

						if (diag.x >= 0 && diag.y < 8
							&& !is_white(squares[diag.y][diag.x]))
						{
							if (check_check)
							{
								enum Pieces old_piece = pretend(orig, diag);
								if (!white_in_check()) moves.push_back(diag);
								unpretend(orig, diag, old_piece);
							}
							else
							{
								moves.push_back(diag);
							}
						}

						diag = Square(x - 1, y - 1);

						if (diag.x >= 0 && diag.y >= 0
							&& !is_white(squares[diag.y][diag.x]))
						{
							if (check_check)
							{
								enum Pieces old_piece = pretend(orig, diag);
								if (!white_in_check()) moves.push_back(diag);
								unpretend(orig, diag, old_piece);
							}
							else
							{
								moves.push_back(diag);
							}
						}

						diag = Square(x + 1, y - 1);

						if (diag.x < 8 && diag.y >= 0
							&& !is_white(squares[diag.y][diag.x]))
						{
							if (check_check)
							{
								enum Pieces old_piece = pretend(orig, diag);
								if (!white_in_check()) moves.push_back(diag);
								unpretend(orig, diag, old_piece);
							}
							else
							{
								moves.push_back(diag);
							}
						}

						Square line(x + 1, y);

						if (line.x < 8 && !is_white(squares[line.y][line.x]))
						{
							if (check_check)
							{
								enum Pieces old_piece = pretend(orig, line);
								if (!white_in_check()) moves.push_back(line);
								unpretend(orig, line, old_piece);
							}
							else
							{
								moves.push_back(line);
							}
						}

						line = Square(x - 1, y);

						if (line.x >= 0 && !is_white(squares[line.y][line.x]))
						{
							if (check_check)
							{
								enum Pieces old_piece = pretend(orig, line);
								if (!white_in_check()) moves.push_back(line);
								unpretend(orig, line, old_piece);
							}
							else
							{
								moves.push_back(line);
							}
						}

						line = Square(x, y + 1);

						if (line.y < 8 && !is_white(squares[line.y][line.x]))
						{
							if (check_check)
							{
								enum Pieces old_piece = pretend(orig, line);
								if (!white_in_check()) moves.push_back(line);
								unpretend(orig, line, old_piece);
							}
							else
							{
								moves.push_back(line);
							}
						}

						line = Square(x, y - 1);

						if (line.y >= 0 && !is_white(squares[line.y][line.x]))
						{
							if (check_check)
							{
								enum Pieces old_piece = pretend(orig, line);
								if (!white_in_check()) moves.push_back(line);
								unpretend(orig, line, old_piece);
							}
							else
							{
								moves.push_back(line);
							}
						}

						// Castling

						if (!WHITE_KING_MOVED && !WHITE_RIGHT_ROOK_MOVED
							&& squares[y][x + 1] == Pieces::UNOCCUPIED
							&& squares[y][x + 2] == Pieces::UNOCCUPIED
							&& !in_check
							&& is_legal(x, y, x + 1, y)
							&& is_legal(x, y, x + 2, y))
						{
							moves.push_back(Square(x + 2, y));
						}

						if (!WHITE_KING_MOVED && !WHITE_LEFT_ROOK_MOVED
							&& squares[y][x - 1] == Pieces::UNOCCUPIED
							&& squares[y][x - 2] == Pieces::UNOCCUPIED
							&& squares[y][x - 3] == Pieces::UNOCCUPIED
							&& !in_check
							&& is_legal(x, y, x - 1, y)
							&& is_legal(x, y, x - 2, y))
						{
							moves.push_back(Square(x - 2, y));
						}

						break;
					}

					case Pieces::BLACK_KING:
					{
						Square orig(x, y);
						Square diag(x + 1, y + 1);

						if (diag.x < 8 && diag.y < 8
							&& !is_black(squares[diag.y][diag.x]))
						{
							if (check_check)
							{
								enum Pieces old_piece = pretend(orig, diag);
								if (!black_in_check()) moves.push_back(diag);
								unpretend(orig, diag, old_piece);
							}
							else
							{
								moves.push_back(diag);
							}
						}

						diag = Square(x - 1, y + 1);

						if (diag.x >= 0 && diag.y < 8
							&& !is_black(squares[diag.y][diag.x]))
						{
							if (check_check)
							{
								enum Pieces old_piece = pretend(orig, diag);
								if (!black_in_check()) moves.push_back(diag);
								unpretend(orig, diag, old_piece);
							}
							else
							{
								moves.push_back(diag);
							}
						}

						diag = Square(x - 1, y - 1);

						if (diag.x >= 0 && diag.y >= 0
							&& !is_black(squares[diag.y][diag.x]))
						{
							if (check_check)
							{
								enum Pieces old_piece = pretend(orig, diag);
								if (!black_in_check()) moves.push_back(diag);
								unpretend(orig, diag, old_piece);
							}
							else
							{
								moves.push_back(diag);
							}
						}

						diag = Square(x + 1, y - 1);

						if (diag.x < 8 && diag.y >= 0
							&& !is_black(squares[diag.y][diag.x]))
						{
							if (check_check)
							{
								enum Pieces old_piece = pretend(orig, diag);
								if (!black_in_check()) moves.push_back(diag);
								unpretend(orig, diag, old_piece);
							}
							else
							{
								moves.push_back(diag);
							}
						}

						Square line(x + 1, y);

						if (line.x < 8 && !is_black(squares[line.y][line.x]))
						{
							if (check_check)
							{
								enum Pieces old_piece = pretend(orig, line);
								if (!black_in_check()) moves.push_back(line);
								unpretend(orig, line, old_piece);
							}
							else
							{
								moves.push_back(line);
							}
						}

						line = Square(x - 1, y);

						if (line.x >= 0 && !is_black(squares[line.y][line.x]))
						{
							if (check_check)
							{
								enum Pieces old_piece = pretend(orig, line);
								if (!black_in_check()) moves.push_back(line);
								unpretend(orig, line, old_piece);
							}
							else
							{
								moves.push_back(line);
							}
						}

						line = Square(x, y + 1);

						if (line.y < 8 && !is_black(squares[line.y][line.x]))
						{
							if (check_check)
							{
								enum Pieces old_piece = pretend(orig, line);
								if (!black_in_check()) moves.push_back(line);
								unpretend(orig, line, old_piece);
							}
							else
							{
								moves.push_back(line);
							}
						}

						line = Square(x, y - 1);

						if (line.y >= 0 && !is_black(squares[line.y][line.x]))
						{
							if (check_check)
							{
								enum Pieces old_piece = pretend(orig, line);
								if (!black_in_check()) moves.push_back(line);
								unpretend(orig, line, old_piece);
							}
							else
							{
								moves.push_back(line);
							}
						}

						// Castling

						if (!BLACK_KING_MOVED && !BLACK_RIGHT_ROOK_MOVED
							&& squares[y][x + 1] == Pieces::UNOCCUPIED
							&& squares[y][x + 2] == Pieces::UNOCCUPIED
							&& !in_check
							&& is_legal(x, y, x + 1, y)
							&& is_legal(x, y, x + 2, y))
						{
							moves.push_back(Square(x + 2, y));
						}

						if (!BLACK_KING_MOVED && !BLACK_LEFT_ROOK_MOVED
							&& squares[y][x - 1] == Pieces::UNOCCUPIED
							&& squares[y][x - 2] == Pieces::UNOCCUPIED
							&& squares[y][x - 3] == Pieces::UNOCCUPIED
							&& !in_check
							&& is_legal(x, y, x - 1, y)
							&& is_legal(x, y, x - 2, y))
						{
							moves.push_back(Square(x - 2, y));
						}

						break;
					}
				}

				return moves;
			}

			enum Pieces pretend(Square from, Square to)
			{
				enum Pieces old_piece = squares[to.y][to.x];
				enum Pieces moved_piece = squares[from.y][from.x];
				squares[from.y][from.x] = Pieces::UNOCCUPIED;
				squares[to.y][to.x] = moved_piece;
				return old_piece;
			}

			void unpretend(Square from, Square to, enum Pieces old_piece)
			{
				squares[from.y][from.x] = squares[to.y][to.x];
				squares[to.y][to.x] = old_piece;
			}

			bool is_legal(uint8_t x_from, uint8_t y_from,
				uint8_t x_to, uint8_t y_to)
			{
				Square from(x_from, y_from);
				Square to(x_to, y_to);

				enum Pieces old_piece = pretend(from, to);

				bool check;

				if (turn == Players::WHITE)
				{
					check = white_in_check();
				}
				else
				{
					check = black_in_check();
				}

				unpretend(from, to, old_piece);

				return !check;
			}

			void move(Square from, Square to)
			{
				enum Pieces moved_piece = squares[from.y][from.x];
				squares[from.y][from.x] = Pieces::UNOCCUPIED;
				squares[to.y][to.x] = moved_piece;

				// Castling

				if (moved_piece == Pieces::WHITE_KING && to.x - from.x == 2)
				{
					squares[from.y][from.x + 1] = Pieces::WHITE_ROOK;
					squares[from.y][from.x + 3] = Pieces::UNOCCUPIED;
				}

				if (moved_piece == Pieces::WHITE_KING && from.x - to.x == 2)
				{
					squares[from.y][from.x - 1] = Pieces::WHITE_ROOK;
					squares[from.y][from.x - 4] = Pieces::UNOCCUPIED;
				}

				if (moved_piece == Pieces::BLACK_KING && to.x - from.x == 2)
				{
					squares[from.y][from.x + 1] = Pieces::BLACK_ROOK;
					squares[from.y][from.x + 3] = Pieces::UNOCCUPIED;
				}

				if (moved_piece == Pieces::BLACK_KING && from.x - to.x == 2)
				{
					squares[from.y][from.x - 1] = Pieces::BLACK_ROOK;
					squares[from.y][from.x - 4] = Pieces::UNOCCUPIED;
				}

				// Keep track of castling legality

				if (moved_piece == Pieces::WHITE_KING)
				{
					debug("setting white king moved");
					SET_WHITE_KING_MOVED;
				}

				if (!WHITE_RIGHT_ROOK_MOVED && moved_piece == Pieces::WHITE_ROOK
					&& from.x == 7 || squares[0][7] != Pieces::WHITE_ROOK)
				{
					debug("setting white right rook moved");
					SET_WHITE_RIGHT_ROOK_MOVED;
				}

				if (!WHITE_LEFT_ROOK_MOVED && moved_piece == Pieces::WHITE_ROOK
					&& from.x == 0 || squares[0][0] != Pieces::WHITE_ROOK)
				{
					debug("setting white left rook moved");
					SET_WHITE_LEFT_ROOK_MOVED;
				}

				if (moved_piece == Pieces::BLACK_KING)
				{
					debug("setting black king moved");
					SET_BLACK_KING_MOVED;
				}

				if (!BLACK_RIGHT_ROOK_MOVED && moved_piece == Pieces::BLACK_ROOK
					&& from.x == 7 || squares[7][7] != Pieces::BLACK_ROOK)
				{
					debug("setting black right rook moved");
					SET_BLACK_RIGHT_ROOK_MOVED;
				}

				if (!BLACK_LEFT_ROOK_MOVED && moved_piece == Pieces::BLACK_ROOK
					&& from.x == 0 || squares[7][0] != Pieces::BLACK_ROOK)
				{
					debug("setting black right rook moved");
					SET_BLACK_LEFT_ROOK_MOVED;
				}

				// En passant

				if (turn == Players::WHITE) white_en_passant_flags = 0;
				else black_en_passant_flags = 0;

				if (moved_piece == Pieces::WHITE_PAWN && to.y - from.y == 2)
				{
					debug("white en passant flag %d set", from.x);
					white_en_passant_flags |= from.x;
				}

				if (moved_piece == Pieces::BLACK_PAWN && from.y - to.y == 2)
				{
					debug("black en passant flag %d set", from.x);
					black_en_passant_flags |= from.x;
				}

				if (moved_piece == Pieces::WHITE_PAWN &&
					black_en_passant_flags & to.x && to.x != from.x)
				{
					debug("ate black pawn en passant");
					squares[from.y][to.x] = Pieces::UNOCCUPIED;
				}

				if (moved_piece == Pieces::BLACK_PAWN &&
					white_en_passant_flags & to.x && to.x != from.x)
				{
					debug("ate white pawn en passant");
					squares[from.y][to.x] = Pieces::UNOCCUPIED;
				}

				if (turn == Players::WHITE) turn = Players::BLACK;
				else turn = Players::WHITE;
			}
	};
};

#endif