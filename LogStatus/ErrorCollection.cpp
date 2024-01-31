//Copyright © 2024 Charles Kerr. All rights reserved.

#include "ErrorCollection.hpp"

#include <algorithm>
#include <stdexcept>
#include <vector>
#include <fstream>

#include "utility/strutil.hpp"

using namespace std::string_literals ;

//======================================================================
ErrorCollection::ErrorCollection() {
    
}

//======================================================================
ErrorCollection::ErrorCollection(const std::filesystem::path &path):ErrorCollection() {
    if (!this->load(path)){
        throw std::runtime_error("Unable to load: "s + path.string()) ;
    }
}

//======================================================================
auto ErrorCollection::load(const std::filesystem::path &path) -> bool {
    auto input = std::ifstream(path.string()) ;
    if (!input.is_open()){
        return false ;
    }
    clienterrors.clear() ;
    auto buffer = std::vector<char>(2048,0) ;
    while (input.good() && !input.eof()){
        input.getline(buffer.data(), buffer.size()) ;
        if (input.gcount()>0) {
            buffer[input.gcount()] = 0 ;
            std::string line = buffer.data() ;
            line = util::trim(util::strip(line,"#")) ;
            if (!line.empty()) {
                auto [name,value] = util::split(line,"=") ;
                if (!value.empty()) {
                    auto iter = clienterrors.find(name) ;
                    if (iter == clienterrors.end()) {
                        // We want to add an entry
                        clienterrors.insert_or_assign(name,std::deque<ErrorEntry>()) ;
                    }
                    iter = clienterrors.find(name) ;
                    iter->second.push_back(ErrorEntry(value)) ;
                }
            }
        }
    }
    input.close() ;
    for (auto &[name,entry]:clienterrors){
        std::sort(entry.begin(),entry.end()) ;
    }
    return true ;
}

//======================================================================
auto ErrorCollection::contains(const std::string &name) const -> bool {
    auto iter = clienterrors.find(name) ;
    return iter != clienterrors.end() ;
}

//======================================================================
auto ErrorCollection::size() const -> size_t {
    return clienterrors.size() ;
}
//======================================================================
auto ErrorCollection::empty() const -> bool {
    return clienterrors.empty() ;
}

//======================================================================
auto ErrorCollection::operator[](const std::string &name) const -> const std::deque<ErrorEntry>& {
    auto iter = clienterrors.find(name) ;
    if (iter == clienterrors.end()){
        throw std::out_of_range("Entry is not present: "s + name) ;
    }
    return iter->second ;
}
//======================================================================
auto ErrorCollection::operator[](const std::string &name)  ->  std::deque<ErrorEntry>& {
    auto iter = clienterrors.find(name) ;
    if (iter == clienterrors.end()){
        throw std::out_of_range("Entry is not present: "s + name) ;
    }
    return iter->second ;

}
