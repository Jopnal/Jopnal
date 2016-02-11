

///////////////////////////////////////////

#ifndef JOP_LIGHTSOURCE_HPP
#define JOP_LIGHTSOURCE_HPP

//Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Component.hpp>
#include <string>
#include <Jopnal\Graphics\Color.hpp>

///////////////////////////////////////////


namespace jop
{
    class JOP_API LightSource : public jop::Component
    {
    public:

        enum class LightType
        {
            Point,
            Spot,
            Directional
        };

    protected:
         Color diffuse;
         Color ambient;
         Color specular;

         bool m_shadow;



    public:
        LightSource(Object& object, const std::string& ID);

        LightSource& setLightType(const LightType lightType);

        LightSource& getLightType();

        //LightSource& setShadow(const bool);

    };
}

#endif