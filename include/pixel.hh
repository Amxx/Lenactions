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

#ifndef PIXEL_HH
#define PIXEL_HH

#include <iostream>
#include <cmath>

#include "typedef.hh"

namespace lenactions {

	class pixel {
		public:
			pixel();
			pixel(const pixel& cpy);
			
      pixel(float g);
			pixel(float x, float y, float z, colorSpace s = RGB);
    
      float&       	get_canal(canal c);
      float&				r();
			float&				g();
			float&				b();
			float&				h();
			float&				s();
			float&				v();
			void          set_canal(canal c, float v);
      void          set_space(colorSpace s);
    
    	void          to_stream(std::ostream& o, format f);
			
		
			static pixel	quadratic(pixel& pi, pixel& pj);
			static pixel	angleteinte(pixel&pi, pixel& pj);
			
    private:
      colorSpace    space;
      float         value[3];    
    
	};

	typedef pixel(*pixelOperator)(pixel&, pixel&);
};



#endif //PIXEL_H
