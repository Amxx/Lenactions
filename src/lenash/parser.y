%{
	#include <iostream>
	#include <fstream>
  #include <map>
  
	#include "Lenactions.hh"
	
	extern int line;
	
  static std::map<std::string, lenactions::image*> imgs;

  int yylex(); 
  int yyerror(std::string msg) {
    std::cerr	<< "---"
							<< "[ERROR] parser : "
							<< msg
							<< " @ line "
							<< line
							<< std::endl;		
		return 1;
  }

%}

%union {
  int         integer;
  float       floating;
  char*       string;
}


%token            TK_SEMI
%token<floating>  TK_FLOAT
%token<integer>   TK_FORMAT
%token<string>    TK_VAR
%token<string>    TK_PATH
%token<string>    TK_STRING

%token            TK_CMD_QUIT
%token            TK_CMD_ECHO
%token            TK_CMD_LIST
%token            TK_CMD_LOAD
%token            TK_CMD_EXPORT
%token            TK_CMD_COPY
%token            TK_CMD_DELETE

%token            TK_CMD_SMOOTH
%token            TK_CMD_PREWITT
%token            TK_CMD_SOBEL
%token            TK_CMD_KIRSCH

%token            TK_CMD_HSEUIL
%token            TK_CMD_SEUILG
%token            TK_CMD_SEUILL
%token            TK_CMD_SEUILH
%token            TK_CMD_AFFINAGE
%token            TK_CMD_CLOSEN
%token            TK_CMD_CLOSEW

%start prg

%%

prg:
| line prg
;

line:
// =================================================================================================
  TK_CMD_QUIT TK_SEMI
  {
    std::cout << "cleaning memory..." << std::flush;
    for(auto im : imgs) delete im.second; 
    std::cout << "done" << std::endl;
    exit(0);
  }
| TK_CMD_ECHO TK_STRING TK_SEMI
  {
    std::cout << $2 << std::endl;
    free($2);
  }
| TK_CMD_LIST TK_SEMI
  {
    std::cout << "Images list (" << imgs.size() << " images loaded)" << std::endl;
    for (auto img : imgs)
      std::cout << " - " << img.first << " [" << img.second->dimensions().first << "x" << img.second->dimensions().second << "]" << std::endl;
  }
// =================================================================================================
| TK_CMD_LOAD TK_VAR TK_PATH TK_SEMI
  {
    if(imgs.find($2) != imgs.end()) {
      delete imgs[$2];
      imgs.erase($2);
    }
    std::ifstream stream;
    stream.open($3);
    if (stream.is_open()) {
      try {
        std::cout << "importing..." << std::flush;
        imgs[$2] = new lenactions::image(stream); 
        std::cout << "done" << std::endl;
      } catch (...) {
        std::cerr << "[Error] invalid input file: " << $3 << std::endl;
      }
      stream.close();
    } else {
      std::cerr << "[Error] Could not open file: >>" << $3 << "<<" << std::endl;
    }
    free($2);
    free($3);
  }
| TK_CMD_EXPORT TK_VAR TK_PATH TK_SEMI
  {
    if (imgs.find($2) !=  imgs.end()) {
      std::ofstream stream;
      stream.open($3);
      if (stream.is_open()) {
        try {
          std::cout << "exporting..." << std::flush;
          imgs[$2]->to_stream(stream, lenactions::P6);
          std::cout << "done" << std::endl;
        } catch(...) {
          std::cerr << "[Error] error exporting to file: " << $3 << std::endl;
        }
        stream.close();
      } else {
        std::cerr << "[Error] Could not open file: " << $3 << std::endl;
      }
    } else {    
      std::cerr << "[Error] Unknown picture: " << $2 << std::endl;
    }
    free($2);
    free($3);
  }
| TK_CMD_EXPORT TK_VAR TK_PATH TK_FORMAT TK_SEMI
  {
    if (imgs.find($2) !=  imgs.end()) {
      std::ofstream stream;
      stream.open($3);
      if (stream.is_open()) {
        try {
          std::cout << "exporting..." << std::flush;
          imgs[$2]->to_stream(stream, (lenactions::format) $4);
          std::cout << "done" << std::endl;
        } catch(...) {
          std::cerr << "[Error] error exporting to file: " << $3 << std::endl;
        }
        stream.close();
      } else {
        std::cerr << "[Error] Could not open file: " << $3 << std::endl;
      }
    } else {    
      std::cerr << "[Error] Unknown picture: " << $2 << std::endl;
    }
    free($2);
    free($3);
  }
