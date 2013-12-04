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

#include "pixel.hh"
#include "convolution.hh"

namespace lenactions {
  
  class image {

		
    public:
      image();
			image(const image& cpy);
      image(std::istream& in);
      ~image();		
		
			pixel& operator[] (int);
			const pixel& operator[] (int) const;
		
			void from_stream(std::istream& in);
      void to_stream(std::ostream& out, format out_format) const;
		
			std::pair<int,int> dimensions() const;
		
			
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
      
			
			float Hseuil(float p) const;
			
      image compose(convolution c) const;
			static image assemblage(const image& a, const image& b, pixelOperator op);
			
			image seuil_global(float s = 0.5) const;
			image seuil_local() const;
			image seuil_histerisis(float high, float low) const;
			
			image affinage() const;
			
			image close_naive(float sHigh					= -0.95,
												float sLow					= -0.85,
												convolution hFilter	= lenactions::convolution::SobelH(),
												convolution vFilter	= lenactions::convolution::SobelV()) const;
			
			image close_waves(float sHigh					= -0.95,
												float sLow					= -0.85,
												convolution hFilter	= lenactions::convolution::SobelH(),
												convolution vFilter	= lenactions::convolution::SobelV()) const;
			
			image hough_line() const;
			image hough_circle(int = -1) const;
			
			
    private:
      int o_cols(int j, int offset) const;
      int o_rows(int i, int offset) const;

		
      int rows;
      int cols;
      pixel* bitmap;	
  };
};
#endif // IMAGE_H
