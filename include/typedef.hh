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

#ifndef TYPEDEF_HH
#define TYPEDEF_HH

#define		PI							3.1415926535897932384626

#define		MIN3(x,y,z)  		((y) <= (z) ? ((x) <= (y) ? (x) : (y)) : ((x) <= (z) ? (x) : (z)))
#define		MAX3(x,y,z)  		((y) >= (z) ? ((x) >= (y) ? (x) : (y)) : ((x) >= (z) ? (x) : (z)))

namespace lenactions {

  enum canal
  {
    R = 0,
    G = 1,
    B = 2,
    H = 3,
    S = 4,
    V = 5,
    GREY = 6
  };
	enum colorSpace
	{
		RGB,
		HSV
	};
	enum format
	{
		P1 = 1,
		P2 = 2,
		P3 = 3,
		P4 = 4,
		P5 = 5,
		P6 = 6
	};

};


#endif //TYPEDEF_H
