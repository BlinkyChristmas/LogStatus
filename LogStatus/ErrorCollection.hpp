//Copyright © 2024 Charles Kerr. All rights reserved.

#ifndef ErrorCollection_hpp
#define ErrorCollection_hpp

#include <cstdint>
#include <deque>
#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>

#include "ErrorEntry.hpp"
//======================================================================
struct ErrorCollection {
    std::unordered_map<std::string,std::deque<ErrorEntry>> clienterrors ;
    std::filesystem::file_time_type lastRead ;
    std::filesystem::path ourpath ;
    ErrorCollection() ;
    ErrorCollection(const std::filesystem::path &path) ;

    auto load(const std::filesystem::path &path) -> bool ;

    auto contains(const std::string &name) const -> bool ;
    auto size() const -> size_t ;
    auto empty() const -> bool ;
    
    auto operator[](const std::string &name) const -> const std::deque<ErrorEntry>& ;
    auto operator[](const std::string &name)  ->  std::deque<ErrorEntry>& ;
    auto hasChanged() const -> bool ;
};

#endif /* ErrorCollection_hpp */
