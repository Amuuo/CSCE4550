/*
============================================================================
Name        : Project 3
Author      : Adam Williams
Version     : 1.0
Copyright   : 2018
Description : Program scans specified ports of given ip addresses
============================================================================
*/


#include<cstdio>
#include<cstdlib>
#include<netdb.h>
#include<cerrno>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<getopt.h>
#include<iostream>
#include<string>   
#include<cstring>
#include<sstream>
#include<vector>
#include<algorithm>
#include<fstream>
#include<set>
#include<iomanip>
#include<unistd.h>

using namespace std;

void parse_cmd_options(int, char**);


vector<string> ip_addresses;     // set of ip addresses to scan
set<uint16_t>  ports;            // set of ports to scan
bool           tcp_only{false};  // bool indicating to scan tcp only
bool           udp_only{false};  // bool indicating to scan udp only
int            tcp_sock;
int            udp_sock;

struct sockaddr_in  tmp_in { 0 };
struct sockaddr_in  tmp_in2 { 0 };
struct servent*     tmp_servent;
struct protoent*    tmp_protoent;
struct netent*      tmp_netent;
struct hostent*     tmp_hostent;


/*======================
           MAIN
 ======================*/
int main(int argc, char** argv) {
  
  char bytes[16];
  struct addrinfo hints;
  struct addrinfo* serv_addr;
  char host[128];
  char service[128];


  parse_cmd_options(argc, argv);
  tmp_in.sin_family = AF_INET;
  tmp_in2.sin_family = AF_INET;
      
  //setservent(1);

  
  /*
  if (!udp_only) 
    if ((tcp_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
      cerr << "Host down" << endl;
  if (!tcp_only) 
    if ((udp_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
      cerr << "Host down" << endl;
  */

  for (auto& ip : ip_addresses) {    
    
    cout << endl << "\n\n\tIP address: " << ip << endl;
    cout << endl << setw(8) << left << "\tPort";
    cout << setw(10) << left << "Status";
    cout << setw(15) << left << "Service";
    cout << setw(10) << left << "Protocol" << "\n" << endl;
    
    tmp_in.sin_addr.s_addr  = inet_addr(ip.c_str());    
    tmp_in2.sin_addr.s_addr = inet_addr(ip.c_str());

    if(!udp_only)
      
    
    if(!tcp_only)
      if (udp_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) == -1)
        cerr << "Host down" << endl;


    for (auto& port : ports) {

      tmp_in.sin_port  = htons(port);              
      tmp_in2.sin_port = htons(port);
      
      if (!udp_only) {    
        
        if (tcp_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) == -1)
          cerr << "Host down" << endl;
        tmp_servent = getservbyport(htons(port), "tcp");
        cout << "\t" << setw(7) << left << port;
                
        if (connect(udp_sock, (struct sockaddr*)&tmp_in, sizeof(tmp_in)) < 0)
          cout << setw(10) << left << "closed";                  
        else 
          cout << setw(10) << left << "open";
                
        cout << setw(15) << left << (tmp_servent ? tmp_servent->s_name : "unknown");         
        cout << setw(7) << left << "tcp" << endl;

        close(tcp_sock);
      }
      
      if (!tcp_only) { 
        
        tmp_servent = getservbyport(htons(port), "udp");
        cout << "\t" << setw(7) << left << port;
                
        if (connect(udp_sock, (struct sockaddr*)&tmp_in, sizeof(tmp_in)) < 0)
          cout << setw(10) << left << "closed";        
        else                    
          cout << setw(10) << left << "open";        
                
        cout << setw(15) << left << (tmp_servent ? tmp_servent->s_name : "unknown");
        cout << setw(7) << left << "udp" << endl;
      }      
    }    
  }

  close(tcp_sock);
  close(udp_sock);
  cout << endl << endl;

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
  string      ip;
  string      file;  
  int         c;

  while ((c = getopt_long(argc, argv, "p:i:f:t:h", long_options, NULL)) != -1) {
    
    switch (c) {

      /*
       * PORTS OPTION
       */
      case 'p':
      {        
        int currentIndex = optind - 1;
        char* token;
        
        cout << "\n\tPort args: " << endl;
        
        /*
         * check all subsequent options that don't begin with hyphen
         */
        while (currentIndex < argc && argv[currentIndex][0] != '-') {

          token = strtok(argv[currentIndex], ",");
          
          string portArg{token};
          string tmp;
          cout << "\t  " << token << endl;
          
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
            int beginRange{stoi(tmp)};
            iss >> tmp;
            int endRange{stoi(tmp)};
            
            cout << endl;            

            for (int i = beginRange; i < endRange+1; ++i)
              ports.insert(i);
          }
          /*
           * DOESN'T CONTAIN HYPEHN: add port to vector
           */
          else {            
            while (token != NULL) {              
              ports.insert(atoi(token));
              token = strtok(NULL, ",");
            }
          }
          currentIndex++;
        }

        cout << endl;
        break;
      }

      /*
       * IP OPTION
       */
      case 'i':
      {        
        int currentIndex = optind-1;
        char* token;
        
        /*
         * check all subsequent options that don't begin with hyphen
         */
        while (currentIndex < argc && argv[currentIndex][0] != '-') {

          token = strtok(argv[currentIndex], ",");

          /*
           * push all ip addresses to ip_addresses vector
           */
          while (token != NULL) {
            ip_addresses.push_back(token);
            token = strtok(NULL, ",");
          }
          currentIndex++;
        }

        cout << "\n\tIP Set: " << endl;

        for (auto& i : ip_addresses)
          cout << "\t  " << i << endl;

        break;
      }

      /*
       * IP FILE OPTION
       */
      case 'f':
      {        
        cout << "\tIP file:" << endl;
        
        ifstream ip_file{optarg};        
        
        if (ip_file.fail()) {
          cerr << "\nERROR: IP file failed to open... exiting..." << endl;
          return;
        }

        string ip_string;

        while (getline(ip_file, ip_string))
          ip_addresses.push_back(ip_string);

        cout << "\nIP address set: " << endl;
        
        for (auto& i : ip_addresses)
          cout << "\t" << i << endl;
        
        break;
      }

      /*
       * TRANSPORT OPTION
       */
      case 't':
      {
        cout << "\tTransport options: " << optarg;
        string transport_arg = optarg;

        transform(transport_arg.begin(),
                  transport_arg.end(),
                  transport_arg.begin(),
                  ::tolower);

        if (transport_arg == "tcp")
          tcp_only = true;

        else if (transport_arg == "udp")
          udp_only = true;

        else
          cerr << "Unknown transport option" << endl;
        
        break;
      }
      /*
       * HELP OPTION
       */
      case 'h':
        cout << 
          "\nUsage:"
            "\n\t--help -h <display invocation options>"
            "\n\t--port -p <ports to scan>"
            "\n\t--ip -i <IP address to scan>"
            "\n\t--file -f <file name containing IP addresses to scan>"
            "\n\t--transport -t <TCP or UDP>" 
          << endl;
        break;

      /*
       * INVALID OPTION
       */
      case '?':        
        cout << "\n\n?: " << optarg;
        cout << "\n\tUnidentified option: " << argv[--optind];
        ++optind;
        break;
    }
  }
}
