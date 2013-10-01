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
  
  /*
  img.gamma(2.);
  
  ImAMXX::image prewitt(img);
  ImAMXX::image sobel(img);
  ImAMXX::image kirsch(img);
  
  prewitt.contour_prewitt();
  sobel.contour_sobel();
  kirsch.contour_kirsch();
  
  out.open("/dev/shm/out_prezitt.ppm");
  prewitt.to_stream(out, ImAMXX::P6);
  out.close();
  
  out.open("/dev/shm/out_sobel.ppm");
  sobel.to_stream(out, ImAMXX::P6);
  out.close();
  
  out.open("/dev/shm/out_kirsch.ppm");
  kirsch.to_stream(out, ImAMXX::P6);
  out.close();
  */
  
  return 0;
}