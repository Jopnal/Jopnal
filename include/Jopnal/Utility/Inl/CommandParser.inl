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


template<typename T, bool IsEnum = std::is_enum<T>::value>
struct ArgumentConverter
{
    static T convert(const std::string& from)
    {
        return T(from);
    }
};
template<>
struct ArgumentConverter<int, false>
{
    static int convert(const std::string& from)
    {
        return from.empty() ? 0 : std::stoi(from);
    }
};
template<typename T>
struct ArgumentConverter<T, true>
{
    static T convert(const std::string& from)
    {
        return static_cast<T>(ArgumentConverter<int>::convert(from));
    }
};
template<typename T>
struct ArgumentConverter<T*, false>
{
    static T* convert(const std::string& from)
    {
        return reinterpret_cast<T*>(std::strtol(from.c_str(), nullptr, 16));
    }
};
template<typename T>
struct ArgumentConverter<std::reference_wrapper<T>, false>
{
    static std::reference_wrapper<T> convert(const std::string& from)
    {
        return std::reference_wrapper<T>(*(reinterpret_cast<T*>(std::strtol(from.c_str(), nullptr, 16))));
    }
};
template<>
struct ArgumentConverter<bool, false>
{
    static bool convert(const std::string& from)
    {
        std::string copy(from);

        std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
        std::istringstream is(copy);

        bool b;
        is >> std::boolalpha >> b;
        return b;
    }
};
template<>
struct ArgumentConverter<const char*, false>
{
    static const char* convert(const std::string& from)
    {
        return from.c_str();
    }
};
template<>
struct ArgumentConverter<char, false>
{
    static char convert(const std::string& from)
    {
        return from.empty() ? '0' : from[0];
    }
};
template<>
struct ArgumentConverter<unsigned char, false>
{
    static unsigned char convert(const std::string& from)
    {
        return static_cast<unsigned char>(ArgumentConverter<char>::convert(from));
    }
};
template<>
struct ArgumentConverter<unsigned int, false>
{
    static unsigned int convert(const std::string& from)
    {
        return from.empty() ? 0u : std::stoul(from);
    }
};
template<>
struct ArgumentConverter<unsigned long, false>
{
    static unsigned long convert(const std::string& from)
    {
        return from.empty() ? 0u : std::stoul(from);
    }
};
template<>
struct ArgumentConverter<long, false>
{
    static long convert(const std::string& from)
    {
        return from.empty() ? 0 : std::stol(from);
    }
};
template<>
struct ArgumentConverter<unsigned long long, false>
{
    static unsigned long long convert(const std::string& from)
    {
        return from.empty() ? 0u : std::stoull(from);
    }
};
template<>
struct ArgumentConverter<long long, false>
{
    static long long convert(const std::string& from)
    {
        return from.empty() ? 0 : std::stoll(from);
    }
};
template<>
struct ArgumentConverter<float, false>
{
    static float convert(const std::string& from)
    {
        return from.empty() ? 0.f : std::stof(from);
    }
};
template<>
struct ArgumentConverter<double, false>
{
    static double convert(const std::string& from)
    {
        return from.empty() ? 0.0 : std::stod(from);
    }
};
template<>
struct ArgumentConverter<long double, false>
{
    static long double convert(const std::string& from)
    {
        return from.empty() ? 0.0 : std::stold(from);
    }
};

//////////////////////////////////////////////

template<typename T, bool IsReference = std::is_lvalue_reference<T>::value>
struct RealType
{
    typedef T type;
};
template<typename T>
struct RealType<T, true>
{
    typedef std::reference_wrapper<typename std::remove_reference<T>::type> type;
};
template<>
struct RealType<const std::string&, true>
{
    typedef std::string type;
};

//////////////////////////////////////////////

template<typename T, bool IsReference = std::is_lvalue_reference<T>::value>
struct ApplyConverter
{
    static T convert(const T from)
    {
        return from;
    }
};
template<typename T>
struct ApplyConverter<T, true>
{
    static T& convert(T from)
    {
        return from;
    }
    static T& convert(std::reference_wrapper<T>& from)
    {
        return from.get();
    }
};