| TK_CMD_COPY TK_VAR TK_VAR TK_SEMI
  {
    if (imgs.find($2) !=  imgs.end()) {
      if (imgs.find($3) !=  imgs.end()) {
        delete imgs[$3];
        imgs.erase($3);
      }
      std::cout << "copying..." << std::flush;
      imgs[$3] = new lenactions::image(*imgs[$2]);
      std::cout << "done" << std::endl;
    } else {    
      std::cerr << "[Error] Unknown picture: " << $2 << std::endl;
    }
    free($2);
    free($3);
  }
| TK_CMD_DELETE TK_VAR TK_SEMI
  {
    if (imgs.find($2) !=  imgs.end()) {
      std::cout << "deleting..." << std::flush;
      delete imgs[$2];
      imgs.erase($2);
      std::cout << "done" << std::endl;
    } else {    
      std::cerr << "[Error] Unknown picture: " << $2 << std::endl;
    }
    free($2);
  }
// =================================================================================================
| TK_CMD_SMOOTH TK_VAR TK_SEMI
  {
    if (imgs.find($2) !=  imgs.end()) {
      std::cout << "smoothing..." << std::flush;
      
			lenactions::image* tmp = new lenactions::image(imgs[$2]->compose(lenactions::convolution::Smooth()));
			delete imgs[$2];
			imgs[$2] = tmp;
			
      std::cout << "done" << std::endl;
    } else {    
      std::cerr << "[Error] Unknown picture: " << $2 << std::endl;
    }
    free($2);
  }
| TK_CMD_PREWITT TK_VAR TK_SEMI
  {
    if (imgs.find($2) !=  imgs.end()) {
      std::cout << "prewitt..." << std::flush;
			
			lenactions::image tempH = imgs[$2]->compose(lenactions::convolution::PrewitzH());
			lenactions::image tempV = imgs[$2]->compose(lenactions::convolution::PrewitzV());

			lenactions::image* tmp = new lenactions::image(lenactions::image::assemblage(tempH, tempV, lenactions::pixel::angleteinte));
			delete imgs[$2];
			imgs[$2] = tmp;
			
      std::cout << "done" << std::endl;
    } else {    
      std::cerr << "[Error] Unknown picture: " << $2 << std::endl;
    }
    free($2);
  }
| TK_CMD_SOBEL TK_VAR TK_SEMI
  {
    if (imgs.find($2) !=  imgs.end()) {
      std::cout << "sobel..." << std::flush;
										
			lenactions::image tempH = imgs[$2]->compose(lenactions::convolution::SobelH());
			lenactions::image tempV = imgs[$2]->compose(lenactions::convolution::SobelV());
			
			lenactions::image* tmp = new lenactions::image(lenactions::image::assemblage(tempH, tempV, lenactions::pixel::angleteinte));
			delete imgs[$2];
			imgs[$2] = tmp;
			
      std::cout << "done" << std::endl;
    } else {    
      std::cerr << "[Error] Unknown picture: " << $2 << std::endl;
    }
    free($2);
  }
| TK_CMD_KIRSCH TK_VAR TK_SEMI
  {
    if (imgs.find($2) !=  imgs.end()) {
      std::cout << "kirsch..." << std::flush;
			
			lenactions::image tempH = imgs[$2]->compose(lenactions::convolution::KirschH());
			lenactions::image tempV = imgs[$2]->compose(lenactions::convolution::KirschV());
			
			lenactions::image* tmp = new lenactions::image(lenactions::image::assemblage(tempH, tempV, lenactions::pixel::angleteinte));
			delete imgs[$2];
			imgs[$2] = tmp;
			
      std::cout << "done" << std::endl;
    } else {    
      std::cerr << "[Error] Unknown picture: " << $2 << std::endl;
    }
    free($2);
  }



// =================================================================================================
| TK_CMD_HSEUIL TK_VAR TK_FLOAT TK_SEMI
  {
    if (imgs.find($2) !=  imgs.end()) {
      std::cout << "Seuil :" << $3 << " : "
								<< imgs[$2]->Hseuil($3)
								<< std::endl;
    } else {    
      std::cerr << "[Error] Unknown picture: " << $2 << std::endl;
    }
    free($2);
  }
