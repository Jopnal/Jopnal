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
struct ArgumentConverter
{
    static T convert(const std::string& from)
    {
        return T(from);
    }
};
template<typename T>
struct ArgumentConverter<T*>
{
    static T* convert(const std::string& from)
    {
        return reinterpret_cast<T*>(std::strtol(from.c_str(), nullptr, 16));
    }
};
template<typename T>
struct ArgumentConverter<std::reference_wrapper<T>>
{
    static std::reference_wrapper<T> convert(const std::string& from)
    {
        return std::reference_wrapper<T>(*(reinterpret_cast<T*>(std::strtol(from.c_str(), nullptr, 16))));
    }
};
template<>
struct ArgumentConverter<const char*>
{
    static const char* convert(const std::string& from)
    {
        return from.c_str();
    }
};
template<>
struct ArgumentConverter<char>
{
    static char convert(const std::string& from)
    {
        JOP_ASSERT(!from.empty(), "Tried to convert empty argument to char!");
        return from[0];
    }
};
template<>
struct ArgumentConverter<unsigned char>
{
    static unsigned char convert(const std::string& from)
    {
        return static_cast<unsigned char>(ArgumentConverter<char>::convert(from));
    }
};
template<>
struct ArgumentConverter<unsigned int>
{
    static unsigned int convert(const std::string& from)
    {
        return std::stoul(from);
    }
};
template<>
struct ArgumentConverter<int>
{
    static int convert(const std::string& from)
    {
        return std::stoi(from);
    }
};
template<>
struct ArgumentConverter<unsigned long>
{
    static unsigned long convert(const std::string& from)
    {
        return std::stoul(from);
    }
};
template<>
struct ArgumentConverter<long>
{
    static long convert(const std::string& from)
    {
        return std::stol(from);
    }
};
template<>
struct ArgumentConverter<unsigned long long>
{
    static unsigned long long convert(const std::string& from)
    {
        return std::stoull(from);
    }
};
template<>
struct ArgumentConverter<long long>
{
    static long long convert(const std::string& from)
    {
        return std::stoll(from);
    }
};
template<>
struct ArgumentConverter<float>
{
    static float convert(const std::string& from)
    {
        return std::stof(from);
    }
};
template<>
struct ArgumentConverter<double>
{
    static double convert(const std::string& from)
    {
        return std::stod(from);
    }
};
template<>
struct ArgumentConverter<long double>
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
    void parse(const std::function<Ret(Args...)>& func, const std::string& args, PtrWrapper& returnWrap)
    {
        Helper<Ret, Args...>::parse(func, args, returnWrap);
    }

    template<typename Ret, typename ... Args>
    struct Helper
    {
        static void parse(const std::function<Ret(Args...)>& func, const std::string& args, PtrWrapper& returnWrap)
        {
            if (returnWrap && returnWrap == typeid(Ret))
                returnWrap = ArgumentApplier::apply<Ret, decltype(func), typename RealType<Args>::type...>(func, ArgumentSplitter::split<typename RealType<Args>::type...>(args));
            else
                ArgumentApplier::apply<Ret, decltype(func), typename RealType<Args>::type...>(func, ArgumentSplitter::split<typename RealType<Args>::type...>(args));
        }
    };

    template<typename ... Args>
    struct Helper<void, Args...>
    {
        static void parse(const std::function<void(Args...)>& func, const std::string& args, PtrWrapper&)
        {
            ArgumentApplier::apply<void, decltype(func), typename RealType<Args>::type...>(func, ArgumentSplitter::split<typename RealType<Args>::type...>(args));
        }
    };

    template<typename Ret>
    struct Helper<Ret>
    {
        static void parse(const std::function<Ret()>& func, const std::string& args, PtrWrapper& returnWrap)
        {
            if (returnWrap && returnWrap == typeid(Ret))
                returnWrap = func();
            else
                func();
        }
    };

    template<>
    struct Helper<void>
    {
        static void parse(const std::function<void(void)>& func, const std::string&, PtrWrapper&)
        {
            func();
        }
    };

    //////////////////////////////////////////////

    template<typename Ret, typename T, typename ... Args>
    void parseMember(const std::function<Ret(T&, Args...)>& func, const std::string& args, PtrWrapper& returnWrap, PtrWrapper& instance)
    {
        JOP_ASSERT(instance && instance == typeid(T*), "Called a member command with invalid instance type!");
        MemberHelper<Ret, T, Args...>::parse(func, args, returnWrap, instance);
    }

    template<typename Ret, typename T, typename ... Args>
    struct MemberHelper
    {
        static void parse(const std::function<Ret(T&, Args...)>& func, const std::string& args, PtrWrapper& returnWrap, PtrWrapper& instance)
        {
            if (returnWrap && returnWrap == typeid(Ret*))
                returnWrap = ArgumentApplier::applyMember<Ret, decltype(func), T, typename RealType<Args>::type...>(func, *(instance.cast<T>()), ArgumentSplitter::split<typename RealType<Args>::type...>(args));
            else
                ArgumentApplier::applyMember<Ret, decltype(func), T, typename RealType<Args>::type...>(func, *(instance.cast<T>()), ArgumentSplitter::split<typename RealType<Args>::type...>(args));
        }
    };

    template<typename T, typename ... Args>
    struct MemberHelper<void, T, Args...>
    {
        static void parse(const std::function<void(T&, Args...)>& func, const std::string& args, PtrWrapper&, PtrWrapper& instance)
        {
            ArgumentApplier::applyMember<void, decltype(func), T, typename RealType<Args>::type...>(func, *(instance.cast<T>()), ArgumentSplitter::split<typename RealType<Args>::type...>(args));
        }
    };

    template<typename Ret, typename T>
    struct MemberHelper<Ret, T>
    {
        static void parse(const std::function<Ret(T&)>& func, const std::string&, PtrWrapper& returnWrap, PtrWrapper& instance)
        {
            if (returnWrap && returnWrap == typeid(Ret*))
                returnWrap = func(*(instance.cast<T>()));
            else
                func(*(instance.cast<T>()));
        }
    };

    template<typename T>
    struct MemberHelper<void, T>
    {
        static void parse(const std::function<void(T&)>& func, const std::string&, PtrWrapper&, PtrWrapper& instance)
        {
            func(*(instance.cast<T>()));
        }
    };
}