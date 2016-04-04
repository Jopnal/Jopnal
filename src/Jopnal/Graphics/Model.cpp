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
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

//////////////////////////////////////////////

// Headers
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace
{
    unsigned int ns_referenceCount = 0;
    std::unique_ptr<Assimp::Importer> ns_importer;

    void createImporter()
    {
        if (ns_referenceCount++)
            return;

        struct Logger : Assimp::Logger
        {
            Logger() : Assimp::Logger(Assimp::Logger::LogSeverity::NORMAL){}
            bool attachStream(Assimp::LogStream*, unsigned int)
            {return true;}
            bool detatchStream(Assimp::LogStream*, unsigned int)
            {return true;}
            void OnDebug(const char*) override
            {}
            void OnInfo(const char* message) override
            {JOP_DEBUG_INFO(message);}
            void OnWarn(const char* message) override
            {JOP_DEBUG_WARNING(message);}
            void OnError(const char* message) override
            {JOP_DEBUG_ERROR(message);}
        };
        Assimp::DefaultLogger::set(new Logger);

        ns_importer = std::make_unique<Assimp::Importer>();
        
        struct Streamer : Assimp::IOSystem
        {
            bool Exists(const char* pFile) const override
            {
                throw std::logic_error("The method or operation is not implemented.");
            }

            char getOsSeparator() const override
            {
                throw std::logic_error("The method or operation is not implemented.");
            }

            Assimp::IOStream* Open(const char* pFile, const char* pMode) override
            {
                throw std::logic_error("The method or operation is not implemented.");
            }

            void Close(Assimp::IOStream* pFile) override
            {
                throw std::logic_error("The method or operation is not implemented.");
            }
        };
        
        ns_importer->SetIOHandler(new Streamer);
    }
}

namespace jop
{
    Model::Model()
        : m_material    (),
          m_mesh        ()
    {
        setMaterial(Material::getDefault());
        setMesh(Mesh::getDefault());

        createImporter();
    }

    Model::Model(const Mesh& mesh, const Material& material)
        : m_material    (),
          m_mesh        ()
    {
        setMaterial(material);
        setMesh(mesh);

        createImporter();
    }

    Model::Model(const Mesh& mesh)
        : m_material    (),
          m_mesh        ()
    {
        setMaterial(Material::getDefault());
        setMesh(mesh);

        createImporter();
    }

    Model::Model(const Model& other)
        : m_material    (other.m_material),
          m_mesh        (other.m_mesh)
    {
        createImporter();
    }

    Model& Model::operator=(const Model& other)
    {
        m_material = other.m_material;
        m_mesh = other.m_mesh;

        createImporter();

        return *this;
    }

    Model::~Model()
    {
        if (!--ns_referenceCount)
        {
            ns_importer.reset();
            Assimp::DefaultLogger::kill();
        }
    }

    //////////////////////////////////////////////

    bool Model::load(const std::string& filePath)
    {
        

        return m_mesh.get() != &Mesh::getDefault();
    }

    //////////////////////////////////////////////

    const Mesh* Model::getMesh() const
    {
        return m_mesh.get();
    }

    //////////////////////////////////////////////

    Model& Model::setMesh(const Mesh& mesh)
    {
        m_mesh = static_ref_cast<const Mesh>(mesh.getReference());
        return *this;
    }

    //////////////////////////////////////////////

    const Material* Model::getMaterial() const
    {
        return m_material.get();
    }

    //////////////////////////////////////////////

    Model& Model::setMaterial(const Material& material)
    {
        m_material = static_ref_cast<Material>(material.getReference());
        return *this;
    }

    //////////////////////////////////////////////

    unsigned int Model::getVertexAmount() const
    {
        return m_mesh.expired() ? 0 : m_mesh->getVertexAmount();
    }

    //////////////////////////////////////////////

    unsigned int Model::getElementAmount() const
    {
        return m_mesh.expired() ? 0 : m_mesh->getElementAmount();
    }

    //////////////////////////////////////////////

    bool Model::isValid() const
    {
        return !m_mesh.expired() && !m_material.expired();
    }

    //////////////////////////////////////////////

    const Model& Model::getDefault()
    {
        static WeakReference<const Mesh> defMesh;
        static Model model;

        if (defMesh.expired())
        {
            defMesh = static_ref_cast<const Mesh>(Mesh::getDefault().getReference());

            model.setMesh(*defMesh);
        }

        return model;
    }
}