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

#ifndef JOP_JSON_HPP
#define JOP_JSON_HPP

// Headers
#include <Jopnal/Header.hpp>

#pragma warning(push)
#pragma warning(disable: 4244)

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#pragma warning(pop)

//////////////////////////////////////////////


namespace rapidjson
{
    /// \brief Check if the document has a parse error
    ///
    /// This will also print the error into the console.
    ///
    /// \param doc The document to check
    ///
    /// \return True if no errors
    ///
    JOP_API bool checkParseError(const Document& doc);

    /// \brief Json array iterator begin
    ///
    /// Enables the use of range iterators.
    ///
    /// \return Array iterator to the beginning
    ///
    JOP_API Value::ValueIterator begin(Value& val);

    /// \brief Json array iterator begin
    ///
    /// Enables the use of range iterators.
    ///
    /// \return Array iterator to the end
    ///
    /// \see begin()
    ///
    JOP_API Value::ValueIterator end(Value& val);

    /// \copydoc begin(Value&)
    ///
    JOP_API Value::ConstValueIterator begin(const Value& val);

    /// \copydoc end(Value&)
    ///
    JOP_API Value::ConstValueIterator end(const Value& val);
}

namespace jop
{
    /// Alias for the rapidjson namespace
    ///
    namespace json = rapidjson;
}

#endif