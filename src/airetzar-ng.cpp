#include "airetzar-ng.hpp"

int main(int argc, const char* argv[]){

    string airodump_csv, interface = "";

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
            }
            toggle_flag = true;
        }
    }

    cout << airodump_csv << " " << interface << endl;

    //int x = std::system("macvendor --no-update b4:82:fe:cf:12:28");
    
    return 0;
}