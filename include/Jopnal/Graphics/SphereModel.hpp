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

#ifndef JOP_SPHEREMODEL_HPP
#define JOP_SPHEREMODEL_HPP

//Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Model.hpp>

//////////////////////////////////////////////

namespace jop
{

    class JOP_API SphereModel : public Model
    {
    public:
        /// \brief Creates a 3D sphere
        ///
        /// High values for sectors and rings will result in better looking results, but will
        /// affect performance heavily.
        ///
        /// \param radius The radius of the sphere
        /// \param rings How many rings will the sphere have
        /// \param sectors How many sectors the circle will get divided to
        ///
        SphereModel(const float radius, const unsigned int rings, const unsigned int sectors);
    };
}

#endif

/// \class SphereModel
/// \ingroup Graphics
///
/// Do explanation about the class