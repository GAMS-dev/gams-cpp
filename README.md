# gams-cpp #

gams-cpp is a C++ API for the General Algebraic Modeling System (GAMS). For further information about GAMS please visit [GAMS](https://www.gams.com) or the [GAMS documentation](https://www.gams.com/latest/docs/).

gams-cpp uses [Qt](https://www.qt.io/) which is licensed under [LGPL](https://www.gnu.org/licenses/lgpl-3.0.en.html). 

# Contribute #

Contributions to gams-cpp are highly appreciated! To submit your improvements please use [GitHub](https://github.com/GAMS-dev) to collaborate.

# How to build #

## 1. Download and install Qt ##

gams-cpp relies on Qt for platform independent handling of directories and also makes use of the Qt Test framework. The recommended way to get Qt is via its official [Qt online installer](https://www.qt.io/). If you are interested in the Qt sources you can download them through the installer and build Qt from scratch by following the [Qt documentation](https://doc.qt.io/qt-5/build-sources.html). Alternatively, you can get and build Qt from the official [Qt GitHub mirror](https://github.com/qt/qt5).

## 2. Download and install GAMS ##

gams-cpp requires the GAMS low-level APIs. All those files are provided by the GAMS distribution packages, which are available for all major platforms. The installation package for your platform can be obtained from the [GAMS download page](https://www.gams.com/download/). After downloading the package please follow the latest GAMS [installation instructions](https://www.gams.com/latest/docs/userguides/userguide/_u_g__i_n_s_t_a_l_l.html).

**Note** By default GAMS will run in demo mode. Please check the [download page](https://www.gams.com/download/). for further details.

## 3. Get the gams-cpp source code ##

Download the gams-cpp sources from GitHub (via git or as zip archive). All information about the usage of this library can be found within the [GAMS API documentation](https://www.gams.com/latest/docs/apis/index.html).

## 4. Building the gams-cpp project ##

Start Qt Creator and open the project file 'gams-cpp.pro'. Depending on the GAMS installation location the following lines of the 'dependency.pri' file have to be updated to the correct GAMS installation location.

```
unix:GAMS_DISTRIB = $$(HOME)/gams/gams24.9_linux_x64_64_sfx/apifiles/C/api
win32:GAMS_DISTRIB = C:\GAMS\win64\24.9\apifiles\C\api
```

Then perform a 'Build All' operation (Build->Build All) to build the library as well as all examples and tests. Finally, the examples can be executed by triggering Run button or pressing 'Ctrl+R'.
