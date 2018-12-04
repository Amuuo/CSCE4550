#include<cstdio>
#include<netdb.h>
#include<getopt.h>
#include<iostream>
#include<string>   
#include<cstring>
#include<sstream>
#include<vector>
#include<algorithm>
#include<fstream>
#include<set>

using namespace std;

void printOptionVariables(char*, int, int, int);
void parse_cmd_options(int, char**);


vector<string> ip_addresses;     // vector of ip addresses to scan
set<int>       ports;            // vector of ports to scan
bool           tcp_only{false};  // bool indicating to scan tcp only
bool           udp_only{false};  // bool indicating to scan udp only



/*======================
           MAIN
 ======================*/
int main(int argc, char** argv) {
  
  parse_cmd_options(argc, argv);
      
  int c;

  scanf("%d", &c);

  return 0;
}





/*======================
    PARSE CMD OPTIONS
 ======================*/
void parse_cmd_options(int argc, char** argv) {

  static struct option long_options[] =
  {
    {"ports",     required_argument, NULL, 'p'},
    {"ip",        required_argument, NULL, 'i'},
    {"file",      required_argument, NULL, 'f'},
    {"transport", required_argument, NULL, 't'},
    {"help",      no_argument,       NULL, 'h'},
    {NULL, 0, NULL, 0}
  };


  extern char *optarg;
  extern int  optind;
  extern int  optopt;
  string      ip;
  string      file;
  
  int c = 1;

  int longindex;
  while (c != -1) {

    c = getopt_long(argc, argv, "pifth", long_options, &longindex);

    switch (c) {

      /*
       * PORTS OPTION
       */
      case 'p':
      {
        printf("\n\nPort:");
        printOptionVariables(optarg, optopt, optind, longindex);
        char* token;

        printf("\n\tport args: ");
        optind--;

        /*
         * check all subsequent options that don't begin with hyphen
         */
        while (argv[optind][0] != '-') {

          token = strtok(argv[optind], ",");
          string portArg{token};
          string tmp;

          /*
           * check if ports options contains a hyphen
           */
          auto result = find(portArg.begin(), portArg.end(), '-');

          /*
           * CONTAINS HYPHEN: add ports in range to ports vector
           */
          if (result != portArg.end()) {

            istringstream iss{portArg};

            getline(iss, tmp, '-');
            int beginRange = stoi(tmp);
            iss >> tmp;

            int endRange = stoi(tmp);
            printf("\n\tBegin Range: %d", beginRange);
            printf("\n\tEnd Range: %d", endRange);

            for (int i = beginRange; i < endRange; ++i)
              ports.insert(i);

            /*
             * DOESN'T CONTAIN HYPEHN: add port to vector
             */
          }
          else {
            while (token != NULL) {
              printf("%s ", token);
              ports.insert(atoi(token));
              token = strtok(NULL, ",");
            }
          }
          optind++;
        }
        printf("\n\tPorts set: ");
        for (auto& p : ports)
          printf("%d, ", p);

        break;
      }

      /*
       * IP OPTION
       */
      case 'i':
      {
        printf("\n\nIP:");
        printOptionVariables(optarg, optopt, optind, longindex);
        printf("\n");
        char* token;

        --optind;
        /*
         * check all subsequent options that don't begin with hyphen
         */
        while (argv[optind][0] != '-') {

          token = strtok(argv[optind], ",");

          /*
           * push all ip addresses to ip_addresses vector
           */
          while (token != NULL) {
            ip_addresses.push_back(token);
            token = strtok(NULL, ",");
          }
          optind++;
        }

        printf("\n\nIP Vector: ");

        for (auto& i : ip_addresses)
          printf("\n\t%s", i.c_str());

        break;
      }

      /*
       * IP FILE OPTION
       */
      case 'f':
      {
        printf("\n\nIP file:");
        printOptionVariables(optarg, optopt, optind, longindex);
        
        ifstream ip_file{optarg};        
        
        if (ip_file.fail()) {
          fprintf(stderr, "\n\nERROR: IP file failed to open... exiting...");
          return;
        }

        string ip_string;

        while (getline(ip_file, ip_string))
          ip_addresses.push_back(ip_string);

        printf("\n\nIP address set: ");
        
        for (auto& i : ip_addresses)
          printf("\n\t%s", i.c_str());
        
        break;
      }

      /*
       * TRANSPORT OPTION
       */
      case 't':
        printf("\n\nTransport option: %s");
        printOptionVariables(optarg, optopt, optind, longindex);
        break;

        /*
         * HELP OPTION
         */
      case 'h':
        printf("\nUsage:"
               "\n\t--help -h <display invocation options>"
               "\n\t--port -p <ports to scan>"
               "\n\t--ip -i <IP address to scan>"
               "\n\t--file -f <file name containing IP addresses to scan>"
               "\n\t--transport -t <TCP or UDP>");
        break;

      case '?':        
        printf("\n\n?: %s", optarg);
        printf("\n\tUnidentified option: %s", argv[--optind]);
        ++optind;
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
}




/*=======================
  PRINT OPTION VARIABLES
 =======================*/
void printOptionVariables(char* optarg, int optopt, int optind, int longindex) {
  
  printf("\n\toptions: %s", optarg);
  printf("\n\toptopt: %s", optopt);
  printf("\n\toptind: %d", optind);
  printf("\n\tlongindex: %d", longindex);
}