//////////////////////////////////////////////

namespace ArgumentApplier
{
    template<std::size_t N>
    struct Helper
    {
        template<typename Func, typename Tuple, typename ... Args>
        static void apply(const Func& func, const Tuple& tuple, const Args&... args)
        {
            Helper<N - 1>::apply(func, tuple, std::get<N - 1>(tuple), args...);
        }
    };
    template<>
    struct Helper<0>
    {
        template<typename Func, typename Tuple, typename... Args>
        static void apply(const Func& func, const Tuple&, const Args&... args)
        {
            func(ApplyConverter<Args>::convert(args)...);
        }
    };

    template<typename Func, typename ... Args>
    void apply(const Func& func, const std::tuple<Args...>& args)
    {
        Helper<std::tuple_size<std::tuple<Func, Args...>>::value - 1>::apply(func, args);
    }

    //////////////////////////////////////////////

    template<typename T, std::size_t N>
    struct MemberHelper
    {
        template<typename Func, typename Tuple, typename ... Args>
        static void apply(const Func& func, T& instance, const Tuple& tuple, const Args&... args)
        {
            MemberHelper<T, N - 1>::apply(func, instance, tuple, std::get<N - 1>(tuple), args...);
        }
    };
    template<typename T>
    struct MemberHelper<T, 0>
    {
        template<typename Func, typename Tuple, typename... Args>
        static void apply(const Func& func, T& instance, const Tuple&, const Args&... args)
        {
            func(instance, ApplyConverter<Args>::convert(args)...);
        }
    };

    template<typename Func, typename T, typename ... Args>
    void applyMember(const Func& func, T& instance, const std::tuple<Args...>& args)
    {
        MemberHelper<T, std::tuple_size<std::tuple<Func, Args...>>::value - 1>::apply(func, instance, args);
    }
}

//////////////////////////////////////////////

namespace DefaultParser
{
    template<typename Left, typename ... Rest>
    struct Splitter
    {
        static std::tuple<Left, Rest...> split(const std::string& args)
        {
            std::tuple<std::string, std::string> parsedArgs = splitFirstArguments(args);
            return std::tuple_cat(std::make_tuple(ArgumentConverter<Left>::convert(std::get<0>(parsedArgs))), Splitter<Rest...>::split(std::get<1>(parsedArgs)));
        }
    };
    template<typename Rest>
    struct Splitter<Rest>
    {
        static std::tuple<Rest> split(const std::string& arg)
        {
            return std::make_tuple(ArgumentConverter<Rest>::convert(arg));
        }
    };

    template<typename Ret, typename ... Args>
    struct Helper
    {
        static void parse(const std::function<Ret(Args...)>& func, const std::string& args)
        {
            ArgumentApplier::apply<decltype(func), typename RealType<Args>::type...>(func, Splitter<typename RealType<Args>::type...>::split(args));
        }
    };
    template<typename Ret>
    struct Helper<Ret>
    {
        static void parse(const std::function<Ret()>& func, const std::string&)
        {
            func();
        }
    };

    template<typename Ret, typename ... Args>
    void parse(const std::function<Ret(Args...)>& func, const std::string& args)
    {
        Helper<Ret, Args...>::parse(func, args);
    }

    //////////////////////////////////////////////

    template<typename Ret, typename T, typename ... Args>
    struct MemberHelper
    {
        static void parse(const std::function<Ret(T&, Args...)>& func, const std::string& args, T& instance)
        {
            ArgumentApplier::applyMember<decltype(func), T, typename RealType<Args>::type...>(func, instance, Splitter<typename RealType<Args>::type...>::split(args));
        }
    };
    template<typename Ret, typename T>
    struct MemberHelper<Ret, T>
    {
        static void parse(const std::function<Ret(T&)>& func, const std::string&, T& instance)
        {
            func(instance);
        }
    };

    template<typename Ret, typename T, typename ... Args>
    void parseMember(const std::function<Ret(T&, Args...)>& func, const std::string& args, void* instance)
    {
        if (instance)
        {
            MemberHelper<Ret, T, Args...>::parse(func, args, *static_cast<T*>(instance));
        }
    }
}