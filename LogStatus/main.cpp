//Copyright © 2024 Charles Kerr. All rights reserved.
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <thread>
#include <vector>
#include <sstream>

#include "ErrorCollection.hpp"
#include "ClientCollection.hpp"
#include "ConnectionStatus.hpp"
#include "ServerStatus.hpp"
#include "utility\timeutil.hpp"
#include "utility\strutil.hpp"

using namespace std::string_literals;

const std::string disconnectedColor = "#D0D0D0"s;
const std::string serverIdle = "#99CCFF"s;
const std::string serverShow = "#99FF99"s;
const std::string clientError = "#FF8888"s;
const std::string neverConnected = "#FFFF99"s;

std::string serverColor = neverConnected ;

enum ConnectState {
    NEVER, CONNECTED, DISCONNECTED
};

auto printTable(std::ostream& output, const ServerStatusEntry &serverstatus, ClientCollection& clients, ConnectionStatus& status, ErrorCollection& clienterrors) -> void;
auto printTableRow(std::ostream& output, const std::string& client, ConnectState state, bool audioerr, bool playerr, const std::string &playname ) -> void;
//auto printTableRow(std::ostream& output, const std::string& client, ConnectState state, bool has_error) -> void;
auto writeBeginning(std::ostream& output) -> void;
auto writeEnding(std::ostream& output) -> void;
//======================================================================================================================
int main(int argc, const char* argv[]) {
    auto exitcode = EXIT_SUCCESS;
    try {
        if (argc < 6) {
            throw std::runtime_error("Invalid # arguments, requires: clientpath  serverlog connectpath errorpath outputpath [-update]");
        }
        auto clientpath = std::filesystem::path(argv[1]);
        ServerStatus serverStatus ;
        auto serverlog = std::string(argv[2]) ;
        auto connectpath = std::filesystem::path(argv[3]);
        auto errorpath = std::filesystem::path(argv[4]);
        auto outputpath = std::filesystem::path(argv[5]);
        
        serverStatus.load(serverlog);
        auto clientCollection = ClientCollection(clientpath);
        auto connectionStatus = ConnectionStatus(connectpath);
        auto errorCollection = ErrorCollection(errorpath);
        std::string updatestring = "";
        auto loop = true;
        if (argc == 7) {
            // We should update?
            updatestring = argv[6];
            if (updatestring != "-update") {
                throw std::runtime_error("Invalid # arguments, requires: clientpath  serverlog connectpath errorpath outputpath [-update]");
            }
        }
        if (updatestring.empty()) {
            loop = false;
        }

        do {
            if (serverStatus.hasChanged()) {
                serverStatus.load(serverlog);
            }
            if (connectionStatus.hasChanged()) {
                connectionStatus.load(connectpath);
            }
            if (errorCollection.hasChanged()) {
                errorCollection.load(errorpath);
            }
            auto output = std::ofstream(outputpath.string());
            if (!output.is_open()) {
                throw std::runtime_error("Unable to create: "s + outputpath.string());
            }
            writeBeginning(output);

            printTable(output, serverStatus.status(),clientCollection, connectionStatus, errorCollection);
            writeEnding(output);
            output.close();
            if (loop) {
                while (loop && !serverStatus.hasChanged() && !errorCollection.hasChanged() && !connectionStatus.hasChanged()) {
                    std::this_thread::sleep_for(std::chrono::seconds(10));
                }
            }
        } while (loop);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exitcode = EXIT_FAILURE;
    }
    catch (...) {
        std::cerr << "Unknown Error!" << std::endl;
        exitcode = EXIT_FAILURE;
    }

    return 0;
}


