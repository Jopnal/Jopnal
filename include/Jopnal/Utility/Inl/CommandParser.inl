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


template<typename T, bool isEnum = std::is_enum<T>::value>
struct ArgumentConverter
{
    static T convert(const std::string& from)
    {
        return T(from);
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
        JOP_ASSERT(!from.empty(), "Tried to convert empty argument to char!");
        return from[0];
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
        return std::stoul(from);
    }
};
template<>
struct ArgumentConverter<int, false>
{
    static int convert(const std::string& from)
    {
        return std::stoi(from);
    }
};
template<>
struct ArgumentConverter<unsigned long, false>
{
    static unsigned long convert(const std::string& from)
    {
        return std::stoul(from);
    }
};
template<>
struct ArgumentConverter<long, false>
{
    static long convert(const std::string& from)
    {
        return std::stol(from);
    }
};
template<>
struct ArgumentConverter<unsigned long long, false>
{
    static unsigned long long convert(const std::string& from)
    {
        return std::stoull(from);
    }
};
template<>
struct ArgumentConverter<long long, false>
{
    static long long convert(const std::string& from)
    {
        return std::stoll(from);
    }
};
template<>
struct ArgumentConverter<float, false>
{
    static float convert(const std::string& from)
    {
        return std::stof(from);
    }
};
template<>
struct ArgumentConverter<double, false>
{
    static double convert(const std::string& from)
    {
        return std::stod(from);
    }
};
template<>
struct ArgumentConverter<long double, false>
{
    static long double convert(const std::string& from)
    {
        return std::stold(from);
    }
};

//////////////////////////////////////////////

template<typename T, bool isReference = std::is_lvalue_reference<T>::value>
struct RealType
{
    typedef T type;
};
template<typename T>
struct RealType<T, true>
{
    typedef std::reference_wrapper<T> type;
};
//template<typename T>
//struct RealType<std::weak_ptr<T>, false>
//{
//    typedef std::weak_ptr<T> type;
//};
template<>
struct RealType<const std::string&, true>
{
    typedef std::string type;
};

//////////////////////////////////////////////

namespace ArgumentApplier
{
    template<typename Ret, std::size_t N>
    struct Helper
    {
        template<typename Func, typename Tuple, typename ... Args>
        static Ret apply(const Func& func, const Tuple& tuple, const Args&... args)
        {
            return Helper<Ret, N - 1>::apply(func, tuple, std::get<N - 1>(tuple), args...);
        }
    };
    template<typename Ret>
    struct Helper<Ret, 0>
    {
        template<typename Func, typename Tuple, typename... Args>
        static Ret apply(const Func& func, const Tuple&, const Args&... args)
        {
            return func(args...);
        }
    };


    template<typename Ret, typename Func, typename ... Args>
    Ret apply(const Func& func, const std::tuple<Args...>& args)
    {
        return Helper<Ret, std::tuple_size<std::tuple<Func, Args...>>::value - 1>::apply(func, args);
    }

    //////////////////////////////////////////////

    template<typename Ret, typename T, std::size_t N>
    struct MemberHelper
    {
        template<typename Func, typename Tuple, typename ... Args>
        static Ret apply(const Func& func, T& instance, const Tuple& tuple, const Args&... args)
        {
            return MemberHelper<Ret, T, N - 1>::apply(func, instance, tuple, std::get<N - 1>(tuple), args...);
        }
    };
    template<typename Ret, typename T>
    struct MemberHelper<Ret, T, 0>
    {
        template<typename Func, typename Tuple, typename... Args>
        static Ret apply(const Func& func, T& instance, const Tuple&, const Args&... args)
        {
            return func(instance, args...);
        }
    };

    template<typename Ret, typename Func, typename T, typename ... Args>
    Ret applyMember(const Func& func, T& instance, const std::tuple<Args...>& args)
    {
        return MemberHelper<Ret, T, std::tuple_size<std::tuple<Func, Args...>>::value - 1>::apply(func, instance, args);
    }
}

//////////////////////////////////////////////

