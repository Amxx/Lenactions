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

#ifndef IMAGE_H
#define IMAGE_H

#include "utils.hh"
#include "pixel.hh"
#include "convolution.hh"

namespace ImAMXX {
  
  class image {
    
    public:
      image();
      image(image& cpy);
      image(std::istream& in);
      ~image();
      
      std::pair<int,int> dimensions();
		
      void from_stream(std::istream& in);
      void to_stream(std::ostream& out, format out_format);
      
      void histogram(std::ostream& out);
      
      void inv();
      void rotate(vByte angle);
	    void gamma(vByte y);
      void normalize();
      void smooth();
    
    
      void contour_prewitt();
      void contour_sobel();
      void contour_kirsch();
      
    
      void connected(int adj = 0);
      
	    //void hide(image img);
      //void reveal();
      
    private:
      int o_cols(int j, int offset);
      int o_rows(int i, int offset);
      pixel* compose(convolution c);

      int rows;
      int cols;
      int scale;
      pixel* bitmap;	
  };
};
#endif // IMAGE_H
