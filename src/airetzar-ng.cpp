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

    cout << airodump_csv << " " << interface << endl;

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
        line = trim(line);
        if(first){
            if (line.find("BSSID") != string::npos) {
                first = false;
            }
        }else{
            if( line == "" ){
                break;
            }else{
                w_network_raw.push_back( line );
            }
        }
    }
    file.close();

    cout << w_network_raw.size() << endl;

    //int x = std::system("macvendor --no-update b4:82:fe:cf:12:28");
    
    return 0;
}

string trim(const string& str){

    size_t first = str.find_first_not_of(' ');
    if (string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}