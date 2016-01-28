// Jopnal Engine C++ Library
// Copyright(c) 2016 Team Jopnal
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//////////////////////////////////////////////

#ifndef JOP_RESOURCE_HPP
#define JOP_RESOURCE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <string>

//////////////////////////////////////////////

namespace jop
{
    class JOP_API Resource
    {
    public:

        /// \brief Virtual destructor
        ///
        virtual ~Resource() = 0;


        /// \brief Virtual method for using FileLoader to load new resource from file
        ///
        /// \param path Name or path for wanted resource
        ///
        virtual bool load(const std::string& path) = 0;

    };
}

#endif

/// \class Resource
/// \ingroup core
///
/// This is the base class for all resources that are loaded from files