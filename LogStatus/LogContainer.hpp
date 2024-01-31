//Copyright © 2024 Charles Kerr. All rights reserved.

#ifndef LogContainer_hpp
#define LogContainer_hpp

#include <cstdint>
#include <iostream>
#include <string>
#include <deque>

#include "LogEntry.hpp"
//======================================================================
class LogContainer {
    
    std::deque<LogEntry> states ;
    
public:
    LogContainer() = default ;
    
    auto addData(const std::string &data) -> void ;
    auto addEntry(const LogEntry &entry) -> void ;
    
    auto size() const -> size_t ;
    auto empty() const -> bool ;
    auto sort() -> void ;
    
    auto first() -> LogEntry& ;
    auto first() const -> const LogEntry& ;

    auto last() -> LogEntry& ;
    auto last() const -> const LogEntry& ;

    auto operator[](size_t index) const -> const LogEntry& ;
    auto operator[](size_t index)  -> LogEntry& ;

    
};

#endif /* LogContainer_hpp */
