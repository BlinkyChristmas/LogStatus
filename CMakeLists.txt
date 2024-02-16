cmake_minimum_required(VERSION 3.25)
project(LogStatus VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

add_executable(LogStatus
    ./LogStatus/ClientCollection.cpp
    ./LogStatus/ClientCollection.hpp
    ./LogStatus/ClientEntry.cpp
    ./LogStatus/ClientEntry.hpp
    ./LogStatus/ConnectionStatus.cpp
    ./LogStatus/ConnectionStatus.hpp
    ./LogStatus/ErrorCollection.cpp
    ./LogStatus/ErrorCollection.hpp
    ./LogStatus/ErrorEntry.cpp
    ./LogStatus/ErrorEntry.hpp
    ./LogStatus/LogContrainer.cpp
    ./LogStatus/LogContrainer.hpp
    ./LogStatus/LogEntry.cpp
    ./LogStatus/LogEntry.hpp
    ./LogStatus/ServerStatus.cpp
    ./LogStatus/ServerStatus.hpp

    ./common/utility/buffer.cpp
    ./common/utility/buffer.hpp
    ./common/utility/dbgutil.hpp
    ./common/utility/mapfile.cpp
    ./common/utility/mapfile.hpp
    ./common/utility/strutil.hpp
    ./common/utility/timeutil.cpp
    ./common/utility/timeutil.hpp

)
if (WIN32)
    target_compile_definitions(LogStatus PRIVATE
        $<$<CONFIG:Release>:NDEBUG>
        $<$<CONFIG:Debug>:_DEBUG=1>
        __WINDOWS_DS__
        _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
        _WIN32_WINNT=0x0A00
    )

    target_compile_options(LogStatus PRIVATE
        $<$<CONFIG:Release>:/O2>
    )
else()
    target_compile_definitions(LogStatus PRIVATE
        $<$<CONFIG:Debug>:DEBUG=1>
    )

    target_compile_options(LogStatus PRIVATE
        $<$<CONFIG:Release>:-O2>
        -Wno-deprecated-declarations
    )
endif (WIN32)
target_include_directories(LogStatus
    PUBLIC
        ${PROJECT_SOURCE_DIR}/
        ${PROJECT_SOURCE_DIR}/LogStatus/
        ${PROJECT_SOURCE_DIR}/common/
)
