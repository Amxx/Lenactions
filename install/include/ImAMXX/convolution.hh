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

#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "typedef.hh"

namespace ImAMXX {
  
  class image;
  
  class convolution {
    friend image;
    
    public:
      convolution(vByte* v, int s, int o, vByte n);
      ~convolution();
    
    private:
      vByte*    value;
      int       size;
      int       offset;
      vByte     norm;
  };
  
};



#endif //CONVOLUTION_H