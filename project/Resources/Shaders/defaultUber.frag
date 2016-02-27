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

// Emission map
#ifdef JMAT_EMISSIONMAP
    uniform sampler2D u_EmissionMap;
#endif

// Surface material
#ifdef JMAT_MATERIAL
    struct Material
    {
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        vec3 emission;
        float shininess;
    };
    uniform Material u_Material;
#else
    uniform vec3 u_Emission;
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
    uniform uint u_NumDirectionalLights;

    // Spot lights
    struct SpotLightInfo
    {
        // Position
        vec3 position;

        // Direction
        vec3 direction;

        // Intensities
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        // Attenuation
        vec3 attenuation;

        // Cutoff
        // x = inner
        // y = outer
        vec2 cutoff;
    };
    uniform SpotLightInfo u_SpotLights[JMAT_MAX_SPOT_LIGHTS];
    uniform uint u_NumSpotLights;

    // Point light calculation
    vec3 calculatePointLight(const in uint index)
    {
        PointLightInfo l = u_PointLights[index];

        // Ambient impact
        vec3 ambient = u_Material.ambient * l.ambient;

        // Normal vector
        vec3 norm = normalize(vf_Normal);

        // Direction from light to fragment
        vec3 lightDir = normalize(l.position - vf_FragPosition);

        // Diffuse impact
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * l.diffuse * u_Material.diffuse * vec3(texture(u_DiffuseMap, vf_TexCoords));

        // Direction from fragment to eye (eye is always at [0,0,0])
        vec3 viewDir = normalize(-vf_FragPosition);

        // Calculate reflection direction
        vec3 reflectDir = reflect(-lightDir, norm);

        // Specular impact
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
        vec3 specular = l.specular * spec * u_Material.specular * vec3(texture(u_SpecularMap, vf_TexCoords));

        // Attenuation
        float dist = length(l.position - vf_FragPosition);
        float attenuation = 1.0 / (l.attenuation.x + l.attenuation.y * dist + l.attenuation.z * (dist * dist));
        ambient *= attenuation; diffuse *= attenuation; specular *= attenuation;

        // Combine all the colors
        return ambient + diffuse + specular;
    }

    // Directional light calculation
    vec3 calculateDirectionalLight(const in uint index)
    {
        DirectionalLightInfo l = u_DirectionalLights[index];

        // Ambient impact
        vec3 ambient = u_Material.ambient * l.ambient;

        // Normal vector
        vec3 norm = normalize(vf_Normal);

        // Direction from light to fragment.
        // Directional light shines infinitely in the same direction,
        // so no need to take fragment position into account
        vec3 lightDir = normalize(-l.direction);

        // Diffuse impact
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * l.diffuse * u_Material.diffuse * vec3(texture(u_DiffuseMap, vf_TexCoords));

        // Direction from fragment to eye
        vec3 viewDir = normalize(-vf_FragPosition);

        // Calculate reflection direction
        vec3 reflectDir = reflect(-lightDir, norm);

        // Specular impact
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
        vec3 specular = l.specular * spec * u_Material.specular * vec3(texture(u_SpecularMap, vf_TexCoords));

        // No attenuation calculations here
        // Directional Light is infinite, Directional Light is eternal

        return ambient + diffuse + specular;
    }

    // Spot light calculation
    vec3 calculateSpotLight(const in uint index)
    {
        SpotLightInfo l = u_SpotLights[index];

        // Ambient impact
        vec3 ambient = u_Material.ambient * l.ambient;

        // Normal vector
        vec3 norm = normalize(vf_Normal);

        // Direction from light to fragment
        vec3 lightDir = normalize(l.position - vf_FragPosition);

        // Diffuse impact
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = l.diffuse * diff * u_Material.diffuse * vec3(texture(u_DiffuseMap, vf_TexCoords));

        // Specular impact
        vec3 viewDir = normalize(-vf_FragPosition);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
        vec3 specular = l.specular * spec * u_Material.specular * vec3(texture(u_SpecularMap, vf_TexCoords));

        // Spotlight soft edges
        float theta = dot(lightDir, normalize(-l.direction));
        float epsilon = (l.cutoff.x - l.cutoff.y);
        float intensity = clamp((theta - l.cutoff.y) / epsilon, 0.0, 1.0);
        ambient *= intensity;
        diffuse *= intensity;
        specular *= intensity;

        // Attenuation
        float dist = length(l.position - vf_FragPosition);
        float attenuation = 1.0 / (l.attenuation.x + l.attenuation.y * dist + l.attenuation.z * (dist * dist));
        ambient *= attenuation; diffuse *= attenuation; specular *= attenuation;

        return ambient + diffuse + specular;
    }

#endif

// Final fragment color
out vec4 out_FinalColor;

void main() 
{
    // Assign the initial color
    vec3 tempColor =
    #ifdef JMAT_AMBIENT
        #ifdef JMAT_DIFFUSEMAP
            JMAT_AMBIENT * vec3(texture(u_DiffuseMap, vf_TexCoords));
        #else
            JMAT_AMBIENT;
        #endif
    #else
        vec3(0.0, 0.0, 0.0);
    #endif

    // Do lighting calculations
    #if defined(JMAT_PHONG)
        // Point lights
        for (uint i = 0u; i < u_NumPointLights; ++i)
            tempColor += calculatePointLight(i);

        // Directional lights
        for (uint i = 0u; i < u_NumDirectionalLights; ++i)
            tempColor += calculateDirectionalLight(i);

        // Spot lights
        for (uint i = 0u; i < u_NumSpotLights; ++i)
            tempColor += calculateSpotLight(i);
    #endif

    // Emission
    #ifdef JMAT_EMISSIONMAP
        tempColor += u_Material.emission * vec3(texture(u_EmissionMap, vf_TexCoords));
    #else
        #ifdef JMAT_MATERIAL
            tempColor += u_Material.emission;
        #else
            tempColor += u_Emission;
        #endif
    #endif

    // Finally assign to the fragment output
    out_FinalColor = vec4(tempColor.xyz, 1.0);
}