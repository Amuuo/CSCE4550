#include<cstdio>
#include<netdb.h>
#include<iostream>
#include<string>   
#include<cstring>
#include<sstream>
#include<vector>

using namespace std;

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



int main(int argc, char** argv) {
    
    string options;
    
    for(int i = 1; i < argc; ++i){
        options += string{argv[i] + ' '};
    }

    vector<string> tokens;
    istringstream iss(options);
    string tmp;


    while(getline(iss, tmp, '-')) {
        printf("\nPushing %s to tokens vector...", tmp.c_str());
        tokens.push_back(tmp);
    }

    for(auto& token : tokens) {
        printf("%s\n", token.c_str());
    }

}