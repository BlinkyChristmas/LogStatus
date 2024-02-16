//Copyright © 2024 Charles Kerr. All rights reserved.

#include "ServerStatus.hpp"

#include <algorithm>
#include <stdexcept>
#include <fstream>
#include "utility/strutil.hpp"

using namespace std::string_literals ;

//======================================================================
ServerStatusEntry::ServerStatusEntry(const std::string &line) : ServerStatusEntry() {
    auto values = util::parse(line,",") ;
    switch (values.size()) {
        default:
        case 3:
            inShow = (values[2]=="OFF"?false:true) ;
            [[fallthrough]];
        case 2:
            isListening = (values[1]=="OFF"?false:true) ;
            [[fallthrough]];
        case 1:
            isRunning = (values[0]=="OFF"?false:true) ;
            [[fallthrough]];
        case 0:
            break;
        
    }
}
//======================================================================
auto ServerStatus::load(const std::string &filepath) -> bool {
    entries.clear() ;
    ourfile = std::filesystem::path(filepath) ;
    lastRead = std::filesystem::last_write_time(ourfile) ;

    auto input = std::ifstream(filepath) ;
    if (!input.is_open()) {
        return false ;
    }
    auto buffer = std::vector<char>(1024,0) ;
    while (input.good() && !input.eof()) {
        input.getline(buffer.data(),buffer.size()-1) ;
        if (input.gcount()>0){
            buffer[input.gcount()] = 0 ;
            std::string line = buffer.data() ;
            line = util::trim(util::strip(line,"#")) ;
            if (!line.empty()){
                auto [key,value] = util::split(line,"=") ;
                if (util::upper(key) == "STATE") {
                    entries.push_back(ServerStatusEntry(value)) ;
                }
            }
        }
    }
    return true ;
}

//======================================================================
auto ServerStatus::status() const -> const ServerStatusEntry & {
    static ServerStatusEntry noentry ;
    if (entries.empty()){
        return noentry ;
    }
    return *(entries.rbegin()) ;
}

// ==============================================================================================================================
auto ServerStatus::hasChanged() const -> bool {
    auto lastwrite = std::filesystem::last_write_time(ourfile) ;
    
    if ( std::chrono::duration_cast<std::chrono::seconds>(lastwrite - lastRead).count() > 0 ){
        return true ;
    }
    return false ;

}
