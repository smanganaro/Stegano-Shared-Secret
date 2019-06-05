#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "stego.h"

void print_help();

Options * parse_options(int argc, char **argv){

  Options * parameters = calloc(1, sizeof(Options));

  int c;
  
  static int mode_flag = -1;
  static int min_shadows_amount = -1;
  static int total_amount_of_shadows = -1;

  while (1)
    {
      static struct option long_options[] =
        {
          {"dir",  required_argument, 0, 'x'},
          {"des",  no_argument, 0, 'x'}, //RANDOM FIX
          {0, 0, 0, 0}
        };
      /* getopt_long stores the option index here. */
      int option_index = 0;

      c = getopt_long_only (argc, argv, "hdrs:m:k:n:",
                       long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
        break;

      switch (c)
        {
        case 0:
          /* If this option set a flag, do nothing else now. */
          if (long_options[option_index].flag != 0)
            break;
          printf ("option %s", long_options[option_index].name);
          if (optarg)
            printf (" with arg %s", optarg);
          printf ("\n");
          break;

        case 'h':
          print_help();
          exit(0);
          break;

        case 'd':
          mode_flag=0;
          break;

        case 'r':
          mode_flag=1;
          break;

        case 's':
          parameters->secret_file_name = optarg;
          break;

        case 'm':
          parameters->water_mark_file_name = optarg;
          break;

        case 'k':
          parameters->min_shadows_amount = atoi(optarg);
          break;

        case 'n':
          parameters->total_amount_of_shadows = atoi(optarg);
        break;

        case 'x':
          parameters->directory = optarg;
        break;


        case '?':
          /* getopt_long already printed an error message. */
          break;

        default:
          abort ();
        }
    }

    if (parameters->secret_file_name == NULL)
    {
        fprintf(stderr, "You must specify the secret file.\n");
        free(parameters);
        exit(1);
    }

    
   if (parameters->water_mark_file_name == NULL)
    {
        fprintf(stderr, "You must specify the watermark file.\n");
        free(parameters);
        exit(1);
    }

    if (parameters->min_shadows_amount == -1)
    {
        fprintf(stderr, "You must specify a minimum amount of shadows needed to retrieve a secret.\n");
        free(parameters);
        exit(1);
    }

    if (parameters->total_amount_of_shadows == -1)
    {
        fprintf(stderr, "You must specify total amount of shadows in which the secret is distributed.\n");
        free(parameters);
        exit(1);
    }

    if (parameters->water_mark_file_name == NULL)
    {
        fprintf(stderr, "You must specify the path of the directory for the secret file.\n");
        free(parameters);
        exit(1);
    }

    parameters->mode = mode_flag;

    if (!(parameters->mode == DISTRIBUTE || parameters->mode == RETRIEVE))
    {
        fprintf(stderr, "You must specify a valid execution mode.\n");
        free(parameters);
        exit(1);
    }

    return parameters;
}


/**
 * Prints help
 */
void print_help()
{
    printf("Usage: stegosharedsecret [OPTIONS]\n");
    printf("Options:\n");
    printf("%-30s", "\t-h");
    printf("prints the help and quits.\n");
    printf("%-30s", "\t-d ");
    printf("selects distribution mode.\n");
    printf("%-30s", "\t-r ");
    printf("selects retrieve mode.\n");
    printf("%-30s", "\t-dir path");
    printf("if -d, path to where you want to save the new images. if -r, path to where the images with the hidden secret are. \n");
    printf("%-30s", "\t-k number");
    printf("minimum amount of shadows to retrieve a secret. \n");
    printf("%-30s", "\t-n number");
    printf("total amount of shadows in which the secret will be distributed.\n");
    printf("%-30s", "\t-s image");
    printf("if -d, name of the input .bmp file. if -r, name of the output .bmp file.\n");
    printf("%-30s", "\t-m image");
    printf("if -d, name of the watermark input .bmp file. if -r, name of the watermark output .bmp file \n");
    printf("%-30s", "\t-pass password");
}
