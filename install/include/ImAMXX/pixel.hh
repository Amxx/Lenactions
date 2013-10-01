/*\
* ImAMXX
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

#ifndef PIXEL_H
#define PIXEL_H

#include <iostream>
#include <cmath>

#include "typedef.hh"

namespace ImAMXX {

	class pixel {
		public:
			pixel();
			pixel(const pixel& cpy);
			pixel(vByte g);
			pixel(vByte x, vByte y, vByte z, colorSpace s = RGB);
    
      vByte         get_canal(canal c);
      void          set_canal(canal c, vByte v);
      void          set_space(colorSpace s);
    
      void          inv();
      void          rotate(vByte a);
    
    	void          to_stream(std::ostream& o, format f, int s);

      // pixel& operator += ( pixel& );
      // pixel operator + ( pixel& P );
      // pixel& operator /= ( const bit& );
      // pixel operator / ( const bit& B);

    private:
      colorSpace    space;
      vByte         value[3];    
    
	};
  
};

#endif //PIXEL_H
