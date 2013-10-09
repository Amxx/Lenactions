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

#ifndef CONVOLUTION_HH
#define CONVOLUTION_HH

namespace lenactions {
	
	class image;
	
	class convolution {
		friend class image;
		
		public:
			convolution(float v[9]);
			convolution(float v[9], float n);
		
			static convolution PrewitzH();
			static convolution PrewitzV();
			static convolution SobelH();
			static convolution SobelV();
			static convolution KirschH();
			static convolution KirschV();
		
		private:
			float	val[9];
			float	norm;
	};	
}


#endif