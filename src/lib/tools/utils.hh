/*\
* lenactions
* A simple image editing library
* Copyrigth 2013 - Hadrien Croubois
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
\*/

#ifndef UTILS_HH
#define UTILS_HH

#include <iostream>
#include <fstream>

namespace lenactions {

	char getc(std::istream& in);
	unsigned char getbit(std::istream& in);
	unsigned char getrawbyte(std::istream& in);
	int getint(std::istream& in);

  void error(const char* str);
  
};

#endif // UTILS_H
