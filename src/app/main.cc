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
  
	
  
	out.open("/dev/shm/5_final.ppm");		
	img.closedContours().to_stream(out, lenactions::P6);
	out.close();
	
  return 0;
}