/**
 *
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017 GAMS Development Corp. <support@gams.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <string>
#include <iostream>
#include "Transport1/transport1.h"
#include "Transport2/transport2.h"
#include "Transport3/transport3.h"
#include "Transport4/transport4.h"
#include "Transport5/transport5.h"
#include "Transport6/transport6.h"
#include "Transport7/transport7.h"
#include "Transport8/transport8.h"
#include "Transport9/transport9.h"
#include "Transport10/transport10.h"
#include "Transport11/transport11.h"
#include "Transport12/transport12.h"
#include "Transport13/transport13.h"
#include "Transport14/transport14.h"
#include "Warehouse/warehouse.h"
#include "DomainChecking/domainchecking.h"
#include <QDebug>

int main(int argc, char* argv[])
{
    try {
        qDebug() << "---------- Transport 1 --------------";
        Transport1::becomes_main(argc, argv);
        qDebug() << "---------- Transport 2 --------------";
        Transport2::becomes_main(argc, argv);
        qDebug() << "---------- Transport 3 --------------";
        Transport3::becomes_main(argc, argv);
        qDebug() << "---------- Transport 4 --------------";
        Transport4::becomes_main(argc, argv);
        qDebug() << "---------- Transport 5 --------------";
        Transport5::becomes_main(argc, argv);
        qDebug() << "---------- Transport 6 --------------";
        Transport6::becomes_main(argc, argv);
        qDebug() << "---------- Transport 7 --------------";
        Transport7::becomes_main(argc, argv);
        qDebug() << "---------- Transport 8 --------------";
        Transport8::becomes_main(argc, argv);
        qDebug() << "---------- Transport 9 --------------";
        Transport9::becomes_main(argc, argv);
        qDebug() << "---------- Transport 10 --------------";
        Transport10::becomes_main(argc, argv);
        qDebug() << "---------- Transport 11 --------------";
        Transport11::becomes_main(argc, argv);
        qDebug() << "---------- Transport 12 --------------";
        Transport12::becomes_main(argc, argv);
        qDebug() << "---------- Transport 13 --------------";
        Transport13::becomes_main(argc, argv);
        qDebug() << "---------- Transport 14 --------------";
        Transport14::becomes_main(argc, argv);
        qDebug() << "---------- Warehouse --------------";
        Warehouse::becomes_main(argc, argv);
        qDebug() << "---------- DomaniChecking --------------";
        DomainChecking::becomes_main(argc, argv);

        std::cout << "END" << std::endl;
    }
    catch (gams::GAMSException e)
    {
        std::cout << e.what() << std::endl;
    }
}
