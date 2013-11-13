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
#include "tools/utils.hh"
#include "tools/priority.hh"
#include "tools/unionfind.hh"

#include <cassert>

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

image::image(const image& cpy) :
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

pixel& image::operator[] (int idx)
{
	return bitmap[idx];
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










float image::Hseuil(float p)
{
	int*	hist = new int[1024];
	int 	seuil = 0;
	int 	cumul = 0;
	
	for (int i=0; i<1024; ++i)
		hist[i] = 0;
	for (int i=0; i<rows*cols; ++i)
		hist[(int) (1024*bitmap[i].get_canal(V))]++;

	while (cumul<p*rows*cols)
	{
		cumul+=hist[seuil];
		seuil++;
	}
	return (float) seuil/1024;
}







image image::compose(convolution c)
{
	image composed;
	composed.rows = 	rows;
	composed.cols = 	cols;
	composed.bitmap = new pixel[rows*cols];
	
  for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j)
    {  
      float r = 0;
      float g = 0;
      float b = 0;
      for (int k = 0; k < 3; ++k)
        for (int l = 0; l < 3; ++l)
        {
          r += c.val[3*k+l]*bitmap[o_rows(i,k-1)*cols+o_cols(j,l-1)].get_canal(R);
          g += c.val[3*k+l]*bitmap[o_rows(i,k-1)*cols+o_cols(j,l-1)].get_canal(G);
          b += c.val[3*k+l]*bitmap[o_rows(i,k-1)*cols+o_cols(j,l-1)].get_canal(B);
        }
      composed.bitmap[i*cols+j].set_canal(R, r/c.norm);
      composed.bitmap[i*cols+j].set_canal(G, g/c.norm);
      composed.bitmap[i*cols+j].set_canal(B, b/c.norm);
    }
  return composed;
}

image image::assemblage(image& a, image& b, pixelOperator op)
{
	if (a.cols != b.cols || a.rows != b.rows) throw;
	
	image assembled;
	assembled.rows = 		a.rows;
	assembled.cols = 		a.cols;
	assembled.bitmap =	new pixel[a.rows*a.cols];
	
	for (int i = 0; i < a.rows; ++i)
		for (int j = 0; j < a.cols; ++j)
			assembled.bitmap[i*a.cols+j] = op(a.bitmap[i*a.cols+j], b.bitmap[i*a.cols+j]);
	return assembled;
}
			







image image::seuil_global(float p)
{
	image seuilled;
	seuilled.rows = 	rows;
	seuilled.cols = 	cols;
	seuilled.bitmap =	new pixel[rows*cols];
	
	float seuil = (p>0)?p:Hseuil(-p);
	
	for (int i=0; i<rows*cols; ++i)
		if (bitmap[i].get_canal(V)>seuil)
			seuilled.bitmap[i] = bitmap[i];
	return seuilled;
}

image image::seuil_local()
{
	float filtre[9] = { 1.f, 1.f, 1.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f};
	image composed = image::compose(convolution(filtre));
	for (int i=0; i<rows*cols; ++i)
		if (bitmap[i].get_canal(V) > composed.bitmap[i].get_canal(V))
			composed.bitmap[i] = bitmap[i];
		else
			composed.bitmap[i].set_canal(V, 0);
	return composed;
}

