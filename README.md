# gams-cpp #

gams-cpp is a C++ API for the General Algebraic Modeling System (GAMS). For further information about GAMS please visit [GAMS](https://www.gams.com) or the [GAMS documentation](https://www.gams.com/latest/docs/).

gams-cpp uses [GoogleTest](https://github.com/google/googletest) which is licensed under the [Revised BSD License](https://github.com/google/googletest/blob/master/LICENSE).

# How to build #

## 1. Download and install GAMS ##

gams-cpp requires the GAMS low-level APIs. All those files are provided by the GAMS distribution packages, which are available for all major platforms. The installation package for your platform can be obtained from the [GAMS download page](https://www.gams.com/download/). After downloading the package please follow the latest GAMS [installation instructions](https://www.gams.com/latest/docs/userguides/userguide/_u_g__i_n_s_t_a_l_l.html).

**Note** By default GAMS will run in demo mode. Please check the [download page](https://www.gams.com/download/) for further details.

## 2. Get the gams-cpp source code ##

Download the gams-cpp sources from GitHub (via git or as zip archive). All information about the usage of this library can be found within the [GAMS API documentation](https://www.gams.com/latest/docs/apis/index.html).

## 3. Building the gams-cpp project ##

Start Qt Creator or a different IDE of your choice, open the CMakeList.txt file in the root folder and click 'Run CMake'. This generates the file '**gamsinclude.pri**' that defines the default location of the GAMS installation location. The path of the 'gamsinclude.pri' file may have to be updated to the correct GAMS installation location on your system.

On Windows the file contains:
```
GAMS_DISTRIB=C:/GAMS/36
GAMS_DISTRIB_C_API=C:/GAMS/36/apifiles/C/api
GAMS_DISTRIB_CPP_API=C:/GAMS/36/apifiles/C++/api
```
On Unix it will look like:
```
GAMS_DISTRIB=$$(HOME)/gams/gams36.0_linux_x64_64_sfx
GAMS_DISTRIB_C_API=$$(HOME)/gams/gams36.0_linux_x64_64_sfx/apifiles/C/api
GAMS_DISTRIB_CPP_API=$$(HOME)/gams/gams36.0_linux_x64_64_sfx/apifiles/C++/api
```

Then perform a 'Build All' operation (Build->Build All) to build the library as well as all examples and tests. Finally, the examples can be executed by triggering Run button or pressing 'Ctrl+R'.
