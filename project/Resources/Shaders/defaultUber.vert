// JOPNAL DEFAULT VERTEX UBERSHADER

//////////////////////////////////////////////

// Matrices
uniform mat4 u_PMatrix; // Perspective
uniform mat4 u_VMatrix; // View
uniform mat4 u_MMatrix; // Model
uniform mat3 u_NMatrix; // Normal

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
    void calculatePoint(out vec3 

#endif

// Vertex attributes
layout(location = 0)in vec3 a_Position; 
layout(location = 1)in vec2 a_TexCoords;
layout(location = 2)in vec3 a_Normal;

// Texture coordinates to fragment shader
out vec2 out_TexCoords;

void main()
{
#ifdef JMAT_USE_LIGHTS

#endif

    gl_Position = u_PMatrix * u_VMatrix * u_MMatrix * vec4(a_Position, 1.0);

    out_TexCoords = a_TexCoords;
}