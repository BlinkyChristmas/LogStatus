//Copyright © 2024 Charles Kerr. All rights reserved.

#ifndef ServerStatus_hpp
#define ServerStatus_hpp

#include <cstdint>
#include <iostream>
#include <string>
#include <filesystem>

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
    std::filesystem::file_time_type lastRead ;
    std::filesystem::path ourfile ;
    std::vector<ServerStatusEntry> entries ;
    auto load(const std::string &filepath) -> bool ;
    auto status() const -> const ServerStatusEntry & ;
    auto hasChanged() const -> bool ;

};

#endif /* ServerStatus_hpp */
