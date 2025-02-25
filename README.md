# gams-cpp #

gams-cpp is a C++ API for the General Algebraic Modeling System (GAMS). 
For further information about GAMS please visit [GAMS](https://www.gams.com) or the [GAMS documentation](https://www.gams.com/latest/docs/). 
gams-cpp uses [GoogleTest](https://github.com/google/googletest) which is licensed under the [Revised BSD License](https://github.com/google/googletest/blob/master/LICENSE).  

# How to build #

## 0. Minimum Requirements ##

- GAMS 49
- CMake 3.8
- any C++17 compiler with std::filesystem support (e.g. GCC 8, MSVC2017 15.7, Apple Clang 11)

## 1. Download and install GAMS ##

gams-cpp requires the GAMS low-level APIs. All those files are provided by the GAMS distribution packages, 
which are available for all major platforms. The installation package for your platform can be obtained from the [GAMS download page](https://www.gams.com/download/). 
After downloading the package please follow the latest GAMS [installation instructions](https://www.gams.com/latest/docs/UG_MAIN.html#UG_INSTALL).

**Note** By default GAMS will run in demo mode. Please check the [download page](https://www.gams.com/download/) for further details.

## 2. Get the gams-cpp source code ##

Download the gams-cpp sources from GitHub (via git or as zip archive). 
All information about the usage of this library can be found within the [GAMS C++ API documentation](https://www.gams.com/latest/docs/API_CPP_TUTORIAL.html).

## 3. Building the gams-cpp project ##

Start Qt Creator or a different IDE of your choice, open the CMakeList.txt file in the root folder and click 'Run CMake'. 
This generates the file '**gamsinclude.txt**' that defines the default location of the GAMS installation location. 
The path of the 'gamsinclude.txt' file may have to be updated to the correct GAMS installation location on your system.

On Windows the file contains:
```
GAMS_DISTRIB=C:/GAMS/49
```
On Unix it will look like:
```
GAMS_DISTRIB=$$(HOME)/gams/gams49.0_linux_x64_64_sfx
```

Then perform a 'Build All' operation (Build->Build All) to build the library as well as all examples and tests. 
Finally, the examples can be executed by triggering Run button or pressing 'Ctrl+R'.

# Dependencies and Licenses

| Dependency | License | Description |
| ------ | ------ | ------ |
| [cpr](https://github.com/libcpr/cpr) | [MIT](https://github.com/libcpr/cpr/blob/master/LICENSE) | C++ Requests is a simple wrapper around [libcurl](https://curl.se/libcurl/) inspired by the excellent [Python Requests](https://github.com/kennethreitz/requests) project. |
| [googletest](https://github.com/google/googletest) | [BSD-3-Clause](https://github.com/google/googletest/blob/main/LICENSE) | Google Testing and Mocking Framework |
| [json](https://github.com/nlohmann/json) | [MIT](https://github.com/nlohmann/json/blob/develop/LICENSE.MIT) | JSON for Modern C++ |
