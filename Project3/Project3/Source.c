#include<stdio.h>
#include<netdb.h>
#include<stdlib.h>
#include<argp.h>
   


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
    

}