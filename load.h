// /CubedProgrammer/TypingTest, a simple game for checking your typing speed
// Copyright (C) 2022 github.com/CubedProgrammer, owner of said account

// This file is part of /CubedProgrammer/TypingTest
// /CubedProgrammer/TypingTest is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// /CubedProgrammer/TypingTest is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with /CubedProgrammer/TypingTest. If not, see <https://www.gnu.org/licenses/>.

#ifndef LOAD_H_
#define LOAD_H_
#include<stddef.h>
extern char **cptt____dict;
extern size_t cptt____dictlen;
int findload(void);
int loaddict(const char *fname, char ***dictp, size_t *len);
#endif
