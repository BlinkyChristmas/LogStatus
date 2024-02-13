//Copyright © 2024 Charles Kerr. All rights reserved.

#include "ErrorEntry.hpp"

#include <algorithm>
#include <stdexcept>

#include "utility/strutil.hpp"

using namespace std::string_literals ;

//======================================================================
//======================================================================
ErrorEntry::ErrorEntry():timestamp(util::ourclock::now()),audio_error(false) {
    
}

//======================================================================
ErrorEntry::ErrorEntry( const std::string &line):ErrorEntry() {
    auto values = util::parse(line,",");
    switch (values.size()) {
        default:
        case 3:
            this->play_name = values[2] ;
            [[fallthrough]];
            /*
        case 3:
            this->address = values[2] ;
            [[fallthrough]];
            */
        case 2:
            this->audio_error = (values[1] == "AUDIO" ? true:false) ;
            [[fallthrough]] ;
        case 1:
            this->timestamp = util::stringToSysTime(values[0]);
            [[fallthrough]];
        case 0:
            break;
    }
}

//======================================================================
auto ErrorEntry::describe() const -> std::string {
    return  (audio_error?"\033[92mAudio\033[0m"s:"\033[91mPlay\033[0m"s) + " \033[93m" + this->address + "\033[0m "s + util::sysTimeToString(this->timestamp)  ;
}

//======================================================================
auto ErrorEntry::operator<(const ErrorEntry &value) const -> bool {
    return this->timestamp < value.timestamp ;
}

