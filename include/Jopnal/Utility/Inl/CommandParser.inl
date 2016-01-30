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


template<typename T, bool isRef = (std::is_reference<T>::value && !std::is_arithmetic<T>::value), bool isPtr = std::is_pointer<T>::value>
struct ArgumentConverter
{
    static T convert(const std::string& from)
    {
        static_assert(!std::is_same<void, T>::value, "Command argument type not specialized");
		return from;
    }
};

// Reference
//template<typename T>
//struct ArgumentConverter<T, true, false>
//{
//    static std::reference_wrapper<T> convert(const std::string& from)
//    {
//        return std::ref(reinterpret_cast<T*>(std::strtol(from.c_str())));
//    }
//};
//
//// Pointer
//template<typename T>
//struct ArgumentConverter<T, false, true>
//{
//    static T convert(const std::string& from)
//    {
//        return reinterpret_cast<T>(std::strtol(from.c_str(), nullptr, 0));
//    }
//};

// bool
template<>
struct ArgumentConverter<bool>
{
    static bool convert(const std::string& from)
    {
        return (from == "true" ? true : (from == "false" ? false : (std::stoi(from) > 0)));
    }
};

// char
template<>
struct ArgumentConverter<char>
{
    static char convert(const std::string& from)
    {
        return from.empty() ? '0' : from[0];
    }
};

// unsigned char
template<>
struct ArgumentConverter<unsigned char>
{
    static unsigned char convert(const std::string& from)
    {
        return from.empty() ? '0' : static_cast<unsigned char>(from[0]);
    }
};

// int
template<>
struct ArgumentConverter<int>
{
    static int convert(const std::string& from)
    {
        return std::stoi(from);
    }
};

// unsigned int
template<>
struct ArgumentConverter<unsigned int>
{
    static unsigned int convert(const std::string& from)
    {
        return static_cast<unsigned int>(std::stoul(from));
    }
};

// short
template<>
struct ArgumentConverter<short>
{
    static short convert(const std::string& from)
    {
        return static_cast<short>(std::stoi(from));
    }
};

// unsigned short
template<>
struct ArgumentConverter<unsigned short>
{
    static unsigned short convert(const std::string& from)
    {
        return static_cast<unsigned short>(std::stoul(from));
    }
};

// long
template<>
struct ArgumentConverter<long>
{
    static long convert(const std::string& from)
    {
        return std::stol(from);
    }
};

// unsigned long
template<>
struct ArgumentConverter<unsigned long>
{
    static unsigned long convert(const std::string& from)
    {
        return std::stoul(from);
    }
};

// long long
template<>
struct ArgumentConverter<long long>
{
    static long long convert(const std::string& from)
    {
        return std::stoll(from);
    }
};

// unsigned long long
template<>
struct ArgumentConverter<unsigned long long>
{
    static unsigned long long convert(const std::string& from)
    {
        return std::stoull(from);
    }
};

// float
template<>
struct ArgumentConverter<float>
{
    static float convert(const std::string& from)
    {
        return std::stof(from);
    }
};

// double
template<>
struct ArgumentConverter<double>
{
    static double convert(const std::string& from)
    {
        return std::stoi(from);
    }
};

// long double
template<>
struct ArgumentConverter<long double>
{
    static long double convert(const std::string& from)
    {
        return std::stold(from);
    }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////
/*****************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////


template<typename T, typename O, typename ... Args>
std::tuple<T, O, Args...> splitIntoTuple(const std::string& argStr)
{
	auto parsed = parseArgumentPair(argStr);
	return std::tuple_cat(std::make_tuple(ArgumentConverter<T>::convert(std::get<0>(parsed))), splitIntoTuple<O, Args...>(std::get<1>(parsed)));
}

template<typename T>
std::tuple<T> splitIntoTuple(const std::string& argStr)
{
	return std::make_tuple(ArgumentConverter<T>::convert(argStr));
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
/*****************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////


template<std::size_t N>
struct ArgumentApplier
{
	template<typename F, typename Tuple, typename ... Args>
    static void apply(const F& func, const Tuple& tuple, const Args&... args)
    {
		ArgumentApplier<N - 1>::apply(func, tuple, std::get<N - 1>(tuple), args...);
    }
};

template<>
struct ArgumentApplier<0>
{
	template<typename F, typename Tuple, typename ... Args>
    static void apply(const F& func, const Tuple&, const Args&... args)
    {
		func(args...);
    }
};

template<typename F, typename ... Args>
void applyArguments(const F& func, const std::tuple<Args...>& args)
{
	ArgumentApplier<std::tuple_size<std::tuple<F, Args...>>::value - 1>::apply(func, args);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*****************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////


namespace DefaultParser
{
    template<typename ... Args>
    void parse(const std::function<void(Args...)>& func, const std::string& commandArgs)
    {
		auto tuple = splitIntoTuple<Args...>(commandArgs);

		applyArguments(func, tuple);
    }

	template<>
	void parse<>(const std::function<void()>& func, const std::string&);
}