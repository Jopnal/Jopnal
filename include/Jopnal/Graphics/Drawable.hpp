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

#ifndef JOP_DRAWABLE_HPP
#define JOP_DRAWABLE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Component.hpp>

//////////////////////////////////////////////


namespace jop
{
    class Layer;

    class JOP_API Drawable : public Component
    {
    private:

        JOP_DISALLOW_MOVE(Drawable);

        void operator =(const Drawable&) = delete;

    public:
    
        /// \brief Constructor
        ///
        /// \param object Reference to the object this drawable will be bound to
        /// \param layer Reference to the layer this drawable will be bound to
        /// \param ID Unique component identifier
        ///
        Drawable(Object& object, Layer& layer, const std::string& ID);

        /// \brief Copy constructor
        ///
        Drawable(const Drawable& other);

        /// \brief Virtual destructor
        ///
        virtual ~Drawable() = 0;


        /// \copydoc jop::Component::clone()
        ///
        virtual Drawable* clone() const override = 0;


        /// \brief Draw function
        ///
        virtual void draw() = 0;
        
    };
}

#endif

/// \class Drawable
/// \ingroup graphics
///
/// #TODO Detailed description