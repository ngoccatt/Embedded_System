# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Espressif/frameworks/esp-idf-v4.4.2/components/bootloader/subproject"
  "D:/esp/hello_world/build/bootloader"
  "D:/esp/hello_world/build/bootloader-prefix"
  "D:/esp/hello_world/build/bootloader-prefix/tmp"
  "D:/esp/hello_world/build/bootloader-prefix/src/bootloader-stamp"
  "D:/esp/hello_world/build/bootloader-prefix/src"
  "D:/esp/hello_world/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/esp/hello_world/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
