//Copyright © 2024 Charles Kerr. All rights reserved.

#include "LogContainer.hpp"

#include <algorithm>
#include <stdexcept>


using namespace std::string_literals ;

//======================================================================
auto LogContainer::addData(const std::string &data) -> void {
    states.emplace_back(LogEntry(data)) ;
}

//======================================================================
auto LogContainer::addEntry(const LogEntry &entry) -> void {
    states.push_back(entry) ;
}

//======================================================================
auto LogContainer::size() const -> size_t {
    return states.size() ;
}

//======================================================================
auto LogContainer::empty() const -> bool {
    return states.empty() ;
}

//======================================================================
auto LogContainer::sort() -> void {
    std::sort(states.begin(),states.end()) ;
}

//======================================================================
auto LogContainer::first() -> LogEntry& {
    if (states.empty()) {
        throw std::out_of_range("There is no information in container");
    }
    return *states.begin() ;
}

//======================================================================
auto LogContainer::first() const -> const LogEntry& {
    if (states.empty()) {
        throw std::out_of_range("There is no information in container");
    }
    return *states.begin() ;
}

//======================================================================
auto LogContainer::last() -> LogEntry& {
    if (states.empty()) {
        throw std::out_of_range("There is no information in container");
    }
    return *states.rbegin() ;
}

//======================================================================
auto LogContainer::last() const -> const LogEntry& {
    if (states.empty()) {
        throw std::out_of_range("There is no information in container");
    }
    return *states.rbegin() ;
}

//======================================================================
auto LogContainer::operator[](size_t index) const -> const LogEntry& {
    if (index >= states.size()){
        throw std::out_of_range("Index: "s + std::to_string(index) + " exceeds size: "s + std::to_string(states.size()));
    }
    return states.at(index) ;
}

//======================================================================
auto LogContainer::operator[](size_t index)  -> LogEntry& {
    if (index >= states.size()){
        throw std::out_of_range("Index: "s + std::to_string(index) + " exceeds size: "s + std::to_string(states.size()));
    }
    return states.at(index) ;

}
