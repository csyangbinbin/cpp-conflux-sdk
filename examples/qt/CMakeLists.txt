list(APPEND CMAKE_PREFIX_PATH  $ENV{QT5_ROOT})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(cfx_qt
    widget.ui
    widget.cpp
    widget.h
    main.cpp
    AsyncQueryAccountThread.h
    AsyncQueryAccountThread.cpp
    TransferThread.h
    TransferThread.cpp
    DeployContractThread.h
    DeployContractThread.cpp
)

target_link_libraries(cfx_qt Qt5::Widgets cpp_conflux_sdk)