| TK_CMD_SEUILG TK_VAR TK_FLOAT TK_SEMI
  {
    if (imgs.find($2) !=  imgs.end()) {
      std::cout << "seuil global " << $3 << " ..." << std::flush;
			
			lenactions::image* tmp = new lenactions::image(imgs[$2]->seuil_global($3));
			delete imgs[$2];
			imgs[$2] = tmp;
			
      std::cout << "done" << std::endl;
    } else {    
      std::cerr << "[Error] Unknown picture: " << $2 << std::endl;
    }
    free($2);
  }
| TK_CMD_SEUILL TK_VAR TK_SEMI
  {
    if (imgs.find($2) !=  imgs.end()) {
      std::cout << "seuil local ..." << std::flush;
			
			lenactions::image* tmp = new lenactions::image(imgs[$2]->seuil_local());
			delete imgs[$2];
			imgs[$2] = tmp;
			
      std::cout << "done" << std::endl;
    } else {    
      std::cerr << "[Error] Unknown picture: " << $2 << std::endl;
    }
    free($2);
  }
| TK_CMD_SEUILH TK_VAR TK_FLOAT TK_FLOAT TK_SEMI
  {
    if (imgs.find($2) !=  imgs.end()) {
      std::cout << "seuil histeresis " << $3 << ", " << $4 << "..." << std::flush;
      
			lenactions::image* tmp = new lenactions::image(imgs[$2]->seuil_histerisis($3, $4));
			delete imgs[$2];
			imgs[$2] = tmp;
			
      std::cout << "done" << std::endl;
    } else {    
      std::cerr << "[Error] Unknown picture: " << $2 << std::endl;
    }
    free($2);
  }
| TK_CMD_AFFINAGE TK_VAR TK_SEMI
  {
    if (imgs.find($2) !=  imgs.end()) {
      std::cout << "affinage ..." << std::flush;
      
			lenactions::image* tmp = new lenactions::image(imgs[$2]->affinage());
			delete imgs[$2];
			imgs[$2] = tmp;
			
      std::cout << "done" << std::endl;
    } else {    
      std::cerr << "[Error] Unknown picture: " << $2 << std::endl;
    }
    free($2);
  }

| TK_CMD_CLOSEN TK_VAR TK_SEMI
  {
    if (imgs.find($2) !=  imgs.end()) {
      std::cout << "contours fermés ..." << std::flush;
      
			lenactions::image* tmp = new lenactions::image(imgs[$2]->close_naive());
			delete imgs[$2];
			imgs[$2] = tmp;
			
      std::cout << "done" << std::endl;
    } else {    
      std::cerr << "[Error] Unknown picture: " << $2 << std::endl;
    }
    free($2);
  }
| TK_CMD_CLOSEN TK_VAR TK_FLOAT TK_FLOAT TK_SEMI
  {
    if (imgs.find($2) !=  imgs.end()) {
      std::cout << "contours fermés " << $3 << ", " << $4 << " ..." << std::flush;
      
			lenactions::image* tmp = new lenactions::image(imgs[$2]->close_naive($3, $4));
			delete imgs[$2];
			imgs[$2] = tmp;
			
      std::cout << "done" << std::endl;
    } else {    
      std::cerr << "[Error] Unknown picture: " << $2 << std::endl;
    }
    free($2);
  }	
| TK_CMD_CLOSEW TK_VAR TK_SEMI
  {
    if (imgs.find($2) !=  imgs.end()) {
      std::cout << "contours fermés ..." << std::flush;
      
			lenactions::image* tmp = new lenactions::image(imgs[$2]->close_waves());
			delete imgs[$2];
			imgs[$2] = tmp;
			
      std::cout << "done" << std::endl;
    } else {    
      std::cerr << "[Error] Unknown picture: " << $2 << std::endl;
    }
    free($2);
  }
| TK_CMD_CLOSEW TK_VAR TK_FLOAT TK_FLOAT TK_SEMI
  {
    if (imgs.find($2) !=  imgs.end()) {
      std::cout << "contours fermés " << $3 << ", " << $4 << " ..." << std::flush;
      
			lenactions::image* tmp = new lenactions::image(imgs[$2]->close_waves($3, $4));
			delete imgs[$2];
			imgs[$2] = tmp;
			
      std::cout << "done" << std::endl;
    } else {    
      std::cerr << "[Error] Unknown picture: " << $2 << std::endl;
    }
    free($2);
  }	
;



%%

int main() {
	return yyparse();
}


