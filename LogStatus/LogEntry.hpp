//Copyright © 2024 Charles Kerr. All rights reserved.

#ifndef LogEntry_hpp
#define LogEntry_hpp

#include <cstdint>
#include <iostream>
#include <string>

#include "utility/timeutil.hpp"
//======================================================================
struct LogEntry {
    
    util::ourclock::time_point timestamp ;
    bool state ;
    std::string address ;
    LogEntry() ;
    LogEntry( const std::string &line) ;
    auto describe() const -> std::string ;
    
    auto operator<(const LogEntry &value) const -> bool ;
    
};

#endif /* LogEntry_hpp */
