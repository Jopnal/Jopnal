// Jopnal.hpp contains all engine functionality.
#include <Jopnal/Jopnal.hpp>

// Let's define our own scene.
class MyScene : public jop::Scene
{
private:

    // Reference to out box object. This will be filled in the constructor.
    jop::WeakReference<jop::Object> m_object;

public:

    MyScene()
        : jop::Scene("MyScene"),
          m_object()
    {
        // Create a camera object. If you don't do this, nothing will be drawn. The camera
        // will, by default, be positioned at [0,0,0] and it'll point directly at [0,0,-1].
        createChild("cam")->createComponent<jop::Camera>(getRenderer(), jop::Camera::Projection::Perspective);

        // Let's create our box object and store its reference
        m_object = createChild("box");

        // Create a GenericDrawable component. This will actually add the functionality for drawing the box.
        // We don't need to do anything else here, as the default mesh, material and shader will be used
        // automatically.
        m_object->createComponent<jop::Drawable>(getRenderer());

        // Move the object further away from the camera, so that it won't be clipped by the near clipping
        // plane.
        m_object->setPosition(0.f, 0.f, -2.5f);
    }

    // Pre-update will be called before objects are updated
    void preUpdate(const float deltaTime) override
    {
        // Rotate our object by its X and Y axes. Don't forget to multiply by the delta time.
        m_object->rotate(0.5f * deltaTime, 1.f * deltaTime, 0.f);
    }
};

// Standard main() can be used, as long as jopnal-main.lib has been linked.
int main(int argc, char* argv[])
{
    // Initialize the engine
    JOP_ENGINE_INIT("spinning_box_example", argc, argv);

    // Create our scene
    jop::Engine::createScene<MyScene>();

    // Finally run the main loop. The program can be closed by clicking the window red X button.
    return JOP_MAIN_LOOP;
}