%{
  #include <iostream>
	#include <string>
	#include "parser.hh"
	
	int line = 1;
	
%}

float           [+-]?[0-9]*(\.[0-9]*)?
format          P[1-6]
word            [A-Za-z0-9\-\_+=\!\$\#\%\&\*\^\@@\\\~\.]+
path            ((\/)?{word})+
var             ${word}
string          \".*\"
comment					^[#;].*[\n] 

%%

[ \t\r]					{ break;                                                    }
[\n]						{ line++; break;                                            }
";"           	{ return TK_SEMI;                                           }

"quit"        	{ return TK_CMD_QUIT;                                       }
"echo"        	{ return TK_CMD_ECHO;                                       }
"list"        	{ return TK_CMD_LIST;                                       }
"load"        	{ return TK_CMD_LOAD;                                       }
"export"      	{ return TK_CMD_EXPORT;                                     }
"copy"       		{ return TK_CMD_COPY;                                       }
"delete"     		{ return TK_CMD_DELETE;                                     }
"smooth"				{	return TK_CMD_SMOOTH;																			}
"prewitt"  		 	{ return TK_CMD_PREWITT;                                    }
"sobel"    		 	{ return TK_CMD_SOBEL;                                      }
"kirsch"   		 	{ return TK_CMD_KIRSCH;                                     }

"Hseuil"				{ return TK_CMD_HSEUIL;                                     }
"seuil_G"				{ return TK_CMD_SEUILG;                                     }
"seuil_L"				{ return TK_CMD_SEUILL;                                     }
"seuil_H"				{ return TK_CMD_SEUILH; 	                                	}
"affinage"			{ return TK_CMD_AFFINAGE;                                   }
"close_naive"		{ return TK_CMD_CLOSENAIVE;                                 }
"close_wave"		{ return TK_CMD_CLOSEWAVE;                                  }
"hough_line"		{ return TK_CMD_HOUGHLINE;                                  }
"hough_circle"	{ return TK_CMD_HOUGHCIRCLE;                                }

{float}       	{ yylval.floating = atof(yytext);       return TK_FLOAT;    }
{format}      	{ yylval.integer = (yytext[1] - '0');   return TK_FORMAT;   }
{var}         	{ yylval.string = strdup(&(yytext[1])); return TK_VAR;      }
{path}        	{ yylval.string = strdup(yytext);       return TK_PATH;     }
{string}      	{ yylval.string = strdup(yytext);       return TK_STRING;   }

{comment}				{ break;																										}

.								{ std::cout << "[LEXER] Syntax error line : "
														<< line
														<< std::endl;
									exit(1);                                                  }


%%

