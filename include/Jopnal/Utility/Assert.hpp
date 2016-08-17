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

#ifndef JOP_ASSERT_HPP
#define JOP_ASSERT_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <string>

//////////////////////////////////////////////


namespace jop
{
    #ifdef JOP_ENABLE_ASSERTS
        #define JOP_ASSERT(expression, message) ::jop::assertion((expression), __FILE__, __LINE__, (message))
        #define JOP_ASSERT_EVAL(expression, message) JOP_ASSERT((expression), (message))
    #else
        #define JOP_ASSERT(expression, message)
        #define JOP_ASSERT_EVAL(expression, message) (expression)
    #endif

    /// \brief Custom assertion function
    ///
    /// This function, should the assertion fail, opens a customized message box and exits the program.
    ///
    /// \param expression The expression to be tested
    /// \param file File name
    /// \param line The line number
    /// \param message Description of the error
    ///
    JOP_API void assertion(const bool expression, const std::string& file, unsigned int line, const std::string& message);
}

/// \def JOP_ASSERT
/// \ingroup utility
///
/// \brief Custom assertion macro
///
/// \param expression The expression to evaluate, which is required to be true
///                   if the program is to continue
/// \param message The error message to print in case of assertion failure

/// \def JOP_ASSERT_EVAL
/// \ingroup utility
///
/// This version of the assertion macro will evaluete the expression even when
/// assertions have been turned off.
///
/// \copydoc JOP_ASSERT

#endif
