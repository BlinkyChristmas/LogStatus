//Copyright © 2024 Charles Kerr. All rights reserved.

#ifndef ClientCollection_hpp
#define ClientCollection_hpp

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include "ClientEntry.hpp"

//======================================================================
struct ClientCollection {
    std::vector<ClientEntry> clients ;
    
    ClientCollection() ;
    ClientCollection(const std::filesystem::path &path);
    
    auto load(const std::filesystem::path &path) -> bool ;
    
    auto size() const -> size_t ;
    auto empty() const -> bool ;
    
    auto operator[](size_t index) const -> const ClientEntry& ;
    auto operator[](size_t index)  ->  ClientEntry& ;

};

#endif /* ClientCollection_hpp */
