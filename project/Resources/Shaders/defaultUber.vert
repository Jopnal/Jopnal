// JOPNAL DEFAULT VERTEX UBERSHADER

//////////////////////////////////////////////

// Vertex attributes
layout(location = 0)in vec3 a_Position;
layout(location = 1)in vec2 a_TexCoords;
layout(location = 2)in vec3 a_Normal;

// Matrices
uniform mat4 u_PMatrix; // Perspective
uniform mat4 u_VMatrix; // View
uniform mat4 u_MMatrix; // Model
uniform mat3 u_NMatrix; // Normal

// Ambient constant
#ifdef JMAT_AMBIENT
    uniform vec3 u_AmbientColor;
#endif

// Surface material
#ifdef JMAT_MATERIAL
    struct Material
    {
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float shininess;
    };

    uniform Material u_Material;

    out vec3 out_MatAmbient;
    out vec3 out_MatDiffuse;
    out vec3 out_MatSpecular;
    out float out_Shininess;

// If material is not used, then have a solid color
#else
    uniform vec3 u_SolidColor;
    out vec3 out_SolidColor;
#endif

// Light info
#ifdef JMAT_PHONG
    // Point lights
    struct PointLightInfo
    {
        // Position
        vec3 position;

        // Intensities
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        // Attenuation
        // x = constant
        // y = linear
        // z = quadratic
        vec3 attenuation;
    };
    uniform PointLightInfo u_PointLights[JMAT_MAX_POINT_LIGHTS];
    uniform int u_NumPointLights;

    // Directional lights
    struct DirectionalLightInfo
    {
        // Direction
        vec3 direction;

        // Intensities
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        // No attenuation for directional lights
    };
    uniform DirectionalLightInfo u_DirectionalLights[JMAT_MAX_DIRECTIONAL_LIGHTS];
    uniform int u_NumDirectionalLights;

    // Light calculation functions
    vec3 calculatePoint(const in int index)
    {
        PointLightInfo l = u_PointLights[index];


    }
    vec3 calculateDirectional(const in int index)
    {
        DirectionalLightInfo l = u_DirectionalLights[index];
    }
#endif

// Vertex attributes for fragment shader
out vec3 out_Position;
out vec2 out_TexCoords;
out vec3 out_Normal;

// Temporary color
out vec3 out_TempColor;

void main()
{
    vec3 tempColor;

    #ifdef JMAT_AMBIENT
        tempColor = u_AmbientColor;
    #endif

    #ifdef JMAT_PHONG
    for (int i = 0; i < u_NumPointLights; i++)
        tempColor += calculatePoint(i);
    for (int i = 0; i < u_NumDirectionalLights; i++)
        tempColor += calculateDirectional(i);
    #endif

    // Send vertex attributes
    out_Position = a_Position;
    out_TexCoords = a_TexCoords;
    out_Normal = a_Normal;

    // Send material
    #ifdef JMAT_MATERIAL
        out_MatAmbient = u_Material.ambient;
        out_MatDiffuse = u_Material.diffuse;
        out_MatSpecular = u_Material.specular;
        out_MatShininess = u_Material.shininess;
    #else
        out_SolidColor = u_SolidColor;
    #endif

    // Send temporary color
    out_TempColor = tempColor;

    // Assign position
    gl_Position = u_PMatrix * u_VMatrix * u_MMatrix * vec4(a_Position, 1.0);
}