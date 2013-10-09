#include <iostream>
#include <fstream>

#include <cstdio>
#include <cstdlib>


#include "pixel.hh"
#include "image.hh"


int main(int argc, char* argv[])
{
  if (argc<2) { printf("Usage %s <file>\n", argv[0]); exit(0); }
  
  
  lenactions::image img;
  
  
  std::ifstream in;
  std::ofstream out;
  
  in.open(argv[1]);
  if (!in.is_open()) { printf("Error opening file %s\n", argv[1]); exit(1); }
  img.from_stream(in);
  in.close();
  
	
	lenactions::image sobelh = img.compose(lenactions::convolution::SobelH());
	lenactions::image sobelv = img.compose(lenactions::convolution::SobelV());
	lenactions::pixelOperator op;
	
	/*
	out.open("/dev/shm/out_sobel.ppm");	
	op = lenactions::pixel::quadratic;
	lenactions::image::assemblage(sobelh, sobelv, op).to_stream(out, lenactions::P6);
	out.close();
	*/
	
	
	op = lenactions::pixel::angleteinte;
	lenactions::image sobelteinte = lenactions::image::assemblage(sobelh, sobelv, op);

	out.open("/dev/shm/out_sobel_teinte.ppm");	
	sobelteinte.to_stream(out, lenactions::P6);
	out.close();
	

	
	
	out.open("/dev/shm/out_sobel_teinte_seuilglobal.ppm");		
	sobelteinte.seuil_global(0.85).to_stream(out, lenactions::P6);
	out.close();
	
	out.open("/dev/shm/out_sobel_teinte_seuillocal.ppm");		
	sobelteinte.seuil_local().to_stream(out, lenactions::P6);
	out.close();
	
	
	float high =	sobelteinte.Hseuil(0.95);
	float low =		sobelteinte.Hseuil(0.85);
	
	out.open("/dev/shm/out_sobel_teinte_seuilhisterisis.ppm");		
	sobelteinte.seuil_histerisis(high, low).to_stream(out, lenactions::P6);
	out.close();
  
  return 0;
}