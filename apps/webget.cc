#include "address.hh"
#include "socket.hh"
#include "util.hh"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/socket.h>

#include <future>
#include <thread>
#include <chrono>

using namespace std;

void get_URL(const string &host, const string &path) {
    // Your code here.

    // You will need to connect to the "http" service on
    // the computer whose name is in the "host" string,
    // then request the URL path given in the "path" string.

    // Then you'll need to print out everything the server sends back,
    // (not just one call to read() -- everything) until you reach
    // the "eof" (end of file).

    Address addr(host, "http");
    TCPSocket http_tcp;
    http_tcp.connect(addr);
    http_tcp.write("GET " + path + " HTTP/1.1\r\n");
    http_tcp.write("HOST: " + host + "\r\n");
    http_tcp.write("Connection: close\r\n");
    http_tcp.write("\r\n");

    //http_tcp.shutdown(SHUT_RDWR);
    bool out = false;

    while(!out){
        std::future<void> future = std::async(std::launch::async, [&http_tcp](){
            cout << http_tcp.read(); 
        });

        std::future_status status;

        bool break_out = false;

        do {
            switch(status = future.wait_for(1s); status) {
                case std::future_status::deferred: break;
                case std::future_status::timeout: break_out = true; break;
                case std::future_status::ready: break;
            }

            if (break_out) {
                break;
            }
        } while (status != std::future_status::ready && !break_out);

        if (break_out) {
            out = true;
            http_tcp.close();
            exit(0);
        }
    }
}

int main(int argc, char *argv[]) {
    try {
        if (argc <= 0) {
            abort();  // For sticklers: don't try to access argv[0] if argc <= 0.
        }

        // The program takes two command-line arguments: the hostname and "path" part of the URL.
        // Print the usage message unless there are these two arguments (plus the program name
        // itself, so arg count = 3 in total).
        if (argc != 3) {
            cerr << "Usage: " << argv[0] << " HOST PATH\n";
            cerr << "\tExample: " << argv[0] << " stanford.edu /class/cs144\n";
            return EXIT_FAILURE;
        }

        // Get the command-line arguments.
        const string host = argv[1];
        const string path = argv[2];

        // Call the student-written function.
        get_URL(host, path);
    } catch (const exception &e) { 
        cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
