#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "stego.h"



int main (int argc, char **argv)
{
  Options *options = parse_options(argc, argv);

  if (options->mode == DISTRIBUTE){
    distribute_secret();
  }else{
    retrieve_secret();    
  }

}

void distribute_secret(){

}

void retrieve_secret(){

}