image image::seuil_histerisis(float high, float low)
{
	float shigh = (high>0)?high:Hseuil(-high);
	float slow  = (low>0)	?low :Hseuil(-low );
	
	
	Unionfind* uf = new Unionfind[rows*cols];
	for (int i=0; i<rows; ++i)
		for (int j=0; j<cols; ++j)
		{
			if (bitmap[i*cols+j].get_canal(V) > shigh)
				uf[i*cols+j].flag() = 1;
			if (bitmap[i*cols+j].get_canal(V) > slow)
			{
				if (i>0      && j>0      && bitmap[(i-1) * cols + j-1].get_canal(V) > slow)		uf[i*cols+j].join(uf[(i-1) * cols + j-1]);
				if (i>0                  && bitmap[(i-1) * cols + j  ].get_canal(V) > slow)		uf[i*cols+j].join(uf[(i-1) * cols + j  ]);
				if (i>0      && j<cols-1 && bitmap[(i-1) * cols + j+1].get_canal(V) > slow)		uf[i*cols+j].join(uf[(i-1) * cols + j+1]);
				if (            j<cols-1 && bitmap[i		 * cols + j+1].get_canal(V) > slow)		uf[i*cols+j].join(uf[i     * cols + j+1]);
				if (i<rows-1 && j>cols-1 && bitmap[(i+1) * cols + j+1].get_canal(V) > slow)		uf[i*cols+j].join(uf[(i+1) * cols + j+1]);
				if (i<rows-1             && bitmap[(i+1) * cols + j  ].get_canal(V) > slow)		uf[i*cols+j].join(uf[(i+1) * cols + j  ]);
				if (i<rows-1 && j>0      && bitmap[(i+1) * cols + j-1].get_canal(V) > slow)		uf[i*cols+j].join(uf[(i+1) * cols + j-1]);
				if (            j>0      && bitmap[i     * cols + j-1].get_canal(V) > slow)		uf[i*cols+j].join(uf[i     * cols + j-1]);
			}
		}
		
	image seuilled;
	seuilled.rows = 	rows;
	seuilled.cols = 	cols;
	seuilled.bitmap =	new pixel[rows*cols];
	
	for (int i=0; i<rows*cols; ++i)
		if (uf[i].root().flag())
			seuilled.bitmap[i] = bitmap[i];
	
	return seuilled;
}






image image::affinage()
{
	image affined;
	affined.rows		=		rows;
	affined.cols		=		cols;
	affined.bitmap	=		new pixel[rows*cols];
	
	bool*	viewed = new bool[rows*cols];
	for (int i=0; i<rows*cols; ++i)
		viewed[i] = false;
	
	for (int i=0; i<rows; i+=1)
		for (int j=0; j<cols; j+=1)
			if ( bitmap[i*cols+j].get_canal(V) && !viewed[i*cols+j] )
			{
				/*
				float theta = 	PI * bitmap[i*cols+j].get_canal(H) / 180.;
				int		x, y;
				float dx		=	sin(theta);
				float dy		=	cos(theta);
				float tmax	=	0.;
				float tmin	=	0.;
				do
				{
					tmin--;
					x = (int) (tmin*dx + .5 + i);
					y = (int) (tmin*dy + .5 + j);
				}
				while
				(
						x > 0 && x < rows &&	y > 0 && y < cols
				&&	bitmap[x*cols+y].get_canal(V)
				&&	fmod(fabs(bitmap[i*cols+j].get_canal(H) - bitmap[x*cols+y].get_canal(H)), 360) < 90.
				);
				
				do
				{
					tmax++;
					x = (int) (tmax*dx + .5 + i);
					y = (int) (tmax*dy + .5 + j);
				}
				while
				(
						x > 0 && x < rows &&	y > 0 && y < cols
				&&	bitmap[x*cols+y].get_canal(V)
				&&	fmod(fabs(bitmap[i*cols+j].get_canal(H) - bitmap[x*cols+y].get_canal(H)), 360) < 90.
				);
				
				x = (int) (((tmax+tmin)/2.)*dx + .5 + i);
				y = (int) (((tmax+tmin)/2.)*dy + .5 + j);
				affined.bitmap[x*cols+y] = bitmap[x*cols+y];
				*/
				
				int x, dx;
				int y, dy;
				int tmin, tmax;
				
				switch ( (int) (std::round(bitmap[i*cols+j].get_canal(H)/45)) % 4 )
				{
					case 0: dx =  1; dy =  0; break;
					case 1: dx =  1; dy =  1; break;
					case 2: dx =  0; dy =  1; break;
					case 3: dx = -1; dy =  1; break;
				}
				
				int k1 = 0;
				while(true)
				{
					x = i + k1*dx;
					y = j + k1*dy;
					if (x < 0 || x >= rows ||	y < 0 || y >= cols) break;
					if (bitmap[x*cols+y].get_canal(V) == 0.)			break;
					viewed[x*cols+y] = true;
					++k1;
				}
				
				int k2 = 0;
				while(true)
				{
					x = i + k2*dx;
					y = j + k2*dy;
					if (x < 0 || x >= rows ||	y < 0 || y >= cols) break;
					if (bitmap[x*cols+y].get_canal(V) == 0.)			break;
					viewed[x*cols+y] = true;
					--k2;
				}
				
				x = ((k1+k2)/2.)*dx + i;
				y = ((k1+k2)/2.)*dy + j;
				affined.bitmap[x*cols+y] = bitmap[x*cols+y];
				
			}
			
	return affined;
}




