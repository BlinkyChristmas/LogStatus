//Copyright © 2024 Charles Kerr. All rights reserved.

#include "LogEntry.hpp"

#include <algorithm>
#include <stdexcept>

#include "utility/strutil.hpp"
using namespace std::string_literals ;

//======================================================================
LogEntry::LogEntry():timestamp(util::ourclock::now()),state(false) {
    
}

//======================================================================
LogEntry::LogEntry( const std::string &line):LogEntry() {
    auto values = util::parse(line,",");
    switch (values.size()) {
        default:
        case 3:
            this->address = values[2] ;
            [[fallthrough]];
        case 2:
            this->state = (values[1] == "Connected" ? true:false) ;
            [[fallthrough]] ;
        case 1:
            this->timestamp = util::stringToSysTime(values[0]);
            [[fallthrough]];
        case 0:
            break;
    }
}

//======================================================================
auto LogEntry::describe() const -> std::string {
    return  (state?"\033[92mConnected\033[0m"s:"\033[91mDisconnected\033[0m"s) + " \033[93m" + this->address + "\033[0m "s + util::sysTimeToString(this->timestamp)  ;
}

//======================================================================
auto LogEntry::operator<(const LogEntry &value) const -> bool {
    return this->timestamp < value.timestamp ;
}

