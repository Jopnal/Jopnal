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

#ifndef JOP_HELPERMACROS_HPP
#define JOP_HELPERMACROS_HPP

//////////////////////////////////////////////


/// \brief Easy copy constructor removal
///
/// This macro must appear inside the class declaration.
///
/// \param className The name of the class
///
#define JOP_DISALLOW_COPY(className) className(const className &) = delete; \
                                     void operator =(const className &) = delete

/// \brief Easy move constructor removal
///
/// This macro must appear inside the class declaration.
///
/// \param className The name of the class
///
#define JOP_DISALLOW_MOVE(className) className(className &&) = delete; \
                                     void operator =(className &&) = delete

/// \brief Easy copy & move constructor removal
///
/// This macro must appear inside the class declaration.
///
/// \param className The name of the class
///
#define JOP_DISALLOW_COPY_MOVE(className) JOP_DISALLOW_COPY(className); \
                                          JOP_DISALLOW_MOVE(className)


/// \brief Convenience macro for defining a component's clone function
///
/// This macro must appear inside the class declaration.
///
/// \param className The name of the component
///
#define JOP_GENERIC_COMPONENT_CLONE(className)                      \
                                                                    \
    JOP_DISALLOW_COPY_MOVE(className);                              \
    className(const className& other, ::jop::Object& newObj);       \
    virtual className* clone(::jop::Object& newObj) const override  \
    {                                                               \
        return new className(*this, newObj);                        \
    }

#endif