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
int            tcp_sock;
int            udp_sock;
int            icmp_sock;

struct sockaddr_in  tmp_in { 0 };
struct servent*     tmp_servent;
struct protoent*    tmp_protoent;
struct netent*      tmp_netent;
struct hostent*     tmp_hostent;
struct ip*          ip_struct;
struct icmp*        tmp_icmp;


fd_set fds;


/*======================
           MAIN
 ======================*/
int main(int argc, char** argv) {
  
  char bytes[1024];
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
    
    cout << endl << "\n\n\tIP address: " << ip << endl;
    cout << endl << setw(8) << left << "\tPort";
    cout << setw(10) << left << "Status";
    cout << setw(15) << left << "Service";
    cout << setw(10) << left << "Protocol" << "\n" << endl;
    
    tmp_in.sin_addr.s_addr  = inet_addr(ip.c_str());        
    //tmp_in.sin_addr.s_addr = INADDR_ANY;
      
    


    for (auto& port : ports) {

      tmp_in.sin_port  = htons(port);              
      
      if (!udp_only) {    
        
        if ((tcp_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
          cerr << "tcp socket failed: " << errno << endl;
        
        
        
        if (tmp_servent == NULL) {
          shutdown(tcp_sock, SHUT_RDWR);
          close(tcp_sock);
          goto udp_scan;
        }

        //cout << "\t" << setw(7) << left << port;
        
        if ((connect(tcp_sock, (struct sockaddr*)&tmp_in, sizeof(struct sockaddr_in))) < 0) {
          //cout << setw(10) << left << "closed";                  
        }
        else {
          tmp_servent = getservbyport(htons(port), "tcp");
          if (tmp_servent) {
            cout << "\t" << setw(7) << left << port;
            cout << setw(10) << left << "open";
            cout << setw(15) << left << tmp_servent->s_name;
            cout << setw(7) << left << "TCP" << endl;
          }
        }
                
        

        shutdown(tcp_sock, SHUT_RDWR); 
        close(tcp_sock);
      }

      udp_scan:

      if (!tcp_only) {

        

        if ((udp_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
          cerr << "\tudp socket failed: " << errno << ", exiting..." << endl;
          exit(1);
        }
        /*if ((icmp_sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
          cerr << "\tudp socket failed: " << errno << ", exiting..." << endl;
          exit(1);
        }*/
        
        memset(bytes, 0, sizeof(bytes));

        int send_size;
        /*if((connect(udp_sock, (struct sockaddr*)&tmp_in, sizeof(tmp_in))) < 0){
          cout << ", connect failed, errno: " << errno << ", exiting..." << endl;
          exit(1);
        }*/
        //else {
          
        bzero(&tmp_in, sizeof(tmp_in));
        tmp_in.sin_family = AF_INET;
        tmp_in.sin_port = htons(port);
        tmp_in.sin_addr.s_addr = inet_addr(ip.c_str());

        if (sendto(udp_sock, bytes, sizeof(bytes), 0, (struct sockaddr*)&tmp_in, sizeof(tmp_in)) < 0) {
          cerr << ", sendto failed, errno: " << errno << ", exiting..." << endl;
          exit(1);
        }

        struct timeval tmp_time;
        tmp_time.tv_sec = 1;
        tmp_time.tv_usec = 0;
          
        while (1) {
          FD_ZERO(&fds);
          FD_SET(udp_sock, &fds);
          FD_SET(udp_sock+1, &fds);

          socklen_t d = sizeof(tmp_in);

          if (select(udp_sock+1, &fds, NULL, NULL, &tmp_time) > 0) {
            recvfrom(udp_sock, bytes, sizeof(bytes), 0x0, NULL, NULL);          
          }
          else if (!FD_ISSET(udp_sock, &fds)) {
            tmp_servent = getservbyport(htons(port), "udp");
            if (tmp_servent) {
              cout << "\t" << setw(7) << left << port;
              cout << setw(10) << left << "open";
              cout << setw(15) << left << tmp_servent->s_name;
              cout << setw(7) << left << "UDP" << endl;                
            }
            break;
          }
          else {
            cerr << "\tselect failed, errno: " << errno << ", exiting..." << endl;
            exit(1);
          }
          /*
          else if (return_time == 0) {
            cout << ", timeout..." << endl;
            //shutdown(tcp_sock, SHUT_RDWR);
            //close(udp_sock);
            //continue;
          }*/
          struct ip* ip_header = (struct ip*)bytes;
          int ip_length = ip_header->ip_hl << 2;
          struct icmp* tmp_icmp = (struct icmp*)(bytes + ip_length);

          if ((tmp_icmp->icmp_type == ICMP_UNREACH) && (tmp_icmp->icmp_code == ICMP_UNREACH_PORT)) {
            cout << "udp port unreachable";
            break;
          }          
        }
        shutdown(udp_sock, SHUT_RDWR);
        close(udp_sock);
      }
    }    
  }

  //close(tcp_sock);
  //close(udp_sock);
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
