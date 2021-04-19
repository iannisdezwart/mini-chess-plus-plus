#ifndef KEYPRESS_HEADER
#define KEYPRESS_HEADER

#include <bits/stdc++.h>
#include <termios.h>
#include <unistd.h>
#include "debug.hpp"

// Todo: port this to mac os and windows

namespace keypress
{
	struct termios saved_termios;

	void start_getch_mode()
	{
		struct termios new_termios;

		tcgetattr(STDIN_FILENO, &saved_termios);

		new_termios = saved_termios;
		new_termios.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
	}

	void stop_getch_mode()
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &saved_termios);
	}

	int kbhit()
	{
		struct timeval tv = { 0, 0 };
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(STDIN_FILENO, &fds);
		return select(1, &fds, NULL, NULL, &tv);
	}

	int getch()
	{
		int read_bytes;
		char ch;

		if ((read_bytes = read(STDIN_FILENO, &ch, 1)) < 0)
		{
			return -1;
		}
		else
		{
			return ch;
		}
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

	uint8_t get_key(std::function<bool()> callback)
	{
		while (!kbhit())
		{
			if (callback() == false)
			{
				return SpecialKeys::UNKNOWN;
			}

			usleep(10);
		}

		int first = getch();
		if (first != 27) return first;

		// Escape sequence

		while (!kbhit())
		{
			if (callback() == false)
			{
				return SpecialKeys::UNKNOWN;
			}

			usleep(10);
		}

		int second = getch();
		if (second != '[') return UNKNOWN;

		// CSI

		while (!kbhit())
		{
			if (callback() == false)
			{
				return SpecialKeys::UNKNOWN;
			}

			usleep(10);
		}

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