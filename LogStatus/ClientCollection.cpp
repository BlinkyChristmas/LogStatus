//Copyright © 2024 Charles Kerr. All rights reserved.

#include "ClientCollection.hpp"

#include <algorithm>
#include <stdexcept>
#include <fstream>

#include "utility/strutil.hpp"

using namespace std::string_literals ;

//======================================================================
ClientCollection::ClientCollection() {
    
}

//======================================================================
ClientCollection::ClientCollection(const std::filesystem::path &path):ClientCollection(){
    if (!this->load(path)){
        throw std::runtime_error("Unable to load: "s + path.string());
    }
}

//======================================================================
auto ClientCollection::load(const std::filesystem::path &path) -> bool {
    clients.clear() ;
    
    auto input = std::ifstream(path.string()) ;
    if (!input.is_open()){
        return false ;
    }
    auto buffer = std::vector<char>(2048,0) ;
    while (input.good() && !input.eof()) {
        input.getline(buffer.data(),buffer.size()-1) ;
        if (input.gcount()>1) {
            buffer[input.gcount()] = 0 ;
            std::string line = buffer.data() ;
            line = util::trim(util::strip(line,"#")) ;
            if (!line.empty()) {
                auto [key,value] = util::split(line, "=") ;
                auto ukey = util::upper(key) ;
                if (ukey == "CLIENT"){
                    // We should process this one!
                    auto [name,audio] = util::split(value,",") ;
                    auto entry = ClientEntry() ;
                    entry.name = name ;
                    if (!audio.empty()) {
                        try {
                            entry.has_audio = (std::stoi(audio) != 0) ;
                        }
                        catch(...) {
                            std::cerr << "Error processing audio value for: "s << value <<" Skipping audio value" << std::endl;
                        }
                    }
                    clients.push_back(entry) ;
                    
                }
            }
        }
    }
    return true ;
}

//======================================================================
auto ClientCollection::size() const -> size_t {
    return clients.size() ;
}

//======================================================================
auto ClientCollection::empty() const -> bool {
    return clients.empty() ;
}

//======================================================================
auto ClientCollection::operator[](size_t index) const -> const ClientEntry& {
    if (index >= clients.size()){
        throw std::out_of_range("Index: "s + std::to_string(index) +" exceeds size: "s + std::to_string(clients.size()));
    }
    return clients.at(index) ;
}

//======================================================================
auto ClientCollection::operator[](size_t index)  ->  ClientEntry& {
    if (index >= clients.size()){
        throw std::out_of_range("Index: "s + std::to_string(index) +" exceeds size: "s + std::to_string(clients.size()));
    }
    return clients.at(index) ;
}

