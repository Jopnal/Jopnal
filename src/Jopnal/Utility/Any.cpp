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

// Headers
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace jop
{
    Any::Concept::~Concept()
    {}

    //////////////////////////////////////////////

    Any::Any(const Any& other)
        : m_data()
    {
        *this = other;
    }

    Any::Any(std::nullptr_t)
        : m_data()
    {}

    Any& Any::operator =(const Any& other)
    {
        if (other.isValid())
            m_data = std::unique_ptr<Concept>(other.m_data->clone());

        return *this;
    }

    //////////////////////////////////////////////

    void Any::clear()
    {
        m_data.reset();
    }

    //////////////////////////////////////////////

    bool Any::isValid() const
    {
        return m_data.operator bool();
    }

    //////////////////////////////////////////////

    Any::operator bool() const
    {
        return isValid();
    }

    //////////////////////////////////////////////

    const std::type_info& Any::getType() const
    {
        return isValid() ? m_data->getType() : typeid(void);
    }

    //////////////////////////////////////////////

    bool Any::operator ==(const std::type_info& type) const
    {
        return getType() == type;
    }
}