//===========================================================================================================================

image image::close_naive(	float sHigh,
													float sLow,
													convolution hFilter,
													convolution vFilter)
{
	image hGrad 	= this->compose(hFilter);
	image vGrad 	= this->compose(vFilter);
	image	gTone 	= assemblage(hGrad, vGrad, pixel::angleteinte);
	
	image Border	= gTone.seuil_histerisis(sHigh, sLow);
	
	image Affined	= Border.affinage(); 
	
  std::ofstream out;

	out.open("/dev/shm/1a_hGrad.ppm");	hGrad.to_stream(out, lenactions::P6);	out.close();
	out.open("/dev/shm/1b_vGrad.ppm");	vGrad.to_stream(out, lenactions::P6);	out.close();
	out.open("/dev/shm/2_gTone.ppm");		gTone.to_stream(out, lenactions::P6);	out.close();
	out.open("/dev/shm/3_Border.ppm");	Border.to_stream(out, lenactions::P6);	out.close();
	out.open("/dev/shm/4_Affined.ppm");	Affined.to_stream(out, lenactions::P6);	out.close();
	
	//-------------------------------------------------------------------------------------------------------------------------
			
	std::vector<std::pair<int, int>> 				anchors;
	
	for (int i=1; i<rows-1; ++i)
		for (int j=1; j<cols-1; ++j)
			if (Affined[i*cols+j].v())
			{
				
				int c = 0;
				
				if (Affined[(i+1) * cols + j  ].v())	c |= 0x00000001;
				if (Affined[i     * cols + j+1].v())	c |= 0x00000002;
				if (Affined[(i-1) * cols + j  ].v())	c |= 0x00000004;
				if (Affined[i     * cols + j-1].v())	c |= 0x00000008;
				
				switch (c)
				{
					case 0x00000000:
						anchors.push_back(std::make_pair(i*cols+j, -1));
						anchors.push_back(std::make_pair(i*cols+j, -1));
						break;
					
					case 0x00000001:
						anchors.push_back(std::make_pair(i*cols+j, 0));
						break;
					
					case 0x00000002:
						anchors.push_back(std::make_pair(i*cols+j, 1));
						break;
						
					case 0x00000004:
						anchors.push_back(std::make_pair(i*cols+j, 2));
						break;
						
					case 0x00000008:
						anchors.push_back(std::make_pair(i*cols+j, 3));
						break;
					
												
					default:
						break;
				}
			}
	
	while(!anchors.empty())
	{
		int pos = anchors.back().first;
		int dir = anchors.back().second;
		int i		= pos / cols;
		int j		= pos % cols;
		
		if (dir == -1)
		{
			if (Affined[(i+1) * cols + j  ].v())	dir = 0;
			if (Affined[i     * cols + j+1].v())	dir = 1;
			if (Affined[(i-1) * cols + j  ].v())	dir = 2;
			if (Affined[i     * cols + j-1].v())	dir = 3;
		}
		
		do
		{
			
			if (Affined[pos].v() == 0.) Affined[pos] = pixel(1., 1., 1.);

			int		new_pos;
			int 	new_dir;
			float max_val	= -1;
			int		neigh		= 0;
						
			if (Affined[(i+1)*cols+    j].v()) ++neigh;
			if (Affined[    i*cols+(j+1)].v()) ++neigh;
			if (Affined[(i-1)*cols+    j].v()) ++neigh;
			if (Affined[    i*cols+(j-1)].v()) ++neigh;
			
			if ( neigh > 1) break;
				
			if ( dir != 0 && gTone[(i+1)*cols+    j].v() > max_val ) { new_pos = (i+1)*cols+    j; new_dir = 2; max_val = gTone[new_pos].v(); }
			if ( dir != 1 && gTone[    i*cols+(j+1)].v() > max_val ) { new_pos =     i*cols+(j+1); new_dir = 3; max_val = gTone[new_pos].v(); }
			if ( dir != 2 && gTone[(i-1)*cols+    j].v() > max_val ) { new_pos = (i-1)*cols+    j; new_dir = 0; max_val = gTone[new_pos].v(); }
			if ( dir != 3 && gTone[    i*cols+(j-1)].v() > max_val ) { new_pos =     i*cols+(j-1); new_dir = 1; max_val = gTone[new_pos].v(); }
			
			pos = new_pos;
			dir = new_dir;
			i		= pos / cols;
			j		= pos % cols;

		} 
		while ( i != 0 && i != rows-1 && j != 0 && j != cols-1 && Affined[pos].v() == 0.);
		anchors.pop_back();
	}
	
	return Affined;	
}





