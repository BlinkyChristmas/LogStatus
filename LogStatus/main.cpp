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

using namespace std::string_literals;

enum ConnectState {
    NEVER, CONNECTED, DISCONNECTED
};

auto printTable(std::ostream& output, ClientCollection& clients, ConnectionStatus& status, ErrorCollection& clienterrors) -> void;
auto printTableRow(std::ostream& output, const std::string& client, ConnectState state, bool audioerr, bool playerr, const std::string &playname ) -> void;
//auto printTableRow(std::ostream& output, const std::string& client, ConnectState state, bool has_error) -> void;
auto writeBeginning(std::ostream& output) -> void;
auto writeEnding(std::ostream& output) -> void;
//======================================================================================================================
int main(int argc, const char* argv[]) {
    auto exitcode = EXIT_SUCCESS;
    try {
        if (argc < 5) {
            throw std::runtime_error("Invalid # arguments, requires: clientpath  connectpath errorpath outputpath [-update]");
        }
        auto clientpath = std::filesystem::path(argv[1]);
        auto connectpath = std::filesystem::path(argv[2]);
        auto errorpath = std::filesystem::path(argv[3]);
        auto outputpath = std::filesystem::path(argv[4]);

        auto clientCollection = ClientCollection(clientpath);
        auto connectionStatus = ConnectionStatus(connectpath);
        auto errorCollection = ErrorCollection(errorpath);
        std::string updatestring = "";
        auto loop = true;
        if (argc == 6) {
            // We should update?
            updatestring = argv[5];
            if (updatestring != "-update") {
                throw std::runtime_error("Invalid # arguments, requires: clientpath  connectpath errorpath outputpath [-update]");
            }
        }
        if (updatestring.empty()) {
            loop = false;
        }

        do {
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

            printTable(output, clientCollection, connectionStatus, errorCollection);
            writeEnding(output);
            output.close();
            if (loop) {
                while (loop && !errorCollection.hasChanged() && !connectionStatus.hasChanged()) {
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
auto printTable(std::ostream& output, ClientCollection& clients, ConnectionStatus& status, ErrorCollection& clienterrors) -> void {
    output << "\t<table style=\"font-family: Arial;font-size:16px;margin-left:40px;\">\n";
    output << "\t\t<thead>\n"; 
    output << "\t\t\t<tr>\n";
    output << "\t\t\t<th style = \"text-align:center;color:#FFFFFF;background-color:#303030;width:502px\" scope=\t\"col\">Client Status</th>\n";
    output << "\t\t\t</tr>\n";
    output << "\t</table>\n";

    output << "\t<table style=\"font-family: Arial;font-size:14px;margin-left:40px;\">\n";
    output << "\t\t\t<tr>\n";
    output << "\t\t\t<th style=\"text-align:center;background-color:#A0A0A0;width:175px\" scope=\"col\">Client/Status</th>\n";
    output << "\t\t\t<th style=\"text-align:center;background-color:#A0A0A0;width:70px\" scope=\"col\">AudioErr</th>\n";
    output << "\t\t\t<th style=\"text-align:center;background-color:#A0A0A0;width:70px\" scope=\"col\">PlayErr</th>\n";
    output << "\t\t\t<th style=\"text-align:center;background-color:#A0A0A0;width:175px\" scope=\"col\">Song Name</th>\n";
    output << "\t\t\t</tr>\n";
    output << "\t\t</thead>\n";
    output << "\t\t<tbody>\n";

    for (const auto& client : clients.clients) {

        ConnectState state = ConnectState::NEVER;
        if (status.contains(client.name)) {
            state = ConnectState::DISCONNECTED;
            if (status[client.name].last().state) {
                state = ConnectState::CONNECTED;
            }
        }
  //      auto has_error = false;
  //      if (clienterrors.contains(client.name)) {
  //          has_error = true;
  //      }
        auto play_error = false;
        auto audio_error = false;
        auto playname = ""s;
  
        if (clienterrors.contains(client.name)) {

            auto eiter = clienterrors[client.name].rbegin();
            audio_error = eiter->audio_error;
            play_error = !eiter->audio_error;
            playname = eiter->play_name;
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
    auto concolor = "#FFFF99"s;
    auto value = "Never"s;
    switch (state) {
    case ConnectState::NEVER:
        concolor = "#FFFF99"s;
        value = "Never"s;
        break;
    case ConnectState::CONNECTED:
        concolor = "#99FF99"s;
        value = "Connected"s;
        break;
    case ConnectState::DISCONNECTED:
        concolor = "#D0D0D0"s;
        value = "Disconnected"s;
        break;
    }
    output << "\t\t\t<tr style=\"text-align:center;\">\n";
    output << "\t\t\t\t<td style=\"background-color:" << concolor << ";\">" << client << "</td>\n";
    output << "\t\t\t\t<td " << (audioerr ? "style=\"background-color:#FF8888;\">" : "style=\"background-color:#D0D0D0;\">") << (audioerr ? "Error" : "") << "</td>\n";
    output << "\t\t\t\t<td " << (playerr ? "style=\"background-color:#FF8888;\">" : "style=\"background-color:#D0D0D0;\">") << (playerr ? "Error" : "") << "</td>\n";
    output << "\t\t\t\t<td " << (playerr ? "style=\"background-color:#FF8888;\">" : "style=\"background-color:#D0D0D0;\">") << (playerr ? playname : "") << "</td>\n";
    output << "\t\t\t</tr>\n";
}