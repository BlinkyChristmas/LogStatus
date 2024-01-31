//Copyright © 2024 Charles Kerr. All rights reserved.

#ifndef ErrorEntry_hpp
#define ErrorEntry_hpp

#include <cstdint>
#include <iostream>
#include <string>

#include "utility/timeutil.hpp"
//======================================================================
struct ErrorEntry {
    util::ourclock::time_point timestamp ;
    bool audio_error;
    std::string address ;
    std::string play_name ;
    
    ErrorEntry() ;
    ErrorEntry( const std::string &line) ;
    auto describe() const -> std::string ;
    
    auto operator<(const ErrorEntry &value) const -> bool ;

};

#endif /* ErrorEntry_hpp */
