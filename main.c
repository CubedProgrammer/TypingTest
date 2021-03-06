// /CubedProgrammer/TypingTest, a simple game for checking your typing speed
// Copyright (C) 2022 github.com/CubedProgrammer, owner of said account

// This file is part of /CubedProgrammer/TypingTest
// /CubedProgrammer/TypingTest is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// /CubedProgrammer/TypingTest is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with /CubedProgrammer/TypingTest. If not, see <https://www.gnu.org/licenses/>.

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#ifdef _WIN32
#include<conio.h>
#include<windows.h>
#define mssleep Sleep
#define gch getch()
#else
#include<sys/select.h>
#include<termios.h>
#include<unistd.h>
#define mssleep(ms)usleep((ms) * 1000)
#define gch getchar()
#endif
#include"load.h"
#define SLOPE 25214903917;
#define YINT 11

size_t nextrand(size_t seed);
time_t getms(void);

int main(int argl, char *argv[])
{
	setvbuf(stdout, NULL, _IONBF, 0);
	char clearln[62];
	memset(clearln, ' ', 60);
	clearln[60] = '\r';
	clearln[61] = '\0';
	char sequential = 0;
	time_t timer = 60000;
	const char *dictfile = NULL;
	for(int i = 1; i < argl; ++i)
	{
		if('-' == argv[i][0])
		{
			for(const char *it = argv[i] + 1; *it != '\0'; ++it)
			{
				switch(*it)
				{
					case'd':
						dictfile = argv[++i];
						break;
					case's':
						sequential = 1;
						break;
					case't':
						if(i + 1 < argl)
							timer = atoi(argv[++i]) * 1000;
						break;
				}
			}
		}
	}
	int succ = dictfile == NULL ? findload() : loaddict(dictfile, &cptt____dict, &cptt____dictlen);
	if(succ == 0)
	{
#ifdef _WIN32
		HANDLE ho = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD cm;
		GetConsoleMode(ho, &cm);
		cm |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		SetConsoleMode(ho, cm);
#else
		struct termios old, curr;
		tcgetattr(STDIN_FILENO, &old);
		memcpy(&curr, &old, sizeof(struct termios));
		curr.c_lflag &= ~(ECHO | ICANON);
		tcsetattr(STDIN_FILENO, TCSANOW, &curr);
		struct timeval onesec;
		fd_set rset, *rsetp = &rset;
#endif
		fputs("Starting in 5", stdout);
		mssleep(997);
		for(int i = '4'; i >= '0'; --i)
		{
			putchar('\b');
			putchar(i);
			mssleep(997);
		}
		putchar('\n');
		char **dict = cptt____dict;
		size_t dlen = cptt____dictlen;
		size_t indices[8];
		size_t dice = time(NULL) ^ SLOPE;
		if(sequential)
		{
			for(int i = 0; i < 8; ++i)
				indices[i] = i;
		}
		else
		{
			for(int i = 0; i < 8; ++i)
				dice = nextrand(dice), indices[i] = dice % dlen;
		}
		const char *currstr;
		const char *ita, *itb;
		time_t begin = getms(), rn = begin + 1;
		int secleft, ready;
		char typed[60] = "";
		size_t chtyped = 0, completed = 0;
		size_t tottyped = 0, accurate = 0;
		char ch;
		for(;rn - begin < timer; rn = getms())
		{
			currstr = dict[indices[0]];
			secleft = begin + timer - rn;
			secleft /= 1000;
			printf("%i:%02i %zu\n%s", secleft / 60, secleft % 60, completed, currstr);
			if(sequential)
			{
				for(int i = 1; i < 8; ++i)
				{
					if(indices[i] < dlen)
						printf(" %s", dict[indices[i]]);
				}
			}
			else
			{
				for(int i = 1; i < 8; ++i)
					printf(" %s", dict[indices[i]]);
			}
			putchar('\n');
			fputs(clearln, stdout);
			fputs("\033\13332m", stdout);
			for(ita = typed, itb = currstr; *itb != '\0' && *ita == *itb; ++ita, ++itb)
				putchar(*ita);
			fputs("\033\13331m", stdout);
			for(; *ita != '\0'; ++ita)
				putchar(*ita);
			fputs("\033\133m", stdout);
#ifndef _WIN32
			FD_ZERO(rsetp);
			FD_SET(STDIN_FILENO, rsetp);
			onesec.tv_sec = 0;
			onesec.tv_usec = 999999;
			ready = select(STDIN_FILENO + 1, rsetp, NULL, NULL, &onesec);
			if(ready)
			{
#endif
				ch = gch;
				switch(ch)
				{
#ifdef _WIN32
					case -040:
#else
					case 033:
#endif
						putchar('\a');
						break;
#ifdef _WIN32
					case'\b':
#else
					case 0177:
#endif
						if(chtyped > 0)
						{
							--chtyped;
							typed[chtyped] = '\0';
						}
						else
							putchar('\a');
						break;
					case' ':
						if(strcmp(typed, currstr) == 0)
						{
							memset(typed, 0, sizeof typed);
							chtyped = 0;
							for(int i = 0; i < 7; ++i)
								indices[i] = indices[i + 1];
							if(sequential)
								indices[7] = indices[6] + 1;
							else
							{
								dice = nextrand(dice);
								indices[7] = dice % dlen;
							}
							++completed;
							if(sequential && completed == dlen)
								timer = getms() - begin;
							break;
						}
					default:
						typed[chtyped] = ch;
						++tottyped;
						if(chtyped < strlen(currstr))
						{
							if(ch == currstr[chtyped])
								++accurate;
						}
						++chtyped;
				}
#ifndef _WIN32
			}
#endif
			fputs("\033\133F", stdout);
			fputs(clearln, stdout);
			fputs("\033\133F", stdout);
		}
		rn -= begin;
		double score = completed * 60000;
		score /= rn;
		printf("%zu characters typed, %zu correct.\n%.3f words/min\n", tottyped, accurate, score);
#ifndef _WIN32
		tcsetattr(STDIN_FILENO, TCSANOW, &old);
#endif
	}
	else
		fputs("Could not load file containing the words.\nCheck if the file ttdict.txt exists in the working directory.\nAlso make sure that you have permission to read it.\n", stderr);
	return succ;
}

size_t nextrand(size_t seed)
{
	seed *= SLOPE;
	seed += YINT;
	seed &= 0xffffffffffff;
	return seed;
}

time_t getms(void)
{
	time_t t;
#if __STDC_VERSION__ >= 201112L
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	t = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#else
#ifdef _WIN32
#error "Please use C11 or higher on windows."
#else
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	t = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#endif
#endif
	return t;
}
