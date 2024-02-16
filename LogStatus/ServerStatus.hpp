//Copyright © 2024 Charles Kerr. All rights reserved.

#ifndef ServerStatus_hpp
#define ServerStatus_hpp

#include <cstdint>
#include <iostream>
#include <string>

//======================================================================
struct ServerStatusEntry {
    bool isRunning ;
    bool isListening ;
    bool inShow ;
    ServerStatusEntry():isRunning(false),isListening(false),inShow(false) {}
    ServerStatusEntry(const std::string &line) ;
};
//======================================================================
struct ServerStatus {
    std::vector<ServerStatusEntry> entries ;
    auto load(const std::string &filepath) -> bool ;
    auto status() const -> const ServerStatusEntry & ;
};

#endif /* ServerStatus_hpp */
