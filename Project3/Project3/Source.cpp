#include<cstdio>
#include<netdb.h>
#include<getopt.h>
#include<iostream>
#include<string>   
#include<cstring>
#include<sstream>
#include<vector>

using namespace std;

vector<string> ip_addresses;
vector<int> ports;


/*
static struct argp_option options[] = {
  { "port",      'p', 0, 0, "List of ports to scan" },
  { "ip",        'i', 0, 0, "IP address to scan" },
  { "file",      'f', 0, 0, "Filename for list of ip addresses" },
  { "transport", 't', 0, 0, "TCP or UDP" }
}
static error_t parse_opt(int key, char *arg, struct argp_state* state) {
  
  struct arguments *args = state->input;
  
  switch ( key ) {
    case 'p': printf("\nport"); break;
    case 'i': printf("\nip"); break;
    case 'f': printf("\nfile"); break;
    case 't': printf("\ntransport"); break;
  }
  return 0;
}
*/


void printOptionVariables(char* optarg, int optopt, int optind, int longindex) {
  printf("\n\toptions: %s", optarg);
  printf("\n\toptopt: %s", optopt);
  printf("\n\toptind: %d", optind);
  printf("\n\tlongindex: %d", longindex);
}


static struct option long_options[] = 
{
  {"ports",     required_argument, NULL, 'p'},
  {"ip",        required_argument, NULL, 'i'},
  {"file",      required_argument, NULL, 'f'},
  {"transport", required_argument, NULL, 't'},
  {"help",      no_argument,       NULL, 'h'},
  {NULL, 0, NULL, 0}
};



int main(int argc, char** argv) {
    
  extern char *optarg;
  extern int optind, optopt;
  bool scanUDP = true;
  bool scanTCP = true;
  string options;
  string ip;
  string ports;
  string file;
  char* token;
  int c = 1;

  int longindex;
  while (c != -1) {
    
    c = getopt_long(argc, argv, "pifth", long_options, &longindex);

    switch (c) {
      case 'p':      
        printf("\n\nPort:");
        printOptionVariables(optarg, optopt, optind, longindex);      
        printf("\n\tport args: ");
        optind--;
        while (argv[optind][0] != '-') {

          token = strtok(argv[optind], ",");

          while (token != NULL) {
            printf("%s ", token);
            token = strtok(NULL, ",");
          }          
          optind++;
        }
        break;      
      case 'i':
        printf("\n\nIP:");
        printOptionVariables(optarg, optopt, optind, longindex);        
        break;
      case 'f':
        printf("\n\nIP file:");
        printOptionVariables(optarg, optopt, optind, longindex);
        break;
      case 't':
        printf("\n\nTransport option: %s");
        printOptionVariables(optarg, optopt, optind, longindex);
        break;
      case 'h':
        printf("\nUsage:"
                "\n\t--help -h <display invocation options>"
                "\n\t--port -p <ports to scan>"
                "\n\t--ip -i <IP address to scan>"
                "\n\t--file -f <file name containing IP addresses to scan>"
                "\n\t--transport -t <TCP or UDP>");
        break;
      case '?':
        printf("\n?: %s", optarg);
        break;
      case ':':
        printf("\n:: %s", optarg);
        break;
      case 1:
        printf("\n1: %s", optarg);
        break;
      case 0:
        printf("\n0: %s", optarg);
        break;
      default:
        printf("\n\n%s: option '-%c' is invalid: ignored\n", argv[0], optarg);
        break;      
    }
  }

  scanf("%d", &c);
  /*
  for (int i = 1; i < argc; ++i){
    printf("\ncmd options %d: %s", i, argv[i]);
    options += string(argv[i]) + " ";
  }

  printf("\n\nOptions variable: %s", options.c_str());

  vector<string> tokens;
  string tmp{};

  istringstream iss(options);

  printf("\n");
  while (getline(iss, tmp, '-')) {

    printf("\nPushing %s to tokens vector...", tmp.c_str());
      tokens.push_back(tmp);
  }
  printf("\n");
  for (auto& token : tokens) {
      printf("%s\n", token.c_str());
  }
  */
  return 0;
}