//===========================================================================================================================

// #define SET_WHITE
// #define SHOW_ANCHOR
// #define SHOW_DIFFUSION

#define				TYPE_EMPTY		0
#define				TYPE_FIELD		1
#define				TYPE_BORDER		2
#define				TYPE_ATTACH		4
#define 			LINK(x) 			{ int idx = x; while (idx != fieldFather[idx]) { Affined[idx] = pixel(1., 1., 1.); idx = fieldFather[idx]; } }

image image::close_waves(	float sHigh,
													float sLow,
													convolution hFilter,
													convolution vFilter)
{
	image hGrad 	= this->compose(hFilter);
	image vGrad 	= this->compose(vFilter);
	image	gTone 	= assemblage(hGrad, vGrad, pixel::angleteinte);
	
	image Border	= gTone.seuil_histerisis(sHigh, sLow);
	
	image Affined	= Border.affinage(); 
	
	
  std::ofstream out;

	out.open("/dev/shm/1a_hGrad.ppm");	hGrad.to_stream(out, lenactions::P6);	out.close();
	out.open("/dev/shm/1b_vGrad.ppm");	vGrad.to_stream(out, lenactions::P6);	out.close();
	out.open("/dev/shm/2_gTone.ppm");		gTone.to_stream(out, lenactions::P6);	out.close();
	out.open("/dev/shm/3_Border.ppm");	Border.to_stream(out, lenactions::P6);	out.close();
	out.open("/dev/shm/4_Affined.ppm");	Affined.to_stream(out, lenactions::P6);	out.close();
	
	//-------------------------------------------------------------------------------------------------------------------------
	
	priority<float, std::pair<int, int>>	fieldDiffusion;
	int*																	type					= new int[rows*cols];
	int*																	adjacency			=	new int[rows*cols];
	int*																	fieldFather		= new int[rows*cols];
	int*																	fieldRoot			= new int[rows*cols];
	Unionfind*														componants		= new Unionfind[rows*cols];
	
	for (int i=0; i<rows*cols; ++i)
	{
		adjacency  [i] = 0;
		type       [i] = TYPE_EMPTY;
		fieldFather[i] = -1;
		fieldRoot  [i] = -1;
	}
	
	for (int i=1; i<rows-1; ++i)
		for (int j=1; j<cols-1; ++j)
			if (Affined[i*cols+j].v())
			{
				#ifdef SET_WHITE
				Affined[i*cols+j] = pixel(1., 1., 1.);
				#endif
				
				int c = 0;
				
				if (Affined[(i-1) * cols + j-1].v())	{ componants[i*cols+j].join(componants[(i-1) * cols + j-1]); c |= 0x00000001; }
				if (Affined[(i-1) * cols + j  ].v())	{ componants[i*cols+j].join(componants[(i-1) * cols + j  ]); c |= 0x00000002; }
				if (Affined[(i-1) * cols + j+1].v())	{ componants[i*cols+j].join(componants[(i-1) * cols + j+1]); c |= 0x00000004; }
				if (Affined[i     * cols + j+1].v())	{ componants[i*cols+j].join(componants[i     * cols + j+1]); c |= 0x00000008; }
				if (Affined[(i+1) * cols + j+1].v())	{ componants[i*cols+j].join(componants[(i+1) * cols + j+1]); c |= 0x00000010; }
				if (Affined[(i+1) * cols + j  ].v())	{ componants[i*cols+j].join(componants[(i+1) * cols + j  ]); c |= 0x00000020; }
				if (Affined[(i+1) * cols + j-1].v())	{ componants[i*cols+j].join(componants[(i+1) * cols + j-1]); c |= 0x00000040; }
				if (Affined[i     * cols + j-1].v())	{ componants[i*cols+j].join(componants[i     * cols + j-1]); c |= 0x00000080; }
			
				switch (c)
				{
					case 0x00000000:
						adjacency	 [i*cols+j] =	2;
						type       [i*cols+j]	=	TYPE_ATTACH;
						fieldFather[i*cols+j] =	i*cols+j;
						fieldRoot  [i*cols+j] =	i*cols+j;
						fieldDiffusion.push(0., std::make_pair(i*cols+j, i*cols+j));
						#ifdef SHOW_ANCHOR
						Affined[i*cols+j] = pixel(1., 0., 0.);
						#endif
						break;
					
					case 0x00000001:	case 0x00000003:
					case 0x00000002:	case 0x00000006:
					case 0x00000004:	case 0x0000000c:
					case 0x00000008:	case 0x00000018:
					case 0x00000010:	case 0x00000030:
					case 0x00000020:	case 0x00000060:
					case 0x00000040:	case 0x000000c0:
					case 0x00000080:	case 0x00000081:
						adjacency  [i*cols+j] =	1;
						type       [i*cols+j]	=	TYPE_ATTACH;
						fieldFather[i*cols+j] =	i*cols+j;
						fieldRoot  [i*cols+j] =	i*cols+j;
						fieldDiffusion.push(0., std::make_pair(i*cols+j, i*cols+j));
						#ifdef SHOW_ANCHOR
						Affined[i*cols+j] = pixel(1., 0., 0.);
						#endif
						break;
					
					default:
						type       [i*cols+j]	=	TYPE_BORDER;
						break;
				}
			}
	
	// printf("Closing : %ld points of interest\n", fieldDiffusion.size());			
			
	while(!fieldDiffusion.empty())
	{
		float	dst = fieldDiffusion.top_key();
		int		anc = fieldDiffusion.top().second;
		int		pos = fieldDiffusion.top().first;
		fieldDiffusion.pop();
		
		int i = pos / cols;
		int j = pos % cols;
		
		if (adjacency[fieldRoot[anc]] <= 0) continue;

		if (type[pos] == TYPE_FIELD)																				// Liaison par contact champ-champ
		{
			if (fieldRoot[pos] == fieldRoot[anc]) continue;
			adjacency[fieldRoot[pos]]--;
			adjacency[fieldRoot[anc]]--;
			LINK(pos);
			LINK(anc);
		}			
		else if (type[pos] == TYPE_BORDER)																	// Rattachement
		{
			if (&(componants[pos].root()) == &(componants[fieldRoot[anc]].root())) continue;
			adjacency[fieldRoot[anc]]--;			
			fieldFather[pos] = anc;
			LINK(pos);
		}
		else if (type[pos] == TYPE_ATTACH && pos != anc)										// Rattachement
		{
			if (&(componants[pos].root()) == &(componants[fieldRoot[anc]].root())) continue;
			
			adjacency[fieldRoot[anc]]--;
			fieldFather[pos] = anc;
			LINK(pos);			
		}
		else if (i == 0 || i == rows-1 || j == 0 || j == rows-1)						// Frontiere
		{
			adjacency[fieldRoot[anc]]--;
			fieldFather[pos] = anc;
			LINK(pos);
		}
		else																																// Expantion du champ
		{
			type[pos]					= TYPE_FIELD;
			fieldFather[pos]	= anc;
			fieldRoot[pos]		= fieldRoot[anc];
			#ifdef SHOW_DIFFUSION
			Affined[i*cols+j] = pixel(1., 1., 0.);
			#endif
			fieldDiffusion.push(dst + 1.343 * (1.1 - hGrad[i*cols+j].v() * hGrad[(i+1)*cols+(j-1)].v() - vGrad[i*cols+j].v() * vGrad[(i+1)*cols+(j-1)].v()), std::make_pair((i+1)*cols+(j-1), pos));
			fieldDiffusion.push(dst + 0.948 * (1.1 - hGrad[i*cols+j].v() * hGrad[(i+1)*cols+    j].v() - vGrad[i*cols+j].v() * vGrad[(i+1)*cols+    j].v()), std::make_pair((i+1)*cols+    j, pos));
			fieldDiffusion.push(dst + 1.343 * (1.1 - hGrad[i*cols+j].v() * hGrad[(i+1)*cols+(j+1)].v() - vGrad[i*cols+j].v() * vGrad[(i+1)*cols+(j+1)].v()), std::make_pair((i+1)*cols+(j+1), pos));
			fieldDiffusion.push(dst + 0.948 * (1.1 - hGrad[i*cols+j].v() * hGrad[    i*cols+(j+1)].v() - vGrad[i*cols+j].v() * vGrad[    i*cols+(j+1)].v()), std::make_pair(    i*cols+(j+1), pos));
			fieldDiffusion.push(dst + 1.343 * (1.1 - hGrad[i*cols+j].v() * hGrad[(i-1)*cols+(j+1)].v() - vGrad[i*cols+j].v() * vGrad[(i-1)*cols+(j+1)].v()), std::make_pair((i-1)*cols+(j+1), pos));
			fieldDiffusion.push(dst + 0.948 * (1.1 - hGrad[i*cols+j].v() * hGrad[(i-1)*cols+    j].v() - vGrad[i*cols+j].v() * vGrad[(i-1)*cols+    j].v()), std::make_pair((i-1)*cols+    j, pos));
			fieldDiffusion.push(dst + 1.343 * (1.1 - hGrad[i*cols+j].v() * hGrad[(i-1)*cols+(j-1)].v() - vGrad[i*cols+j].v() * vGrad[(i-1)*cols+(j-1)].v()), std::make_pair((i-1)*cols+(j-1), pos));
			fieldDiffusion.push(dst + 0.948 * (1.1 - hGrad[i*cols+j].v() * hGrad[    i*cols+(j-1)].v() - vGrad[i*cols+j].v() * vGrad[    i*cols+(j-1)].v()), std::make_pair(    i*cols+(j-1), pos));
		}
	}
	return Affined;	
}
















int image::o_cols(int j, int offset) {
  int r = j+offset;
  while (r < 0 || r >= cols)
    r = (r < 0)?(-r):(2*(cols-1)-r);
  return r;
}

int image::o_rows(int i, int offset) {
  int r = i+offset;
  while (r < 0 || r >= rows)
    r = (r < 0)?(-r):(2*(rows-1)-r);
  return r;
}

