


#include <Jopnal/Precompiled.hpp>
#include <Jopnal\Graphics\LightSource.hpp>



namespace jop
{

    LightSource::LightSource(Object& object, const std::string& ID)
        : Component(object, ID)
    {}

    LightSource& LightSource::setLightType(const LightType lightType)
    {
        m_light = lightType;

        return *this;
    }

    LightSource& getLightType()
    {
        return m_light;
    }
}