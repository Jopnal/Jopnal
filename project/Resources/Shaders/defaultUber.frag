// JOPNAL DEFAULT FRAGMENT UBERSHADER

//////////////////////////////////////////////

// Attributes from vertex shader
in vec3 vf_Position;
in vec2 vf_TexCoords;
in vec3 vf_Normal;

// Fragment position from vertex shader
in vec3 vf_FragPosition;

// Diffuse map
#ifdef JMAT_DIFFUSEMAP
    uniform sampler2D u_DiffuseMap;
#endif

// Specular map
#ifdef JMAT_SPECULARMAP
    uniform sampler2D u_SpecularMap;
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
#else
    // If material is not used, then have a solid color
    uniform vec3 u_SolidColor;
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
    uniform uint u_NumPointLights;

    //// Directional lights
    //struct DirectionalLightInfo
    //{
    //    // Direction
    //    vec3 direction;

    //    // Intensities
    //    vec3 ambient;
    //    vec3 diffuse;
    //    vec3 specular;

    //    // No attenuation for directional lights
    //};
    //uniform DirectionalLightInfo u_DirectionalLights[JMAT_MAX_DIRECTIONAL_LIGHTS];
    //uniform uint u_NumDirectionalLights;

    //// Spot lights
    //struct SpotLightInfo
    //{
    //    // Position in eye (camera) coordinates
    //    vec3 position;

    //    // Direction
    //    vec3 direction;

    //    // Intensities
    //    vec3 ambient;
    //    vec3 diffuse;
    //    vec3 specular;

    //    // Attenuation
    //    vec3 attenuation;

    //    // Cutoff
    //    // x = inner
    //    // y = outer
    //    vec2 cutoff;
    //};
    //uniform SpotLightInfo u_SpotLights[JMAT_MAX_SPOT_LIGHTS];
    //uniform uint u_NumSpotLights;

    vec3 calculatePointLight(const in uint index)
    {
        PointLightInfo l = u_PointLights[index];

        // Normal vector
        vec3 norm = normalize(vf_Normal);

        // Direction from light to fragment
        vec3 lightDir = normalize(l.position - vf_FragPosition);

        // Calculate diffuse impact
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * l.diffuse * u_Material.diffuse * vec3(texture(u_DiffuseMap, vf_TexCoords));

        // Specular
        vec3 viewDir = normalize(-vf_FragPosition);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
        vec3 specular = l.specular * spec * u_Material.specular;

        return diffuse + specular;
    }

#endif

// Final fragment color
out vec4 out_FinalColor;

void main() 
{
    // Assign the initial color
    vec3 tempColor =
    #ifdef JMAT_AMBIENT
        JMAT_AMBIENT * vec3(texture(u_DiffuseMap, vf_TexCoords));
    #else
        vec3(0.0, 0.0, 0.0);
    #endif

    // Do lighting calculations
    #if defined(JMAT_PHONG)
        for (uint i = 0u; i < u_NumPointLights; ++i)
            tempColor += calculatePointLight(i);
    #endif

    // Finally assign to the fragment output
    out_FinalColor = vec4(tempColor.xyz, 1.0);
}