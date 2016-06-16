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

// Headers
#include JOP_PRECOMPILED_HEADER_FILE

#ifndef JOP_PRECOMPILED_HEADER

	#include <Jopnal/Utility/Json.hpp>

    #include <Jopnal/Core/DebugHandler.hpp>

#endif

//////////////////////////////////////////////


namespace rapidjson
{
    bool checkParseError(const Document& doc)
    {
        if (!doc.HasParseError())
            return true;

        const char* errStr = "Unknown error";

        switch (doc.GetParseError())
        {
            case kParseErrorArrayMissCommaOrSquareBracket:
                errStr = "Array missing comma or square bracket";
                break;
            case kParseErrorDocumentEmpty:
                errStr = "Document empty";
                break;
            case kParseErrorDocumentRootNotSingular:
                errStr = "Document root not singular";
                break;
            case kParseErrorNumberMissExponent:
                errStr = "Number missing exponent";
                break;
            case kParseErrorNumberMissFraction:
                errStr = "Number missing fraction";
                break;
            case kParseErrorNumberTooBig:
                errStr = "Number too big";
                break;
            case kParseErrorObjectMissColon:
                errStr = "Object missing a colon";
                break;
            case kParseErrorObjectMissCommaOrCurlyBracket:
                errStr = "Object missing comma or curly bracket";
                break;
            case kParseErrorObjectMissName:
                errStr = "Object missing a name";
                break;
            case kParseErrorStringEscapeInvalid:
                errStr = "Invalid escape character";
                break;
            case kParseErrorStringInvalidEncoding:
                errStr = "Invalid encoding";
                break;
            case kParseErrorStringMissQuotationMark:
                errStr = "Missing quotation mark";
                break;
            case kParseErrorStringUnicodeEscapeInvalidHex:
                errStr = "Invalid unicode escape hex";
                break;
            case kParseErrorStringUnicodeSurrogateInvalid:
                errStr = "Invalid unicode surrogate";
                break;
            case kParseErrorTermination:
                errStr = "Parsing terminated";
        }

        JOP_DEBUG_ERROR("Failed to parse JSON document, at location " << doc.GetErrorOffset() << ": " << errStr);

        return false;
    }

    //////////////////////////////////////////////

    Value::ValueIterator begin(Value& val)
    {
        return val.Begin();
    }

    Value::ValueIterator end(Value& val)
    {
        return val.End();
    }

    Value::ConstValueIterator begin(const Value& val)
    {
        return val.Begin();
    }

    Value::ConstValueIterator end(const Value& val)
    {
        return val.End();
    }
}