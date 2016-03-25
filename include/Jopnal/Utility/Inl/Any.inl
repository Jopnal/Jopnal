// Jopnal Engine C++ Library
// Copyright (c) 2016 Team Jopnal
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgement in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

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

#ifdef JOP_ENABLE_ASSERTS
inline void invalidCastMessage(const bool valid, const std::type_info& from, const std::type_info& to)
{
    JOP_ASSERT(valid, "Tried to cast an empty Any object");
    JOP_ASSERT(from == to, "Tried to do an invalid Any cast from " + std::string(from.name()) + " to " + std::string(to.name()));
}
#endif

template<typename T>
T& Any::cast()
{
#ifdef JOP_ENABLE_ASSERTS
    invalidCastMessage(isValid(), getType(), typeid(T));
#endif

    return static_cast<Data<T>*>(m_data.get())->getData();
}

template<typename T>
const T& Any::cast() const
{
#ifdef JOP_ENABLE_ASSERTS
    invalidCastMessage(isValid(), getType(), typeid(T));
#endif

    return static_cast<const Data<T>*>(m_data.get())->getData();
}

template<typename T>
T Any::dynamicCast()
{
    return (isValid() ? dynamic_cast<T>(static_cast<Data<T>*>(m_data.get())->getData()) : nullptr);
}

template<typename T>
const T Any::dynamicCast() const
{
    return (isValid() ? dynamic_cast<const T>(static_cast<const Data<T>*>(m_data.get())->getData()) : nullptr);
}

template<typename T>
Any& Any::operator =(const T& data)
{
    if (getType() == typeid(T) || getType() == typeid(typename std::remove_const<T>::type))
        static_cast<Data<T>*>(m_data.get())->getData() = data;

    else
        m_data = std::make_unique<Data<T>>(data);

    return *this;
}

template<typename T>
Any& Any::operator=(T&& data)
{
    if (getType() == typeid(T) || getType() == typeid(typename std::remove_const<T>::type))
        static_cast<Data<T>*>(m_data.get())->getData() = std::move(data);

    else
    {
        auto newData = std::make_unique<Data<T>>();
        newData->getData() = std::move(data);

        m_data = std::move(newData);
    }

    return *this;
}