#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <tuple>
#include <fstream>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::make_tuple;
using std::get;
using std::tuple;
using std::stoi;
using std::ifstream;

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

    vector < string > w_network_raw;

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
                w_network_raw.push_back( line );
            }
        }
    }
    file.close();

    cout << w_network_raw[2] << endl;

    //int x = std::system("macvendor --no-update b4:82:fe:cf:12:28");
    
    return 0;
}