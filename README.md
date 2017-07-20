# GAMS-cpp #

GAMS-cpp is a C++ API for GAMS (General Algebraic Modeling System). For further information about GAMS please visit [GAMS](https://www.gams.com) or the [GAMS documentation](https://www.gams.com/latest/docs/).

GAMS-cpp uses [Qt](https://www.qt.io/) which is licensed under [LGPL](https://www.gnu.org/licenses/lgpl-3.0.en.html). 

# Contribute #

Contributions to GAMS-cpp are highly appreciated! To submit your improvements please use [GitHub](https://github.com/GAMS-dev) to collaborate.

# How to build #

## 1. Download and install Qt ##

GAMS-cpp relies on Qt for platform independent handling of directories and also makes use of the Qt Test framework. The recommended way to get Qt is via its official [Qt online installer](https://www.qt.io/). If you are interested in the Qt sources you can download them through the installer and build Qt from scratch by following the [Qt documentation](https://doc.qt.io/qt-5/build-sources.html). Alternatively, you can get and build Qt from the official [Qt GitHub mirror](https://github.com/qt/qt5).

## 2. Get the GAMS-cpp source code ##

Download the GAMS-cpp sources from GitHub (via git or as zip archive). Alternatively, you can use the sources included in the [GAMS distribution](https://www.gams.com/download/). All information about the GAMS-cpp distribution version can be found within the [GAMS API documentation](https://www.gams.com/latest/docs/apis/index.html).

## 3. Building the GAMS-cpp project ##

Start Qt Creator and open the project file 'GAMS-cpp.pro'. Then perform a 'Build All' operation (Build->Build All) to build the library as well as all examples and tests.
