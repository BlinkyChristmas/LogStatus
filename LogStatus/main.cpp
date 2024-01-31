//Copyright © 2024 Charles Kerr. All rights reserved.
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>

#include "ErrorCollection.hpp"
#include "ClientCollection.hpp"
#include "ConnectionStatus.hpp"

using namespace std::string_literals ;

enum ConnectState {
    NEVER,CONNECTED,DISCONNECTED
};

auto printTable(std::ostream &output, ClientCollection &clients, ConnectionStatus &status, ErrorCollection &clienterrors) -> void ;
auto printTableRow (std::ostream &output, const std::string &client, ConnectState state, bool has_error) -> void ;

//======================================================================================================================
int main(int argc, const char * argv[]) {
    auto exitcode = EXIT_SUCCESS ;
    try{
        if (argc != 5) {
            throw std::runtime_error("Invalid # arguments, requires: clientpath  connectpath errorpath outputpath") ;
        }
        auto clientpath = std::filesystem::path(argv[1]) ;
        auto connectpath = std::filesystem::path(argv[2]) ;
        auto errorpath = std::filesystem::path(argv[3]) ;
        auto outputpath = std::filesystem::path(argv[4]) ;

        auto clientCollection = ClientCollection(clientpath) ;
        auto connectionStatus = ConnectionStatus(connectpath) ;
        auto errorCollection = ErrorCollection(errorpath) ;
        
        auto output = std::ofstream(outputpath.string()) ;
        if (!output.is_open()){
            throw std::runtime_error("Unable to create: "s + outputpath.string());
        }
        // start writing the document
        output << "<!DOCTYPE html>\n";
        output << "<html>\n";
        output << "<head>\n";
        output << "     <title> Blinky Show Client Status</title>\n";
        output << "</head>\n";
        output << "<body>\n";
        
    
        
        
        output << "</body>\n";
        output << "/html>\n";
        
    }
    catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
        exitcode = EXIT_FAILURE ;
    }
    catch(...) {
        std::cerr << "Unknown Error!" << std::endl;
        exitcode = EXIT_FAILURE ;
    }
    
    return 0;
}

// ===============================================================================================================================
auto printTable(std::ostream &output, ClientCollection &clients, ConnectionStatus &status, ErrorCollection &clienterrors) -> void {
    output << "\t<table>\n";
    output << "\t\t<caption>\n";
    output << "\t\t\tBlinky Show Client Status\n" ;
    output << "\t\t</caption>\n" ;
    output << "\t\t<thead>\n";
    output << "\t\t\t<th scope=\"col\">Client</th>\n";
    output << "\t\t\t<th style=\"text-align:center;\" scope=\"col\">Status</th>\n";
    output << "\t\t\t<th scope=\"col\">Error</th>\n";
    output << "\t\t</thead>\n";
    
    output << "\t\t<tbody>\n" ;
    
    for (const auto &client : clients.clients) {
        
        ConnectState state = ConnectState::NEVER ;
        if (status.contains(client.name)){
            state = ConnectState::DISCONNECTED ;
            if (status[client.name].last().state) {
                state = ConnectState::CONNECTED ;
            }
        }
        auto has_error = false ;
        if (clienterrors.contains(client.name)){
            has_error = true ;
        }
        
        // Now we print the entry ;
        printTableRow(output, client.name, state, has_error) ;
    }
    
    
    
    output << "\t\t</tbody>\n" ;
    output << "\t</table>\n" ;

}

// ==============================================================================================================
auto printTableRow (std::ostream &output, const std::string &client, ConnectState state, bool has_error) -> void {
    auto concolor = "magenta"s ;
    auto value = "NEVER"s ;
    switch(state) {
        case ConnectState::NEVER:
            concolor = "magenta"s;
            value = "NEVER"s ;
            break;
        case ConnectState::CONNECTED:
            concolor = "green"s;
            value = "Connected"s ;
            break;
        case ConnectState::DISCONNECTED:
            concolor = "black"s;
            value = "Disconnected"s;
            break;
    }
    output << "\t\t\t<tr style=\"text-align:center;\">\n" ;
    output << "\t\t\t\t<th scope=\"row\">"<<client<<"</th>\n";
    output << "\t\t\t\t<td style=\"color:"<<concolor<<";\">"<<value<<"</td>\n";
    output << "\t\t\t\t<td "<< (has_error? "style=\"color:red;\">":">") << (has_error?"Error":"")<<"</td>\n";
    output << "\t\t\t</tr>\n";
}
