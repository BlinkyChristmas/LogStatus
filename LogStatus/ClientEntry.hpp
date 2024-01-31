//Copyright © 2024 Charles Kerr. All rights reserved.

#ifndef ClientEntry_hpp
#define ClientEntry_hpp

#include <cstdint>
#include <iostream>
#include <string>

//======================================================================
struct ClientEntry {
    std::string name ;
    bool has_audio ;
    ClientEntry() ;
};

#endif /* ClientEntry_hpp */
