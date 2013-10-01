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

#include "image.hh"

using namespace lenactions;

/*
 * Construction / Destruction
 */

image::image() :
	rows(0),
	cols(0),
	bitmap(NULL)
{
}

image::image(image& cpy) :
	rows(cpy.rows),
	cols(cpy.cols)
{
	bitmap = new pixel[rows*cols];
	for (int i = 0; i < rows*cols; i++)
		bitmap[i] = pixel(cpy.bitmap[i]);
}

image::image(std::istream& in) :
	bitmap(NULL)
{
	from_stream(in);
}

image::~image()
{
	if(bitmap) delete[] bitmap;
}

std::pair<int, int> image::dimensions() {
  return std::make_pair(cols, rows);
}


/*
 * Importation/Exportation
 */

void image::from_stream(std::istream& in)
{
  int scale;
  
  
  if(bitmap) delete[] bitmap;
  
  switch(getc(in))
	{
		case 'P':
			break;
		default:
			throw;
	}
	format in_format = (format) getint(in);
  
	cols = getint(in);
  rows = getint(in);
	scale = (in_format == P1  || in_format == P4)?1:getint(in);

	bitmap = new pixel[cols*rows];
  for (int i = 0; i < rows*cols; ++i)
	{
		switch (in_format)
		{
			case 1:
      {
        float g = (float) !getint(in);
				bitmap[i] = pixel(g);
				break;
			}
      case 2:
      {
        float g = (float) getint(in)/scale;
				bitmap[i] = pixel(g);
				break;
			}
      case 3:
			{
				float r = (float) getint(in)/scale;
				float g = (float) getint(in)/scale;
				float b = (float) getint(in)/scale;
				bitmap[i] = pixel(r, g,b, RGB);
				break;
			}
			case 4:
      {
        float g = (float) !getrawbyte(in);
				bitmap[i] = pixel(g);
				break;
			}
      case 5:
      {
        float g = (float) getrawbyte(in)/scale;
				bitmap[i] = pixel(g);
				break;
			}
      case 6:
			{
				float r = (float) getrawbyte(in)/scale;
				float g = (float) getrawbyte(in)/scale;
				float b = (float) getrawbyte(in)/scale;
				bitmap[i] = pixel(r, g, b, RGB);
				break;
			}
		}
	}
}

void image::to_stream(std::ostream& out, format out_format)
{
	out << "P" << out_format << std::endl;
	out << cols << " " << rows << std::endl;
	switch (out_format)
	{
		case P1:
		case P4:
			break;
		case P2:
		case P3:
		case P5:
		case P6:
			out << 255 << std::endl;
	}
	for (int i = 0; i < rows*cols; ++i)
		bitmap[i].to_stream(out, out_format);
}






/*
int image::o_cols(int j, int offset) {
  int r = j+offset;
  while (r < 0 || r >= cols)
    r = (r < 0)?-r:2*(cols-1)-r;
  return r;
}

int image::o_rows(int i, int offset) {
  int r = i+offset;
  while (r < 0 || r >= rows)
    r = (r < 0)?-r:2*(rows-1)-r;
  return r;
}
*/
/*
pixel* image::compose(convolution c)
{
	pixel* new_bitmap = new pixel[rows*cols];
  for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j)
    {  
      float r = 0;
      float g = 0;
      float b = 0;      
      for (int k = 0; k < c.size; ++k)
        for (int l = 0; l < c.size; ++l)
        {
          r += c.value[k*c.size+l]*bitmap[o_rows(i,k-c.offset)*cols+o_cols(j,l-c.offset)].get_canal(R);
          g += c.value[k*c.size+l]*bitmap[o_rows(i,k-c.offset)*cols+o_cols(j,l-c.offset)].get_canal(G);
          b += c.value[k*c.size+l]*bitmap[o_rows(i,k-c.offset)*cols+o_cols(j,l-c.offset)].get_canal(B);
        }
      new_bitmap[i*cols+j].set_canal(R, r/c.norm);
      new_bitmap[i*cols+j].set_canal(G, g/c.norm);
      new_bitmap[i*cols+j].set_canal(B, b/c.norm);
    }
  return new_bitmap;
}
*/
