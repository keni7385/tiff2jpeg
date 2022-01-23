cmake_minimum_required(VERSION 3.22)
project(tiff2jpeg VERSION 0.0.1 LANGUAGES CXX)

configure_file(config.h.in config.h)

find_package(JPEG REQUIRED)
find_package(TIFF REQUIRED COMPONENTS CXX)
find_package(exiv2 REQUIRED CONFIG NAMES exiv2) # search ${CMAKE_INSTALL_PREFIX}/lib/cmake/exiv2/

add_executable(tiff2jpeg
  main.cpp
  metadata.cpp
  tiff2jpeg.cpp
)

target_include_directories(tiff2jpeg
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(tiff2jpeg
  PRIVATE
    JPEG::JPEG
    TIFF::CXX
    exiv2lib
)

target_compile_options(tiff2jpeg
  PRIVATE -Werror -Wall -Wextra
)

set_target_properties(tiff2jpeg
  PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_compile_features(tiff2jpeg
  PRIVATE cxx_std_20
)
