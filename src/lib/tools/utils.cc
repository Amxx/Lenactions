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

#include "utils.hh"

char lenactions::getc(std::istream& in)
{
	int ch;	
	if ((ch = in.get()) == -1) error("EOF / read error");
	
	if (ch == '#') {
		do if ((ch = in.get()) == -1) error("EOF / read error");
		while (ch != '\n' && ch != '\r');
	}
	return (char) ch;
}

unsigned char lenactions::getbit(std::istream& in)
{
	char ch;
	do ch = getc(in);
	while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r');
	if (ch != '0' && ch != '1') error("junk in file where bits should be");
	return (ch == '1')?1:0;
}

unsigned char lenactions::getrawbyte(std::istream& in)
{
	int ch;	
	if ((ch = in.get()) == -1) error("EOF / read error");
	return (unsigned char) ch;
}

int lenactions::getint(std::istream& in)
{
	char ch;
	do ch = getc(in);
	while (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n');
	if (ch < '0' || ch > '9') error("junk in file where an integer should be");
	int i = 0;
	do {
		i = i * 10 + ch - '0';
		ch = getc(in);
	} while (ch >= '0' && ch <= '9');
	return i;
}

void lenactions::error(const char* str)
{
	std::cerr << std::endl
						<< str
						<< std::endl
						<< std::endl;
	throw;
}
