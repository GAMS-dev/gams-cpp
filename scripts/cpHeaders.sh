# GAMS - General Algebraic Modeling System C++ API
#
# GAMS - General Algebraic Modeling System C++ API
#
# Copyright (c) 2017 GAMS Software GmbH <support@gams.com>
# Copyright (c) 2017 GAMS Development Corp. <support@gams.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
#!/bin/bash

USAGE="Copy the gitlab GAMS_cpp headers to a destination path.\n"
USAGE+="\nUsage: cpHeaders src_path dest_path\n"
USAGE+="\nMandatory arguments:\n"
USAGE+="  src_path     source path of the headers.\n"
USAGE+="  dest_path    destination path for the headers.\n"

SRC_PATH=$1
if [ -z "$SRC_PATH" ]; then
    echo "Please set the api-header source path."
    echo $USAGE
    exit 1
fi

DEST_PATH=$2
if [ -z "$DEST_PATH" ]; then
    echo "Please set the api-header destination path."
    echo $USAGE
    exit 1
fi

cp $SRC_PATH/*.h $DEST_PATH
rm -f $DEST_PATH/*impl.h
rm -f $DEST_PATH/gamspath.h
rm -f $DEST_PATH/solveroptions.h
