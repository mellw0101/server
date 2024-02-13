#include <cstdlib>
#include <iostream>
// #include <istream>
// #include <sstream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
// #include <string.h>


using namespace std;

int main() {
    auto __fatal_error__= [&](string s)-> int {
        string err_msg(" - \033[31mERROR\033[0m: " + s);
        perror(err_msg.c_str());
        return 1;
    };

    string __ip_address_("192.168.0.11");
    int __sock(0);
    uint16_t __port(8001);
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    const char *__prompt(" --> : ");
    string __msg_str_("");

    auto __init_socket__ =[&]()-> void {
        if((__sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            cout << "\n Socket creation error \n";
            exit(__sock);
        }
    };

    auto __convert_IPv4__=[&]()-> void {
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(__port);
        // Convert IPv4 and IPv6 addresses from text to binary form
        if(inet_pton(AF_INET, __ip_address_.c_str(), &serv_addr.sin_addr)<= 0) {
            __fatal_error__("\nInvalid address/ Address not supported \n");
            exit(-1);
        }
    };

    auto __connect__=[&]()-> void {
        if(connect(__sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            __fatal_error__("\nConnection Failed \n");
        }
    };
    
    auto __get_input__=[&]()-> void {
        __msg_str_.clear();
        cout << __prompt;
        cin.clear();
        getline(cin, __msg_str_);
        __msg_str_ += '\0';
    };

    auto __send_data__=[&]()-> void {
        int __status = send(__sock, __msg_str_.c_str(), __msg_str_.length(), 0);
        if(__status < 0) {
            cout << "could not send message\n";
            exit(-1);
        }
    };

    while(true) {
        __init_socket__();
        __convert_IPv4__();
        __connect__();
        __get_input__();
        __send_data__();

        // if(send(__sock, __msg_str_.c_str(), __msg_str_.length(), 0) < 0) {
        //     cout << "could not send message\n";
        // }
    }


    // Close the socket
    close(__sock);
    return 0;
}