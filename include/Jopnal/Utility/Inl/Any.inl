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


template<typename T>
Any::Data<T>::Data(const T& data)
    : Concept   (),
      m_data    (data)
{}

namespace detail
{
    template<typename T, bool copyable>
    struct Copier
    {
        static void copy(Any::Data<T>& left, const Any::Data<T>& right)
        {
            left.getData() = right.getData();
        }
    };

    template<typename T>
    struct Copier<T, false>
    {
        static void copy(Any::Data<T>&, const Any::Data<T>&)
        {
            JOP_ASSERT(false, "Cannot copy an uncopyable Any object: " + std::string(typeid(T)));
        }
    };
}

template<typename T>
Any::Data<T>* Any::Data<T>::clone() const
{
    Data<T>* newData = new Data<T>;
    detail::Copier<T, std::is_copy_assignable<T>::value>::copy(*newData, *this);

    return newData;
}

template<typename T>
T& Any::Data<T>::getData()
{
    return m_data;
}

template<typename T>
const T& Any::Data<T>::getData() const
{
    return m_data;
}

template<typename T>
const std::type_info& Any::Data<T>::getType() const
{
    return typeid(m_data);
}

//////////////////////////////////////////////

template<typename T>
Any::Any(const T& data)
    : m_data(std::make_unique<Data<T>>(data))
{}

inline void invalidCastMessage(const bool valid, const std::type_info& from, const std::type_info& to)
{
    JOP_ASSERT(valid, "Tried to cast an empty Any object");
    JOP_ASSERT(from == to, "Tried to do an invalid Any cast from " + std::string(from.name()) + " to " + std::string(to.name()));
}

template<typename T>
T& Any::cast()
{
    invalidCastMessage(isValid(), getType(), typeid(T));

    return static_cast<Data<T>*>(m_data.get())->getData();
}

template<typename T>
const T& Any::cast() const
{
    invalidCastMessage(isValid(), getType(), typeid(T));

    return static_cast<const Data<T>*>(m_data.get())->getData();
}

template<typename T>
Any& Any::operator =(const T& data)
{
    if (getType() == typeid(T))
        static_cast<Data<T>*>(m_data.get())->getData() = data;

    else if (getType() == typeid(T*))
        *(static_cast<Data<T>*>(m_data.get())->getData()) = data;

    else
        m_data = std::make_unique<Data<T>>(data);

    return *this;
}

//template<typename T>
//Any& Any::operator=(T&& data)
//{
//    if (getType() == typeid(T))
//        static_cast<Data<T>*>(m_data.get())->getData() = std::move(data);
//
//    else if (getType() == typeid(T*))
//        static_cast<T&>(static_cast<Data<T>*>(m_data.get())->getData()) = std::move(data);
//
//    else
//    {
//        auto newData = std::make_unique<Data<T>>();
//        newData->getData() = std::move(data);
//
//        m_data = std::move(newData);
//    }
//
//    return *this;
//}