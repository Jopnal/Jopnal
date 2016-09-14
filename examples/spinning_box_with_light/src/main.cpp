// For info on the uncommented lines, see the spinning_box example

#include <Jopnal/Jopnal.hpp>

class MyScene : public jop::Scene
{
private:

    jop::WeakReference<jop::Object> m_object;

public:

    MyScene()
        : jop::Scene    ("MyScene"),
          m_object      ()
    {
        createChild("cam")->createComponent<jop::Camera>(getRenderer(), jop::Camera::Projection::Perspective);

        m_object = createChild("box");
        m_object->createComponent<jop::Drawable>(getRenderer());
        m_object->setPosition(0.f, 0.f, -2.5f);

        // Create an object with a directional light .
        auto light = createChild("light");
        light->createComponent<jop::LightSource>(getRenderer(), jop::LightSource::Type::Directional);

        // Move the light to the right and set it to point to the left
        // Notice that the rotation is expected to be in radians.
        light->setPosition(5.f, 0.f, 0.f).setRotation(0.f, glm::radians(90.f) /* same as glm::half_pi<float>() */, 0.f);

        auto drawable = m_object->getComponent<jop::Drawable>();

        // To modify the drawable's material, we must create a new one to replace the default.
        auto& newMaterial = jop::ResourceManager::getEmpty<jop::Material>("newMaterial");
        drawable->setMaterial(newMaterial);

        // Set the lighting model to use. By default a material will have no lighting.
        newMaterial.setLightingModel(jop::Material::LightingModel::Default);

        // Let's add the default texture for good measure.
        newMaterial.setMap(jop::Material::Map::Diffuse0, jop::Texture2D::getDefault());

        // Set the diffuse reflection.
        newMaterial.setReflection(jop::Material::Reflection::Diffuse, jop::Color::White);
    }

    void preUpdate(const float deltaTime) override
    {
        m_object->rotate(0.5f * deltaTime, 1.f * deltaTime, 0.f);
    }
};

int main(int argc, char* argv[])
{
    JOP_ENGINE_INIT("spinning_box_with_light_example", argc, argv);

    jop::Engine::createScene<MyScene>();

    return JOP_MAIN_LOOP;
}