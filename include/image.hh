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

#ifndef IMAGE_HH
#define IMAGE_HH

#include "utils.hh"
#include "pixel.hh"
#include "convolution.hh"
#include "unionfind.hh"

#include "structure/priority.hh"

namespace lenactions {
  
  class image {

		
    public:
      image();
			image(const image& cpy);
      image(std::istream& in);
      ~image();		
		
			pixel& operator[] (int);
			
			void from_stream(std::istream& in);
      void to_stream(std::ostream& out, format out_format);
		
			std::pair<int,int> dimensions();
		
			
      // void histogram(std::ostream& out);
      // void inv();
      // void rotate(vByte angle);
	    // void gamma(vByte y);
      // void normalize();
      // void smooth();
    
      // void contour_prewitt();
      // void contour_sobel();
      // void contour_kirsch();
          
      // void connected(int adj = 0);
      
			
			float Hseuil(float p);
			
      image compose(convolution c);
			static image assemblage(image& a, image& b, pixelOperator op);
			
			image seuil_global(float s = 0.5);
			image seuil_local();
			image seuil_histerisis(float high, float low);
			
			image affinage();
			
			image closedContours(float sHigh					= 0.95,
													 float sLow						= 0.85,
													 convolution hFilter	= lenactions::convolution::SobelH(),
													 convolution vFilter	= lenactions::convolution::SobelV());
			
			
    private:
      int o_cols(int j, int offset);
      int o_rows(int i, int offset);

		
      int rows;
      int cols;
      pixel* bitmap;	
  };
};
#endif // IMAGE_H
