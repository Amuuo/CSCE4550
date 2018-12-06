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
#include<sys/time.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<netinet/ip_icmp.h>
#include<arpa/inet.h>
#include<sys/select.h>
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
bool           display_closed_ports{false};

int            tcp_sock;
int            udp_sock;

struct sockaddr_in  tmp_in { 0 };
struct servent*     tmp_servent;
struct timeval      tmp_time{ 1, 0 };


fd_set file_set;


/*======================
           MAIN
 ======================*/
int main(int argc, char** argv) {
  
  char bytes[65535];
  struct addrinfo hints;
  struct addrinfo* serv_addr;
  int return_time;
  int send_size;
  int return_size;

  parse_cmd_options(argc, argv);
  memset(&tmp_in, 0, sizeof(tmp_in));
  tmp_in.sin_family = AF_INET;
      
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
    
    cout << endl << "\n\tIP address: " << ip << endl;
    cout << endl << setw(8) << left << "\tPort";
    cout << setw(10) << left << "Status";
    cout << setw(15) << left << "Service";
    cout << setw(10) << left << "Protocol" << "\n" << endl;
    
    //tmp_in.sin_addr.s_addr  = inet_addr(ip.c_str());        
    
    
    for (auto& port : ports) {

      bzero(&tmp_in, sizeof(tmp_in));
      tmp_in.sin_addr.s_addr = inet_addr(ip.c_str());
      tmp_in.sin_family = AF_INET;
      tmp_in.sin_port = htons(port);
      
      
      /*
       *  TCP SCAN
       */
      if (!udp_only) {    
        
        if ((tcp_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
          cerr << "tcp socket failed: " << errno << endl;

        tmp_servent = getservbyport(htons(port), "tcp");

        /*
         * connect
         */
        if ((connect(tcp_sock, (struct sockaddr*)&tmp_in, sizeof(struct sockaddr_in))) < 0) {
          if (display_closed_ports && tmp_servent) {
            cout << "\t" << setw(7) << left << port;
            cout << setw(10) << left << "closed";
            cout << setw(15) << left << tmp_servent->s_name;
            cout << setw(7) << left << "TCP" << endl;
          }
        }
        else {                    
          cout << "\t" << setw(7) << left << port;
          cout << setw(10) << left << "open";
          cout << setw(15) << left << (tmp_servent ? tmp_servent->s_name : "**unknown");
          cout << setw(7) << left << "TCP" << endl;          
        }
        shutdown(tcp_sock, SHUT_RDWR); 
        close(tcp_sock);
      }

      /*
       * UDP SCAN
       */
      if (!tcp_only) {

        if ((udp_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
          cerr << "\tudp socket failed: " << errno << ", exiting..." << endl;
          exit(1);
        }
        memset(bytes, 0, sizeof(bytes));

        /*
         * connect
         */
        if((connect(udp_sock, (struct sockaddr*)&tmp_in, sizeof(tmp_in))) < 0){
          cout << ", connect failed, errno: " << errno << ", exiting..." << endl;
          exit(1);
        }                             
        /*
         * send
         */
        if((sendto(udp_sock, NULL, NULL, 0, (sockaddr*)&tmp_in, sizeof(tmp_in))) < 0) {
          cerr << ", sendto failed, errno: " << errno << ", exiting..." << endl;
          exit(1);
        }                  

        FD_ZERO(&file_set);
        FD_SET(udp_sock, &file_set);
        tmp_servent = getservbyport(htons(port), "udp");

        if ((return_size = select(udp_sock + 1, &file_set, NULL, NULL, &tmp_time)) < 0) {
          cerr << "\tselect failed, errno: " << errno << ", exiting..." << endl;
          exit(1);
        }
        else if (return_size == 0/*!FD_ISSET(udp_sock, &file_set)*/) {          
          cout << "\t" << setw(7) << left << port;
          cout << setw(10) << left << "open";
          cout << setw(15) << left << (tmp_servent ? tmp_servent->s_name : "**unknown");
          cout << setw(7) << left << "UDP" << endl;
        }                               
        else {          
          if (tmp_servent && display_closed_ports) {
            cout << "\t" << setw(7) << left << port;
            cout << setw(10) << left << "closed";
            cout << setw(15) << left << tmp_servent->s_name;
            cout << setw(7) << left << "UDP" << endl;
          }          
        }
        shutdown(udp_sock, SHUT_RDWR);        
        close(udp_sock);
      }
    }    
  }
  cout << endl << endl;

  return 0;
}





/*======================
    PARSE CMD OPTIONS
 ======================*/
void parse_cmd_options(int argc, char** argv) {

  static struct option long_options[] =
  {
    {"ports",          required_argument, NULL, 'p'},
    {"ip",             required_argument, NULL, 'i'},
    {"file",           required_argument, NULL, 'f'},
    {"transport",      required_argument, NULL, 't'},
    {"help",           no_argument,       NULL, 'h'},
    {"display-closed", no_argument,       NULL, 'd'},
    {NULL, 0, NULL, 0}
  };

  extern char *optarg;
  extern int  optind;  
  string      ip;
  string      file;  
  int         cmd_option;

  while ((cmd_option = getopt_long(argc, argv, "p:i:f:t:hd", long_options, NULL)) != -1) {
    
    switch (cmd_option) {

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
            "\n\t--help -h <display cmdline options>"
            "\n\t--port -p <ports to scan>"
            "\n\t--ip -i <IP address to scan>"
            "\n\t--file -f <file name containing IP addresses to scan>"
            "\n\t--transport -t <TCP or UDP>"
            "\n\t--display-closed -d display open and closed ports"
          << endl;
        break;
      
      /*
      * DISPLAY CLOSED PORTS OPTION
      */
      case 'd':
        display_closed_ports = true;
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