// =========================================================================
auto writeBeginning(std::ostream& output) -> void {
    // start writing the document
    output << "<!DOCTYPE html>\n";
    output << "<html>\n"; 
    output << "<head>\n";
    output << "     <meta http-equiv = \"refresh\" content = \"10\" >\n";
    output << "<style>\n";
    output << "table,th,td{border:2px solid black;border-collapse:collapse}\n";
    output << "</style>\n";
    output << "</head>\n";
 
}
// ========================================================================
auto writeEnding(std::ostream& output) -> void {
    output << "</body>\n";
    output << "</html>\n";

}
// ===============================================================================================================================
auto printTable(std::ostream& output, const ServerStatusEntry &serverstatus, ClientCollection& clients, ConnectionStatus& status, ErrorCollection& clienterrors) -> void {
    
    serverColor = neverConnected;
    auto text = "Not Running Server"s;
    if (serverstatus.isRunning) {
        serverColor = disconnectedColor;
        text = "Server Waiting for Show Time"s;
    }
    if (serverstatus.isListening) {
        serverColor = serverIdle;
        text = "Server Idle"s;
    }
    if (serverstatus.inShow) {
        serverColor = serverShow;
        text = "Playing Show"s;
    }


    output << "\t<table style=\"font-family: Arial;font-size:16px;margin-left:40px;\">\n";
    output << "\t\t<thead>\n"; 
    output << "\t\t\t<tr>\n";
    output << "\t\t\t<th style = \"text-align:center;color:"<< serverColor <<";background-color:#303030;width:423px\" scope=\"col\">" << util::sysTimeToString(util::ourclock::now()) << "</th>\n";
    output << "\t\t\t</tr>\n";
    output << "\t\t\t<tr>\n";
    output << "\t\t\t<th style = \"text-align:center;color:#FFFFFF;background-color:#303030;width:423px\" scope=\"col\">Server Status</th>\n";
    output << "\t\t\t</tr>\n";
    output << "\t\t\t<tr>\n";
    output << "\t<table style=\"font-family: Arial;font-size:14px;margin-left:40px;\">\n";
    output << "\t\t\t<td style = \"text-align:center;color:#000000;background-color:"<< serverColor <<";width:423px\" scope=\"col\">" << text << "</td>\n";
    output << "\t\t\t</tr>\n";
    output << "\t</table>\n";

    output << "\t<table style=\"font-family: Arial;font-size:16px;margin-left:40px;\">\n";
    output << "\t\t<thead>\n";
    output << "\t\t\t<tr>\n";
    output << "\t\t\t<th style = \"text-align:center;color:#FFFFFF;background-color:#303030;width:423px\" scope=\"col\">Client Status</th>\n";
    output << "\t\t\t</tr>\n";
    output << "\t</table>\n";


    output << "\t<table style=\"font-family: Arial;font-size:14px;margin-left:40px;\">\n";
    output << "\t\t\t<tr>\n";
    output << "\t\t\t<th style=\"text-align:center;background-color:#A0A0A0;width:190px\" scope=\"col\">Client/Status</th>\n";
    output << "\t\t\t<th style=\"text-align:center;background-color:#A0A0A0;width:185px\" scope=\"col\">Timestamp</th>\n";
    output << "\t\t\t<th style=\"text-align:center;background-color:#A0A0A0;width:40px\" scope=\"col\">Error</th>\n";
     output << "\t\t\t</tr>\n";
    output << "\t\t</thead>\n";
    output << "\t\t<tbody>\n";
    const auto formattime = "%a - %b %d %H:%M:%S"s;
    for (const auto& client : clients.clients) {

        ConnectState state = ConnectState::NEVER;
        auto playname = ""s;
        if (status.contains(client.name)) {
            state = ConnectState::DISCONNECTED;
            if (status[client.name].last().state) {
                state = ConnectState::CONNECTED;
            }
           playname = util::sysTimeToString(status[client.name].last().timestamp,formattime);
        }
       
  //      auto has_error = false;
  //      if (clienterrors.contains(client.name)) {
  //          has_error = true;
  //      }
        auto play_error = false;
        auto audio_error = false;
       

        if (clienterrors.contains(client.name)) {

            auto eiter = clienterrors[client.name].rbegin();
            audio_error = eiter->audio_error;
            play_error = !eiter->audio_error;
         }

       

        // Now we print the entry ;
 //       printTableRow(output, client.name, state, has_error);
        printTableRow(output, client.name, state, audio_error, play_error, playname);
    }



    output << "\t\t</tbody>\n";
    output << "\t</table>\n";

}

// ==============================================================================================================
//auto printTableRow(std::ostream& output, const std::string& client, ConnectState state, bool has_error) -> void {
auto printTableRow(std::ostream& output, const std::string& client, ConnectState state, bool audioerr, bool playerr , const std::string &playname) -> void {
    auto concolor  = neverConnected;
    auto value = "Never"s;
    switch (state) {
    case ConnectState::NEVER:
        concolor = neverConnected;
        value = "Never"s;
        break;
    case ConnectState::CONNECTED:
        concolor = serverColor;
        value = "Connected"s;
        break;
    case ConnectState::DISCONNECTED:
        concolor = disconnectedColor;
        value = "Disconnected"s;
        break;
    }




    output << "\t\t\t<tr style=\"text-align:center;\">\n";
    output << "\t\t\t\t<td style=\"background-color:" << concolor << ";\">" << client << "</td>\n";
    output << "\t\t\t\t<td style=\"background-color:" << concolor << ";\">" << playname << "</td>\n";
    output << "\t\t\t\t<td " << ((audioerr || playerr) ? ("style=\"background-color:"s + clientError + ";\">") : ("style=\"background-color:"s + disconnectedColor + ";\">"s)) << ((audioerr || playerr) ? "Error" : "") << "</td>\n";
//    output << "\t\t\t\t<td " << (playerr ? "style=\"background-color:#FF8888;\">" : "style=\"background-color:#D0D0D0;\">") << (playerr ? "Error" : "") << "</td>\n";
//    output << "\t\t\t\t<td " << (playerr ? "style=\"background-color:#FF8888;\">" : "style=\"background-color:#D0D0D0;\">") << (playerr ? playname : "") << "</td>\n";
 
    output << "\t\t\t</tr>\n";
}
