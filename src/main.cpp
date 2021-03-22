#include <stdio.h>
#include <fstream>
#include "scanner.h"
#include "parser.h"
int main(int argc, char** argv){
  if(argc < 2){
    printf("must past file, feather [file] ");
    return -1;
  }
  printf("using file %s \n", argv[1]);

  scanner scn;
  scn.open(argv[1]);
  
  parser par = parser(scn);
  auto v = par.create_ast_node();
  v->print();

  printf("res: %i \n", v->interpret());
  v->destroy();
  return 0;
}
