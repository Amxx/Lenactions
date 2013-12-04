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

#include "pixel.hh"

using namespace lenactions;

/*
 * __________________________________ Construct ___________________________________
 */

pixel::pixel()
{
  space = RGB;
  value[0] = 0;
  value[1] = 0;
  value[2] = 0;
}

pixel::pixel(const pixel& cpy)
{
  space = cpy.space;
  value[0] = cpy.value[0];
  value[1] = cpy.value[1];
  value[2] = cpy.value[2];
}

pixel::pixel(float g)
{
  space = RGB;
  value[0] = g;
  value[1] = g;
  value[2] = g;
}

pixel::pixel(float x, float y, float z, colorSpace s)
{
  space = s;
  value[0] = x;
  value[1] = y;
  value[2] = z;
}

/*
 * ____________________________________ Access ____________________________________
 */

float& pixel::get_canal(canal c) 
{
  switch (c)
  {
    case R:
    case G:
    case B:
      set_space(RGB);
      return value[c];
    case H:
    case S:
    case V:
      set_space(HSV);
      return value[c - 3];
    case GREY:
      set_space(HSV);
			return value[2];
  }
  throw;
}

float& pixel::r() { return get_canal(R); }
float& pixel::g() { return get_canal(G); }
float& pixel::b() { return get_canal(B); }
float& pixel::h() { return get_canal(H); }
float& pixel::s() { return get_canal(S); }
float& pixel::v() { return get_canal(V); }

void pixel::set_canal(canal c, float v)
{
  switch (c)
  {
    case R:
    case G:
    case B:
      set_space(RGB);
      value[c] = v;
      break;
    case H:
    case S:
    case V:
      set_space(HSV);
      value[c - 3] = v;
      break;
    case GREY:
      if (space == HSV)
      {
        value[1] = 0;
        value[2] = v;
      }
      else
      {
        value[0] = v;
        value[1] = v;
        value[2] = v;
      }
  }
}

void pixel::set_space(colorSpace s)
{
  if (space == s)
    return;
  
  switch (s)
  {
    case RGB:
    {
      float r;
      float g;
      float b;
      float h = get_canal(H) / 60;
      float s = get_canal(S);
      float v = get_canal(V);
      
      float c = s * v;
      float x = c * (1.0 - std::abs(fmod(h, 2.0) - 1.0));

      switch ((int) h % 6)
			{
				case 0:
					r = c;
					g = x;
					b = 0;
					break;
				case 1:
					r = x;
					g = c;
					b = 0;
					break;
				case 2:
					r = 0;
					g = c;
					b = x;
					break;
				case 3:
					r = 0;
					g = x;
					b = c;
					break;
				case 4:
					r = x;
					g = 0;
					b = c;
					break;
				case 5:
					r = c;
					g = 0;
					b = x;
					break;
				default:
					throw;
			}
      
      space = RGB;
      set_canal(R, r + v - c);
      set_canal(G, g + v - c);
      set_canal(B, b + v - c);			
      break;
    }
    
    case HSV:
    {      
      float r = get_canal(R);
      float g = get_canal(G);
      float b = get_canal(B);
      float h = 0;
      float s = 0;
      float v = 0;
 
      float rgb_min = MIN3(r, g, b);
      float rgb_max = MAX3(r, g, b);
      
      if ((v = rgb_max) == 0)
      {
        space = HSV;
        set_canal(H, h);
        set_canal(S, s);
        set_canal(V, v);
        break;
      }
      
      r /= v;
      g /= v;
      b /= v;
      
      rgb_min = MIN3(r, g, b);
      rgb_max = MAX3(r, g, b);
      
      if ((s = rgb_max - rgb_min) == 0)
      {
        space = HSV;
        set_canal(H, h);
        set_canal(S, s);
        set_canal(V, v);
        break;
      }

			r = (r - rgb_min)/(rgb_max - rgb_min);		
			g = (g - rgb_min)/(rgb_max - rgb_min);
			b = (b - rgb_min)/(rgb_max - rgb_min);
      
			rgb_min = MIN3(r, g, b);
			rgb_max = MAX3(r, g, b);
			
			if (rgb_max == r)
			{
				h = 0.0 + 60.0 * (g - b);
				if (h < 0.0) h += 360.0;
			}
			else if (rgb_max == g)
				h = 120.0 + 60.0 * (b - r);
			else /* rgb_max == rgb.b */
				h = 240.0 + 60.0 * (r - g);
      
      space = HSV;
      set_canal(H, h);
      set_canal(S, s);
      set_canal(V, v);
			break;
    }
    
  }
}


/*
 * ____________________________________ Export ____________________________________
 */

void pixel::to_stream(std::ostream& out, format out_format)
{
	switch(out_format)
	{
		case P1:
			out << (int) (get_canal(V) <= .5)   << " ";
			break;
		case P2:
			out << (int) (255 * get_canal(V))   << " ";
			break;
		case P3:
			out << (int) (255 * get_canal(R))   << " "
					<< (int) (255 * get_canal(G))   << " "
					<< (int) (255 * get_canal(B))   << " ";
			break;
		case P4:
			out << (unsigned char) (get_canal(V) <= .5);
			break;
		case P5:
			out << (unsigned char) (255 * get_canal(V));
			break;
		case P6:
			out << (unsigned char) (255 * get_canal(R))
					<< (unsigned char) (255 * get_canal(G))
					<< (unsigned char) (255 * get_canal(B));
    
	}
}















pixel pixel::quadratic(pixel& pi, pixel& pj)
{
	float r = sqrt(pi.get_canal(R)*pi.get_canal(R) + pj.get_canal(R)*pj.get_canal(R));
	float g = sqrt(pi.get_canal(G)*pi.get_canal(G) + pj.get_canal(G)*pj.get_canal(G));
	float b = sqrt(pi.get_canal(B)*pi.get_canal(B) + pj.get_canal(B)*pj.get_canal(B));
	return pixel(r, g, b, RGB);
}

pixel	pixel::angleteinte(pixel&pi, pixel& pj)
{
	float h = 180. * atan2(pj.get_canal(V), pi.get_canal(V)) / PI;
	float s = 1;
	float v = sqrt(pi.get_canal(V)*pi.get_canal(V)+pj.get_canal(V)*pj.get_canal(V));
	
	if (h < 0.) h += 360.;
	
	return pixel(h, s, v, HSV);	
}
