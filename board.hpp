#ifndef BOARD_HEADER
#define BOARD_HEADER

#include <bits/stdc++.h>

#define BLACK_SQUARE "\e[48;2;100;100;100m"
#define WHITE_SQUARE "\e[48;2;140;140;140m"
#define SEL_SQUARE "\e[48;2;120;120;255m"
#define WARN_SQUARE "\e[48;2;255;120;120m"
#define CURSOR_SQUARE "\e[48;2;255;120;255m"
#define PREV_MOVE_FROM_SQUARE "\e[48;2;120;120;160m"
#define PREV_MOVE_TO_SQUARE "\e[48;2;90;90;255m"
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

	char piece_to_char(enum Pieces piece)
	{
		switch (piece)
		{
			case Pieces::WHITE_KING:
				return 'K';
			case Pieces::WHITE_QUEEN:
				return 'Q';
			case Pieces::WHITE_ROOK:
				return 'R';
			case Pieces::WHITE_KNIGHT:
				return 'N';
			case Pieces::WHITE_BISHOP:
				return 'B';
			case Pieces::WHITE_PAWN:
				return 'P';

			case Pieces::BLACK_KING:
				return 'k';
			case Pieces::BLACK_QUEEN:
				return 'q';
			case Pieces::BLACK_ROOK:
				return 'r';
			case Pieces::BLACK_KNIGHT:
				return 'n';
			case Pieces::BLACK_BISHOP:
				return 'b';
			case Pieces::BLACK_PAWN:
				return 'p';

			case Pieces::UNOCCUPIED:
				return ' ';

			default:
				return '?';
		};
	}

	enum Pieces char_to_piece(char piece)
	{
		switch (piece)
		{
			case 'K':
				return Pieces::WHITE_KING;
			case 'Q':
				return Pieces::WHITE_QUEEN;
			case 'R':
				return Pieces::WHITE_ROOK;
			case 'N':
				return Pieces::WHITE_KNIGHT;
			case 'B':
				return Pieces::WHITE_BISHOP;
			case 'P':
				return Pieces::WHITE_PAWN;

			case 'k':
				return Pieces::BLACK_KING;
			case 'q':
				return Pieces::BLACK_QUEEN;
			case 'r':
				return Pieces::BLACK_ROOK;
			case 'n':
				return Pieces::BLACK_KNIGHT;
			case 'b':
				return Pieces::BLACK_BISHOP;
			case 'p':
				return Pieces::BLACK_PAWN;

			case ' ':
				return Pieces::UNOCCUPIED;

			default:
				return Pieces::UNOCCUPIED;
		}
	}

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

			Board() : turn(Players::WHITE), white_king(-1, -1), black_king(-1, -1) {}

			void initialise_blank()
			{
				for (uint8_t row = 0; row < 8; row++)
				{
					for (uint8_t col = 0; col < 8; col++)
					{
						squares[row][col] = Pieces::UNOCCUPIED;
					}
				}
			}

			void initialise_standard()
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
				Square prev_move_from = { -1, -1 },
				Square prev_move_to = { -1, -1 },
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

						// Prev move squares

						if (prev_move_from.x == col && prev_move_from.y == row - 1)
						{
							printf(PREV_MOVE_FROM_SQUARE);
						}

						if (prev_move_to.x == col && prev_move_to.y == row - 1)
						{
							printf(PREV_MOVE_TO_SQUARE);
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

			void print_upsd(Square cursor, Square sel = { 8, 8 },
				Square prev_move_from = { -1, -1 },
				Square prev_move_to = { -1, -1 },
				std::vector<Square> highl = std::vector<Square>(),
				std::vector<Square> warn = std::vector<Square>())
			{
				for (uint8_t row = 1; row <= 8; row++)
				{
					printf("%hhu  ", row);

					for (int8_t col = 7; col >= 0; col--)
					{
						if (row % 2 == 0 && col % 2 == 0 || row % 2 && col % 2)
						{
							printf(WHITE_SQUARE);
						}
						else
						{
							printf(BLACK_SQUARE);
						}

						// Prev move squares

						if (prev_move_from.x == col && prev_move_from.y == row - 1)
						{
							printf(PREV_MOVE_FROM_SQUARE);
						}

						if (prev_move_to.x == col && prev_move_to.y == row - 1)
						{
							printf(PREV_MOVE_TO_SQUARE);
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

				for (uint8_t i = 8; i != 0; i--)
				{
					printf("%c ", 'A' + i - 1);
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
							std::vector<Square> moves = possible_moves(col, row, true);
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
							std::vector<Square> moves = possible_moves(col, row, true);
							if (moves.size()) return true;
						}
					}
				}

				return false;
			}

			std::vector<Square> possible_moves(uint8_t x, uint8_t y,
				bool check_check)
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
							if (!check_check || is_legal(x, y, x, y + 1))
							{
								moves.push_back(Square(x, y + 1));
							}

							if (y == 1 && squares[y + 2][x] == Pieces::UNOCCUPIED)
							{
								if (!check_check || is_legal(x, y, x, y + 2))
								{
									moves.push_back(Square(x, y + 2));
								}
							}
						}

						if (is_black(squares[y + 1][x - 1]))
						{
							if (!check_check || is_legal(x, y, x - 1, y + 1))
							{
								moves.push_back(Square(x - 1, y + 1));
							}
						}

						if (is_black(squares[y + 1][x + 1]))
						{
							if (!check_check || is_legal(x, y, x + 1, y + 1))
							{
								moves.push_back(Square(x + 1, y + 1));
							}
						}

						// En passant

						if (squares[y][x - 1] == Pieces::BLACK_PAWN
							&& black_en_passant_flags & (x - 1))
						{
							if (!check_check || is_legal(x, y, x - 1, y + 1))
							{
								moves.push_back(Square(x - 1, y + 1));
							}
						}

						if (squares[y][x + 1] == Pieces::BLACK_PAWN
							&& black_en_passant_flags & (x + 1))
						{
							if (!check_check || is_legal(x, y, x + 1, y + 1))
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
							if (!check_check || is_legal(x, y, x, y - 1))
							{
								moves.push_back(Square(x, y - 1));
							}

							if (y == 6 && squares[y - 2][x] == Pieces::UNOCCUPIED)
							{
								if (!check_check || is_legal(x, y, x, y - 2))
								{
									moves.push_back(Square(x, y - 2));
								}
							}
						}

						if (is_white(squares[y - 1][x - 1]))
						{
							if (!check_check || is_legal(x, y, x - 1, y - 1))
							{
								moves.push_back(Square(x - 1, y - 1));
							}
						}

						if (is_white(squares[y - 1][x + 1]))
						{
							if (!check_check || is_legal(x, y, x + 1, y - 1))
							{
								moves.push_back(Square(x + 1, y - 1));
							}
						}

						// En passant

						if (squares[y][x - 1] == Pieces::WHITE_PAWN
							&& white_en_passant_flags & (x - 1))
						{
							if (!check_check || is_legal(x, y, x - 1, y - 1))
							{
								moves.push_back(Square(x - 1, y - 1));
							}
						}

						if (squares[y][x + 1] == Pieces::WHITE_PAWN
							&& white_en_passant_flags & (x + 1))
						{
							if (!check_check || is_legal(x, y, x + 1, y - 1))
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
							if (!check_check || is_legal(x, y, diag.x, diag.y))
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
							if (!check_check || is_legal(x, y, diag.x, diag.y))
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
							if (!check_check || is_legal(x, y, diag.x, diag.y))
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
							if (!check_check || is_legal(x, y, diag.x, diag.y))
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
							if (!check_check || is_legal(x, y, diag.x, diag.y))
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
							if (!check_check || is_legal(x, y, diag.x, diag.y))
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
							if (!check_check || is_legal(x, y, diag.x, diag.y))
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
							if (!check_check || is_legal(x, y, diag.x, diag.y))
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
							if (!check_check || is_legal(x, y, x - 1, y - 2))
							{
								moves.push_back(Square(x - 1, y - 2));
							}
						}

						if (x - 2 >= 0 && y - 1 >= 0 && !is_white(squares[y - 1][x - 2]))
						{
							if (!check_check || is_legal(x, y, x - 2, y - 1))
							{
								moves.push_back(Square(x - 2, y - 1));
							}
						}

						if (x + 1 < 8 && y - 2 >= 0 && !is_white(squares[y - 2][x + 1]))
						{
							if (!check_check || is_legal(x, y, x + 1, y - 2))
							{
								moves.push_back(Square(x + 1, y - 2));
							}
						}

						if (x + 2 < 8 && y - 1 >= 0 && !is_white(squares[y - 1][x + 2]))
						{
							if (!check_check || is_legal(x, y, x + 2, y - 1))
							{
								moves.push_back(Square(x + 2, y - 1));
							}
						}

						if (x - 1 >= 0 && y + 2 < 8 && !is_white(squares[y + 2][x - 1]))
						{
							if (!check_check || is_legal(x, y, x - 1, y + 2))
							{
								moves.push_back(Square(x - 1, y + 2));
							}
						}

						if (x - 2 >= 0 && y + 1 < 8 && !is_white(squares[y + 1][x - 2]))
						{
							if (!check_check || is_legal(x, y, x - 2, y + 1))
							{
								moves.push_back(Square(x - 2, y + 1));
							}
						}

						if (x + 1 < 8 && y + 2 < 8 && !is_white(squares[y + 2][x + 1]))
						{
							if (!check_check || is_legal(x, y, x + 1, y + 2))
							{
								moves.push_back(Square(x + 1, y + 2));
							}
						}

						if (x + 2 < 8 && y + 1 < 8 && !is_white(squares[y + 1][x + 2]))
						{
							if (!check_check || is_legal(x, y, x + 2, y + 1))
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
							if (!check_check || is_legal(x, y, x - 1, y - 2))
							{
								moves.push_back(Square(x - 1, y - 2));
							}
						}

						if (x - 2 >= 0 && y - 1 >= 0 && !is_black(squares[y - 1][x - 2]))
						{
							if (!check_check || is_legal(x, y, x - 2, y - 1))
							{
								moves.push_back(Square(x - 2, y - 1));
							}
						}

						if (x + 1 < 8 && y - 2 >= 0 && !is_black(squares[y - 2][x + 1]))
						{
							if (!check_check || is_legal(x, y, x + 1, y - 2))
							{
								moves.push_back(Square(x + 1, y - 2));
							}
						}

						if (x + 2 < 8 && y - 1 >= 0 && !is_black(squares[y - 1][x + 2]))
						{
							if (!check_check || is_legal(x, y, x + 2, y - 1))
							{
								moves.push_back(Square(x + 2, y - 1));
							}
						}

						if (x - 1 >= 0 && y + 2 < 8 && !is_black(squares[y + 2][x - 1]))
						{
							if (!check_check || is_legal(x, y, x - 1, y + 2))
							{
								moves.push_back(Square(x - 1, y + 2));
							}
						}

						if (x - 2 >= 0 && y + 1 < 8 && !is_black(squares[y + 1][x - 2]))
						{
							if (!check_check || is_legal(x, y, x - 2, y + 1))
							{
								moves.push_back(Square(x - 2, y + 1));
							}
						}

						if (x + 1 < 8 && y + 2 < 8 && !is_black(squares[y + 2][x + 1]))
						{
							if (!check_check || is_legal(x, y, x + 1, y + 2))
							{
								moves.push_back(Square(x + 1, y + 2));
							}
						}

						if (x + 2 < 8 && y + 1 < 8 && !is_black(squares[y + 1][x + 2]))
						{
							if (!check_check || is_legal(x, y, x + 2, y + 1))
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
							if (!check_check || is_legal(x, y, line.x, line.y))
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
							if (!check_check || is_legal(x, y, line.x, line.y))
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
							if (!check_check || is_legal(x, y, line.x, line.y))
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
							if (!check_check || is_legal(x, y, line.x, line.y))
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
							if (!check_check || is_legal(x, y, line.x, line.y))
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
							if (!check_check || is_legal(x, y, line.x, line.y))
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
							if (!check_check || is_legal(x, y, line.x, line.y))
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
							if (!check_check || is_legal(x, y, line.x, line.y))
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
							if (!check_check || is_legal(x, y, diag.x, diag.y))
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
							if (!check_check || is_legal(x, y, diag.x, diag.y))
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
							if (!check_check || is_legal(x, y, diag.x, diag.y))
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
							if (!check_check || is_legal(x, y, diag.x, diag.y))
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
							if (!check_check || is_legal(x, y, line.x, line.y))
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
							if (!check_check || is_legal(x, y, line.x, line.y))
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
							if (!check_check || is_legal(x, y, line.x, line.y))
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
							if (!check_check || is_legal(x, y, line.x, line.y))
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
							if (!check_check || is_legal(x, y, diag.x, diag.y))
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
							if (!check_check || is_legal(x, y, diag.x, diag.y))
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
							if (!check_check || is_legal(x, y, diag.x, diag.y))
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
							if (!check_check || is_legal(x, y, diag.x, diag.y))
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
							if (!check_check || is_legal(x, y, line.x, line.y))
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
							if (!check_check || is_legal(x, y, line.x, line.y))
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
							if (!check_check || is_legal(x, y, line.x, line.y))
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
							if (!check_check || is_legal(x, y, line.x, line.y))
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
							if (!check_check || is_legal(x, y, diag.x, diag.y))
							{
								moves.push_back(diag);
							}
						}

						diag = Square(x - 1, y + 1);

						if (diag.x >= 0 && diag.y < 8
							&& !is_white(squares[diag.y][diag.x]))
						{
							if (!check_check || is_legal(x, y, diag.x, diag.y))
							{
								moves.push_back(diag);
							}
						}

						diag = Square(x - 1, y - 1);

						if (diag.x >= 0 && diag.y >= 0
							&& !is_white(squares[diag.y][diag.x]))
						{
							if (!check_check || is_legal(x, y, diag.x, diag.y))
							{
								moves.push_back(diag);
							}
						}

						diag = Square(x + 1, y - 1);

						if (diag.x < 8 && diag.y >= 0
							&& !is_white(squares[diag.y][diag.x]))
						{
							if (!check_check || is_legal(x, y, diag.x, diag.y))
							{
								moves.push_back(diag);
							}
						}

						Square line(x + 1, y);

						if (line.x < 8 && !is_white(squares[line.y][line.x]))
						{
							if (!check_check || is_legal(x, y, line.x, line.y))
							{
								moves.push_back(line);
							}
						}

						line = Square(x - 1, y);

						if (line.x >= 0 && !is_white(squares[line.y][line.x]))
						{
							if (!check_check || is_legal(x, y, line.x, line.y))
							{
								moves.push_back(line);
							}
						}

						line = Square(x, y + 1);

						if (line.y < 8 && !is_white(squares[line.y][line.x]))
						{
							if (!check_check || is_legal(x, y, line.x, line.y))
							{
								moves.push_back(line);
							}
						}

						line = Square(x, y - 1);

						if (line.y >= 0 && !is_white(squares[line.y][line.x]))
						{
							if (!check_check || is_legal(x, y, line.x, line.y))
							{
								moves.push_back(line);
							}
						}

						// Castling

						if (!WHITE_KING_MOVED && !WHITE_RIGHT_ROOK_MOVED
							&& squares[y][x + 1] == Pieces::UNOCCUPIED
							&& squares[y][x + 2] == Pieces::UNOCCUPIED
							&& check_check
							&& !white_in_check()
							&& is_legal(x, y, x + 1, y)
							&& is_legal(x, y, x + 2, y))
						{
							moves.push_back(Square(x + 2, y));
						}

						if (!WHITE_KING_MOVED && !WHITE_LEFT_ROOK_MOVED
							&& squares[y][x - 1] == Pieces::UNOCCUPIED
							&& squares[y][x - 2] == Pieces::UNOCCUPIED
							&& squares[y][x - 3] == Pieces::UNOCCUPIED
							&& check_check
							&& !white_in_check()
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
							if (!check_check || is_legal(x, y, diag.x, diag.y))
							{
								moves.push_back(diag);
							}
						}

						diag = Square(x - 1, y + 1);

						if (diag.x >= 0 && diag.y < 8
							&& !is_black(squares[diag.y][diag.x]))
						{
							if (!check_check || is_legal(x, y, diag.x, diag.y))
							{
								moves.push_back(diag);
							}
						}

						diag = Square(x - 1, y - 1);

						if (diag.x >= 0 && diag.y >= 0
							&& !is_black(squares[diag.y][diag.x]))
						{
							if (!check_check || is_legal(x, y, diag.x, diag.y))
							{
								moves.push_back(diag);
							}
						}

						diag = Square(x + 1, y - 1);

						if (diag.x < 8 && diag.y >= 0
							&& !is_black(squares[diag.y][diag.x]))
						{
							if (!check_check || is_legal(x, y, diag.x, diag.y))
							{
								moves.push_back(diag);
							}
						}

						Square line(x + 1, y);

						if (line.x < 8 && !is_black(squares[line.y][line.x]))
						{
							if (!check_check || is_legal(x, y, line.x, line.y))
							{
								moves.push_back(line);
							}
						}

						line = Square(x - 1, y);

						if (line.x >= 0 && !is_black(squares[line.y][line.x]))
						{
							if (!check_check || is_legal(x, y, line.x, line.y))
							{
								moves.push_back(line);
							}
						}

						line = Square(x, y + 1);

						if (line.y < 8 && !is_black(squares[line.y][line.x]))
						{
							if (!check_check || is_legal(x, y, line.x, line.y))
							{
								moves.push_back(line);
							}
						}

						line = Square(x, y - 1);

						if (line.y >= 0 && !is_black(squares[line.y][line.x]))
						{
							if (!check_check || is_legal(x, y, line.x, line.y))
							{
								moves.push_back(line);
							}
						}

						// Castling

						if (!BLACK_KING_MOVED && !BLACK_RIGHT_ROOK_MOVED
							&& squares[y][x + 1] == Pieces::UNOCCUPIED
							&& squares[y][x + 2] == Pieces::UNOCCUPIED
							&& check_check
							&& !black_in_check()
							&& is_legal(x, y, x + 1, y)
							&& is_legal(x, y, x + 2, y))
						{
							moves.push_back(Square(x + 2, y));
						}

						if (!BLACK_KING_MOVED && !BLACK_LEFT_ROOK_MOVED
							&& squares[y][x - 1] == Pieces::UNOCCUPIED
							&& squares[y][x - 2] == Pieces::UNOCCUPIED
							&& squares[y][x - 3] == Pieces::UNOCCUPIED
							&& check_check
							&& !black_in_check()
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

			bool is_legal_move(enum Players player, uint8_t x_from,
				uint8_t y_from, uint8_t x_to, uint8_t y_to)
			{
				Square from(x_from, y_from);
				Square to(x_to, y_to);

				enum Pieces moved_piece = squares[y_from][x_from];

				if (player == Players::WHITE && !is_white(moved_piece)
					|| player == Players::BLACK && !is_black(moved_piece))
				{
					return false;
				}

				std::vector<Square> moves = possible_moves(x_from, y_from, true);

				for (size_t i = 0; i < moves.size(); i++)
				{
					if (to.x == moves[i].x && to.y == moves[i].y)
					{
						if (is_legal(x_from, y_from, x_to, y_to))
						{
							return true;
						}
						else
						{
							return false;
						}
					}
				}

				return false;
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
					SET_WHITE_KING_MOVED;
				}

				if (!WHITE_RIGHT_ROOK_MOVED && moved_piece == Pieces::WHITE_ROOK
					&& from.x == 7 || squares[0][7] != Pieces::WHITE_ROOK)
				{
					SET_WHITE_RIGHT_ROOK_MOVED;
				}

				if (!WHITE_LEFT_ROOK_MOVED && moved_piece == Pieces::WHITE_ROOK
					&& from.x == 0 || squares[0][0] != Pieces::WHITE_ROOK)
				{
					SET_WHITE_LEFT_ROOK_MOVED;
				}

				if (moved_piece == Pieces::BLACK_KING)
				{
					SET_BLACK_KING_MOVED;
				}

				if (!BLACK_RIGHT_ROOK_MOVED && moved_piece == Pieces::BLACK_ROOK
					&& from.x == 7 || squares[7][7] != Pieces::BLACK_ROOK)
				{
					SET_BLACK_RIGHT_ROOK_MOVED;
				}

				if (!BLACK_LEFT_ROOK_MOVED && moved_piece == Pieces::BLACK_ROOK
					&& from.x == 0 || squares[7][0] != Pieces::BLACK_ROOK)
				{
					SET_BLACK_LEFT_ROOK_MOVED;
				}

				// En passant

				if (turn == Players::WHITE) white_en_passant_flags = 0;
				else black_en_passant_flags = 0;

				if (moved_piece == Pieces::WHITE_PAWN && to.y - from.y == 2)
				{
					white_en_passant_flags |= from.x;
				}

				if (moved_piece == Pieces::BLACK_PAWN && from.y - to.y == 2)
				{
					black_en_passant_flags |= from.x;
				}

				if (moved_piece == Pieces::WHITE_PAWN &&
					black_en_passant_flags & to.x && to.x != from.x)
				{
					squares[from.y][to.x] = Pieces::UNOCCUPIED;
				}

				if (moved_piece == Pieces::BLACK_PAWN &&
					white_en_passant_flags & to.x && to.x != from.x)
				{
					squares[from.y][to.x] = Pieces::UNOCCUPIED;
				}

				// Pawn promotion

				if (moved_piece == Pieces::WHITE_PAWN && to.y == 7)
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
							squares[to.y][to.x] = Pieces::WHITE_BISHOP;
							break;

						case '2':
							squares[to.y][to.x] = Pieces::WHITE_KNIGHT;
							break;

						case '3':
							squares[to.y][to.x] = Pieces::WHITE_ROOK;
							break;

						case '4':
							squares[to.y][to.x] = Pieces::WHITE_QUEEN;
							break;

						default:
							goto try_again;
					}
				}

				if (moved_piece == Pieces::BLACK_PAWN && to.y == 0)
				{
					printf("select promotion:\n");
					printf("1: " BISHOP ", ");
					printf("2: " KNIGHT ", ");
					printf("3: " ROOK ", ");
					printf("4: " QUEEN "\n");
					try_again_1:
					printf("> ");

					char choice;
					std::cin >> choice;

					switch (choice)
					{
						case '1':
							squares[to.y][to.x] = Pieces::BLACK_BISHOP;
							break;

						case '2':
							squares[to.y][to.x] = Pieces::BLACK_KNIGHT;
							break;

						case '3':
							squares[to.y][to.x] = Pieces::BLACK_ROOK;
							break;

						case '4':
							squares[to.y][to.x] = Pieces::BLACK_QUEEN;
							break;

						default:
							goto try_again_1;
					}
				}

				if (turn == Players::WHITE) turn = Players::BLACK;
				else turn = Players::WHITE;
			}

			std::string to_str() const
			{
				std::string str;

				if (turn == Players::WHITE) str += 'W';
				else str += 'B';

				for (size_t y = 0; y < 8; y++)
				{
					for (size_t x = 0; x < 8; x++)
					{
						enum Pieces piece = squares[y][x];
						str += piece_to_char(piece);
					}
				}

				return str;
			}
	};
};

#endif