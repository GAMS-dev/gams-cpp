/*
 * GAMS - General Algebraic Modeling System C++ API
 *
 * Copyright (c) 2017-2020 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2020 GAMS Development Corp. <support@gams.com>
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

#ifndef GAMSLIB_GLOBAL_H
#define GAMSLIB_GLOBAL_H

#include <string>
#include <vector>
#include <memory>
#include <map>

namespace gams {

typedef void* LogId;

class GAMSCheckpointImpl;
class GAMSDatabaseImpl;
class GAMSDatabaseDomainViolationImpl;
class GAMSDomainImpl;
class GAMSJobImpl;
class GAMSModelInstanceImpl;
class GAMSModelInstanceOptImpl;
class GAMSModifierImpl;
class GAMSOptionsImpl;
class GAMSSymbolImpl;
class GAMSSymbolDomainViolationImpl;
class GAMSSymbolRecordImpl;
class GAMSWorkspaceImpl;
class GAMSWorkspaceInfoImpl;

}

#ifdef IGNORE_EXPORT

#   define LIBSPEC
#   define EXPIMP_TEMPLATE

#else // ! IGNORE_EXPORT

#ifdef _WIN32

#ifdef MAKELIB
#   define LIBSPEC __declspec(dllexport)
#else
#   define LIBSPEC __declspec(dllimport)
#endif

#ifdef _MSC_VER
class LIBSPEC std::runtime_error;
template class LIBSPEC std::shared_ptr<gams::GAMSCheckpointImpl>;
template class LIBSPEC std::shared_ptr<gams::GAMSDatabaseImpl>;
template class LIBSPEC std::shared_ptr<gams::GAMSDatabaseDomainViolationImpl>;
template class LIBSPEC std::shared_ptr<gams::GAMSDomainImpl>;
template class LIBSPEC std::shared_ptr<gams::GAMSJobImpl>;
template class LIBSPEC std::shared_ptr<gams::GAMSModelInstanceImpl>;
template class LIBSPEC std::shared_ptr<gams::GAMSModelInstanceOptImpl>;
template class LIBSPEC std::shared_ptr<gams::GAMSModifierImpl>;
template class LIBSPEC std::shared_ptr<gams::GAMSOptionsImpl>;
template class LIBSPEC std::shared_ptr<gams::GAMSSymbolImpl>;
template class LIBSPEC std::shared_ptr<gams::GAMSSymbolDomainViolationImpl>;
template class LIBSPEC std::shared_ptr<gams::GAMSSymbolRecordImpl>;
template class LIBSPEC std::shared_ptr<gams::GAMSWorkspaceImpl>;
template class LIBSPEC std::shared_ptr<gams::GAMSWorkspaceInfoImpl>;
#endif

#else  // ! _WIN32

#ifdef __GNUC__
#define LIBSPEC __attribute__((__visibility__("default")))
#else
#define LIBSPEC
#endif

#endif // ! _WIN32

#endif // ! IGNORE_EXPORT

#endif // GAMSLIB_GLOBAL_H
