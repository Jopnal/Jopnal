// This example will create a wall of boxes and smashes it with a wrecking ball.
// 3D-version. No textures used, only debug drawing.

// Jopnal.hpp contains all engine functionality.
#include <Jopnal/Jopnal.hpp>

// Let's define our own scene.
class MyScene : public jop::Scene
{
private:

    // Member variables
    jop::WeakReference<jop::Object> m_cam;
    jop::WeakReference<jop::Object> m_crane;
    jop::WeakReference<jop::Object> m_house;

    //////////////////////////////////////////////

    void createGround(const float ground)
    {
        // Let's create a floor so the wall doesn't fall to the void.

        // The floor is made from two triangles, and creates an area from negative 'ground' to positive 'ground' around the origin of the world.
        std::vector<glm::vec3> floor =
        {
            glm::vec3(ground, 0.f, ground),
            glm::vec3(ground, 0.f, -ground),
            glm::vec3(-ground, 0.f, ground),
            glm::vec3(-ground, 0.f, -ground),
            glm::vec3(-ground, 0.f, ground),
            glm::vec3(ground, 0.f, -ground)
        };

        jop::RigidBody::ConstructInfo groundInfo(jop::ResourceManager::getNamed<jop::TerrainShape>("ground", floor));
        createChild("ground")->createComponent<jop::RigidBody>(getWorld<3>(), groundInfo);
    }

    //////////////////////////////////////////////

    void createWall(const unsigned int wall)
    {
        // Scene will have a child called "house".
        m_house = createChild("house");

        // Box sizes.
        float h_box = wall / 10.f;
        float f_box = h_box * 2.f;

        // Build supporting walls to both sides of the boxes.
        jop::RigidBody::ConstructInfo supportWallInfo(jop::ResourceManager::getNamed<jop::BoxShape>("stWall", glm::vec3(1.f, wall + h_box, h_box)), jop::RigidBody::Type::Static);

        // Creating a child, setting its new position and creating a body to it at once. Split into several lines for easier reading.
        m_house->createChild("lWall")
               ->setPosition(-(int)wall - f_box, wall * 0.5f, 0.f)
                .createComponent<jop::RigidBody>(getWorld<3>(), supportWallInfo);

        // Second wall, in one line.
        m_house->createChild("RWall")->setPosition(wall - f_box, wall * 0.5f, 0.f).createComponent<jop::RigidBody>(getWorld<3>(), supportWallInfo);

        jop::RigidBody::ConstructInfo boxInfo(jop::ResourceManager::getNamed<jop::BoxShape>("box", f_box), jop::RigidBody::Type::Dynamic, 5.f);
        boxInfo.restitution = 0.15f;

        // Rows and columns of boxes.
        for (unsigned int i = 0; i < wall / h_box; ++i)
        {
            for (unsigned int j = 0; j < wall / f_box; ++j)
            {
                // Positions of the boxes.
                float x = float(i - float(wall)) + float(h_box * i) + h_box;
                float y = j + float(h_box * (float(j) - 1.f)) + f_box;

                // All the boxes will be children of the child "house".
                auto b = m_house->createChild("");
                b->setPosition(x, y, 0.f);
                b->createComponent<jop::RigidBody>(getWorld<3>(), boxInfo);
            }
        }
    }

    //////////////////////////////////////////////

    void createWreckingBall(const unsigned int chain)
    {
        // We'll need at least one static body so the ball and chain don't fall straight to the ground.
        jop::RigidBody::ConstructInfo craneInfo(jop::ResourceManager::getNamed<jop::BoxShape>("crane", glm::vec3(1.f, 1.f, 1.f)), jop::RigidBody::Type::Static);
        m_crane = createChild("crane");
        m_crane->setPosition(0.f, 60.f, 0.f);
        m_crane->createComponent<jop::RigidBody>(getWorld<3>(), craneInfo);

        // We'll use this shape for all the links in the chain.
        jop::RigidBody::ConstructInfo chainInfo(jop::ResourceManager::getNamed<jop::BoxShape>("chain", glm::vec3(1.f, 1.f, 3.f)), jop::RigidBody::Type::Dynamic, 80.f);
        for (unsigned int i = 0; i < chain; ++i)
        {
            // All the pieces of the chain are children of the child "crane".
            auto c = m_crane->createChild("");
            c->setPosition(0.f, 0.f, 4.f + (4.f * (float)i));
            c->createComponent<jop::RigidBody>(getWorld<3>(), chainInfo);
        }

        // Let's link the chain parts one after another.
        for (unsigned int i = 0; i < chain - 1; ++i)
        {
            auto* cLink = m_crane->getChildren()[i].getComponent<jop::RigidBody>();
            auto* nLink = m_crane->getChildren()[i + 1].getComponent<jop::RigidBody>();

            cLink->link<jop::RopeJoint>(*nLink, false);
        }

        // Create a wrecking ball to the end of the chain.
        jop::RigidBody::ConstructInfo wrballInfo(jop::ResourceManager::getNamed<jop::SphereShape>("wrball", glm::pi<float>() * 1.5f), jop::RigidBody::Type::Dynamic, 160.f);
        auto wrBall = createChild("wrBall");
        wrBall->setPosition(0.f, 60.f, 10.f + (4.f * (float)chain));
        wrBall->createComponent<jop::RigidBody>(getWorld<3>(), wrballInfo);

        // Connect the static body to the beginning of the chain.
        m_crane->getComponent<jop::RigidBody>()->link<jop::RopeJoint>(*m_crane->getChildren().begin()->getComponent<jop::RigidBody>(), true);

        // And finally connect the end of the chain to the wrecking ball.
        m_crane->getChildren().back().getComponent<jop::RigidBody>()->link<jop::RopeJoint>(*wrBall->getComponent<jop::RigidBody>(), true);
    }

public:

    MyScene()
        : jop::Scene    ("MyScene"),
          m_cam         (),
          m_crane       (),
          m_house       ()
    {
        // Create a camera object. If you don't do this, nothing will be drawn. The camera
        // will, by default, be positioned at [0,0,0] and it'll point directly at [0,0,-1].
        m_cam = createChild("cam");
        m_cam->createComponent<jop::Camera>(getRenderer(), jop::Camera::Projection::Perspective);

        // New position and rotation of 160 degrees around Y-axis.
        m_cam->setPosition(20.f, 10.f, -120.f).setRotation(glm::quat(0.174f, 0.f, 0.985f, 0.f));

        // Activate debug mode so we can see what is happening.
        getWorld<3>().setDebugMode(true);

        // Sizes to use.
        const float ground = 100.f;
        const unsigned int wall = 20;
        const unsigned int chain = 10;

        // Creating the main thing of this demo.
        // Split into separate functions to keep everything easier to read.
        createGround(ground);
        createWall(wall);
        createWreckingBall(chain);
    }

    //////////////////////////////////////////////

    // Pre-update will be called before objects are updated
    void preUpdate(const float /* deltaTime */) override
    {
        // We'll delete all the pieces of wall that drop off from the ground plate
        for (auto itr = m_house->getChildren().begin(); itr != m_house->getChildren().end(); ++itr)
            if (itr->getGlobalPosition().y < -10.f)
                itr->removeSelf();
    }
};

// Standard main() can be used, as long as jopnal-main.lib has been linked.
int main(int argc, char* argv[])
{
    // Initialize the engine.
    JOP_ENGINE_INIT("wrecking_ball_example", argc, argv);

    // Create our scene.
    jop::Engine::createScene<MyScene>();

    // Finally run the main loop. The program can be closed with alt + F4 or with the escape button.
    return JOP_MAIN_LOOP;
}