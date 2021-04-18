#ifndef KEYPRESS_HEADER
#define KEYPRESS_HEADER

#include <bits/stdc++.h>
#include <termios.h>
#include <unistd.h>
#include "debug.hpp"

// Todo: port this to mac os and windows

namespace keypress
{
	int getch()
	{
		struct termios oldattr;
		struct termios newattr;
		int ch;

		tcgetattr(STDIN_FILENO, &oldattr);
		newattr = oldattr;
		newattr.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &newattr);

		ch = getchar();
		tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);

		// debug("getch() -> %d", ch);

		return ch;
	}

	enum SpecialKeys : uint8_t
	{
		TAB = 9,
		ENTER = 10,
		DELETE = 21,
		SPACE = 32,
		BACKSPACE = 127,
		ARROW_UP = 128,
		ARROW_DOWN,
		ARROW_RIGHT,
		ARROW_LEFT,
		UNKNOWN = 255
	};

	uint8_t get_key()
	{
		int first = getch();
		if (first != 27) return first;

		// Escape sequence

		int second = getch();
		if (second != '[') return UNKNOWN;

		// CSI

		int third = getch();

		switch (third)
		{
			case 'A': return ARROW_UP;
			case 'B': return ARROW_DOWN;
			case 'C': return ARROW_RIGHT;
			case 'D': return ARROW_LEFT;
			default: return UNKNOWN;
		}
	}
};

#endif