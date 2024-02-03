//Copyright © 2024 Charles Kerr. All rights reserved.

#include "ConnectionStatus.hpp"

#include <algorithm>
#include <stdexcept>

#include <fstream>
#include <vector>

#include "utility/strutil.hpp"
using namespace std::string_literals ;

//======================================================================
ConnectionStatus::ConnectionStatus(const std::filesystem::path &filepath) : ConnectionStatus() {
    if (!this->load(filepath)) {
        throw std::runtime_error("Unable to load: "s + filepath.string());
    }
}

//======================================================================
auto ConnectionStatus::load(const std::filesystem::path &filepath) -> bool  {
    connectionStatus.clear() ;
    if (!std::filesystem::exists(filepath)) {
        return false ;
    }
    auto fcopy = filepath ;
    try {
        fcopy.replace_extension(fcopy.extension().string() + "-copy"s);
    }
    catch (...) {
        std::cerr << "Unable to copy file " << std::endl;
    }
    //std::cout << "copy the log file" << std::endl;
    std::filesystem::copy(filepath,fcopy) ;
    auto input = std::ifstream(fcopy.string()) ;
    if (!input.is_open()) {
        std::cerr << "Unable to open copy: "s << fcopy.string() << std::endl;
        return false ;
    }
    auto buffer = std::vector<char>(2048,0) ;
    while (input.good() && !input.eof()) {
        input.getline(buffer.data(), buffer.size()-1) ;
        if (input.gcount()>0) {
            buffer[input.gcount()] = 0 ;
            std::string line = buffer.data() ;
            line = util::trim(line) ;
            if (!line.empty()) {
                auto [name,data] = util::split(line, "=") ;
                auto iter = connectionStatus.find(name) ;
                if (iter == connectionStatus.end()) {
                    auto cont = LogContainer() ;
                    cont.addData(data) ;
                    connectionStatus.insert_or_assign(name,cont) ;
                }
                else {
                    iter->second.addData(data) ;
                }
            }
        }
    }
    input.close();
    // Delete our copy of the file
    std::filesystem::remove(fcopy) ;
    // Now, lets sort them all
    for (auto &[name,entry]:connectionStatus) {
        entry.sort() ;
    }
    return true ;
}

//======================================================================
auto ConnectionStatus::size() const -> size_t {
    return connectionStatus.size() ;
}
//======================================================================
auto ConnectionStatus::empty() const -> bool {
    return connectionStatus.empty() ;
}

//======================================================================
auto ConnectionStatus::keys() const -> std::vector<std::string> {
    auto rvalue = std::vector<std::string>() ;
    for (const auto &[name,value]:connectionStatus){
        rvalue.push_back(name) ;
    }
    return rvalue ;
}

//======================================================================
auto ConnectionStatus::contains(const std::string &name) const -> bool {
    auto iter = connectionStatus.find(name) ;
    return iter != connectionStatus.end() ;
}

//======================================================================
auto ConnectionStatus::operator[](const std::string &key) const -> const LogContainer& {
    auto iter = connectionStatus.find(key) ;
    if (iter != connectionStatus.end()){
        return iter->second ;
    }
    throw std::out_of_range("No status for entry: "s + key) ;
}
//======================================================================
auto ConnectionStatus::operator[](const std::string &key)  -> LogContainer& {
    auto iter = connectionStatus.find(key) ;
    if (iter != connectionStatus.end()){
        return iter->second ;
    }
    throw std::out_of_range("No status for entry: "s + key) ;

}

// ========================================================================
auto ConnectionStatus::save(const std::filesystem::path &path, int hours) -> bool {
    auto output = std::ofstream(path.string()) ;
    if (!output.is_open()){
        return false ;
    }
    auto time = util::ourclock::now() - std::chrono::hours(hours) ;
    for (const auto &[name,value]:connectionStatus){
        auto cont = LogContainer() ;
        for (size_t i = 0 ; i < value.size();i++) {
            if (value[i].timestamp >= time ) {
                cont.addEntry(value[i]) ;
            }
        }
        if (cont.empty()) {
            // it was empty?
            cont.addEntry(value.last()) ;
        }
        cont.sort() ;
        if (cont.last().state) {
            // The last state was "connected, so we need to add one
            auto temp = cont.last() ;
            temp.state = false ;
            temp.timestamp = util::ourclock::now() ;
            cont.addEntry(temp) ;
            cont.sort() ;
        }
        // Now we write them out :
        for (size_t i = 0 ; i < cont.size();i++) {
            output << name << " = " << util::sysTimeToString(cont[i].timestamp) << " , " << (cont[i].state? "Connected , ":"Disconnected , ") << cont[i].address << std::endl; ;
        }
    }
    return true ;
}