namespace ArgumentSplitter
{
    template<typename Left, typename Right, typename ... Rest>
    std::tuple<Left, Right, Rest...> split(const std::string& args)
    {
        std::tuple<std::string, std::string> parsedArgs = splitFirstArguments(args);
        return std::tuple_cat(std::make_tuple(ArgumentConverter<Left>::convert(std::get<0>(parsedArgs))), split<Right, Rest...>(std::get<1>(parsedArgs)));
    }

    template<typename Last>
    std::tuple<Last> split(const std::string& arg)
    {
        return std::make_tuple(ArgumentConverter<Last>::convert(arg));
    }
}

//////////////////////////////////////////////

namespace DefaultParser
{
    template<typename Ret, typename ... Args>
    void parse(const std::function<Ret(Args...)>& func, const std::string& args, Any& returnWrap)
    {
        Helper<Ret, Args...>::parse(func, args, returnWrap);
    }

    template<typename Ret, typename ... Args>
    struct Helper
    {
        static void parse(const std::function<Ret(Args...)>& func, const std::string& args, Any& returnWrap)
        {
            if (returnWrap)
                returnWrap = ArgumentApplier::apply<Ret, decltype(func), typename RealType<Args>::type...>(func, ArgumentSplitter::split<typename RealType<Args>::type...>(args));
            else
                ArgumentApplier::apply<Ret, decltype(func), typename RealType<Args>::type...>(func, ArgumentSplitter::split<typename RealType<Args>::type...>(args));
        }
    };

    template<typename ... Args>
    struct Helper<void, Args...>
    {
        static void parse(const std::function<void(Args...)>& func, const std::string& args, Any&)
        {
            ArgumentApplier::apply<void, decltype(func), typename RealType<Args>::type...>(func, ArgumentSplitter::split<typename RealType<Args>::type...>(args));
        }
    };

    template<typename Ret>
    struct Helper<Ret>
    {
        static void parse(const std::function<Ret()>& func, const std::string&, Any& returnWrap)
        {
            if (returnWrap)
                returnWrap = func();
            else
                func();
        }
    };

    template<>
    struct Helper<void>
    {
        static void parse(const std::function<void(void)>& func, const std::string&, jop::Any&)
        {
            func();
        }
    };

    //////////////////////////////////////////////

    template<typename Ret, typename T, typename ... Args>
    void parseMember(const std::function<Ret(T&, Args...)>& func, const std::string& args, Any& returnWrap, Any& instance)
    {
        JOP_ASSERT(instance && instance == typeid(T*), "Called a member command with invalid instance type!");
        MemberHelper<Ret, T, Args...>::parse(func, args, returnWrap, instance);
    }

    template<typename Ret, typename T, typename ... Args>
    struct MemberHelper
    {
        static void parse(const std::function<Ret(T&, Args...)>& func, const std::string& args, Any& returnWrap, Any& instance)
        {
            if (returnWrap)
                returnWrap = ArgumentApplier::applyMember<Ret, decltype(func), T, typename RealType<Args>::type...>(func, *(instance.cast<T*>()), ArgumentSplitter::split<typename RealType<Args>::type...>(args));
            else
                ArgumentApplier::applyMember<Ret, decltype(func), T, typename RealType<Args>::type...>(func, *(instance.cast<T*>()), ArgumentSplitter::split<typename RealType<Args>::type...>(args));
        }
    };

    template<typename T, typename ... Args>
    struct MemberHelper<void, T, Args...>
    {
        static void parse(const std::function<void(T&, Args...)>& func, const std::string& args, Any&, Any& instance)
        {
            ArgumentApplier::applyMember<void, decltype(func), T, typename RealType<Args>::type...>(func, *(instance.cast<T*>()), ArgumentSplitter::split<typename RealType<Args>::type...>(args));
        }
    };

    template<typename Ret, typename T>
    struct MemberHelper<Ret, T>
    {
        static void parse(const std::function<Ret(T&)>& func, const std::string&, Any& returnWrap, Any& instance)
        {
            if (returnWrap)
                returnWrap = func(*(instance.cast<T*>()));
            else
                func(*(instance.cast<T*>()));
        }
    };

    template<typename T>
    struct MemberHelper<void, T>
    {
        static void parse(const std::function<void(T&)>& func, const std::string&, Any&, Any& instance)
        {
            func(*(instance.cast<T*>()));
        }
    };
}