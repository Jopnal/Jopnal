//Jopnal Engine C++ Library
//Copyright(c) 2016 Team Jopnal
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

//////////////////////////////////////////////


template<typename T>
WeakReference<T> Object::getComponent()
{
    static_assert(std::is_base_of<Component, T>::value, "Object::getComponent(): Tried to get a component that doesn't inherit from jop::Component");

    const std::type_info& ti = typeid(T);

    for (auto& i : m_components)
    {
        if (typeid(*i) == ti)
            return static_ref_cast<T>(i->getReference());
    }

    return WeakReference<T>();
}

//////////////////////////////////////////////

namespace detail
{
    template<typename T, typename First = void, typename ... Args>
    struct FirstIsSame{enum{value=std::is_same<T,First>::value};};

    template<typename T, bool IsDrawable, bool FirstIsLayer>
    struct ComponentMaker
    {
        template<typename ... Args>
        static std::unique_ptr<T> make(Object& obj, Args&... args)
        {
            return std::make_unique<T>(obj, args...);
        }
    };
    template<typename T>
    struct ComponentMaker<T, true, true>
    {
        template<typename ... Args>
        static std::unique_ptr<T> make(Object& obj, Layer& layer, Args&... args)
        {
            auto ptr = std::make_unique<T>(obj, args...);
            layer.addDrawable(*ptr);
            return ptr;
        }
    };
}

template<typename T, typename ... Args>
T& Object::createComponent(Args& ... args)
{
    static_assert(std::is_base_of<Component, T>::value, "Object::createComponent(): Tried to create a component that doesn't inherit from jop::Component");
    
    m_components.emplace_back(detail::ComponentMaker<T, std::is_base_of<Drawable, T>::value && !std::is_same<Camera, T>::value && !std::is_same<LightSource, T>::value, detail::FirstIsSame<Layer, Args...>::value>::make(*this, args...));
    return static_cast<T&>(*m_components.back());
}