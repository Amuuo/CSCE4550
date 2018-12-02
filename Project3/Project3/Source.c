#include<stdio.h>
#include<netdb.h>
#include<stdlib.h>
#include<argp.h>
   


const char *argp_program_version =
"argp-ex3 1.0";
const char *argp_program_bug_address =
"<bug-gnu-utils@gnu.org>";


static char doc[] =
"Argp example #3 -- a program with options and arguments using argp";

static char args_doc[] = "port ip file transport";


struct arguments {
  char * args[4];
  int port, ip, file, transport;
  char *output_file;
};

static struct argp_option options[] = {
  { "port",      'p', 0, 0, "List of ports to scan" },
  { "ip",        'i', 0, 0, "IP address to scan" },
  { "file",      'f', 0, 0, "Filename for list of ip addresses" },
  { "transport", 't', 0, 0, "TCP or UDP" }
};


static error_t parse_opt(int key, char *arg, struct argp_state* state) {
  
  struct arguments *args = state->input;
  
  switch ( key ) {
    case 'p': printf("\nport"); break;
    case 'i': printf("\nip"); break;
    case 'f': printf("\nfile"); break;
    case 't': printf("\ntransport"); break;
    default: return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };



int main(int argc, char** argv) {
    
  int i;
  struct arguments arguments;
  
  arguments.file = 0;
  arguments.ip = 0;
  arguments.port = 0;
  arguments.transport = 0;
  arguments.output_file = "--";


  argp_parse(&argp, argc, argv, 0, 0, &arguments);    

  for ( i = 0; i < 4; ++i ) {
    printf("%s", arguments.args[i]);
  }
    
  char buff[10];
  scanf("%s", buff);
}