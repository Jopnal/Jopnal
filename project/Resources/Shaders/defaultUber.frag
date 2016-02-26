// JOPNAL DEFAULT FRAGMENT UBERSHADER

//////////////////////////////////////////////

// Attributes from vertex shader
in vec3 out_Position;
in vec2 out_TexCoords;
in vec3 out_Normal;

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
    in vec3 out_MatAmbient;
    in vec3 out_MatDiffuse;
    in vec3 out_MatSpecular;
    in float out_MatShininess;
#else
    in vec3 out_SolidColor;
#endif

// Light info
#ifdef JMAT_PHONG
    // Spot lights
    struct SpotLightInfo
    {
        // Position in eye (camera) coordinates
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

    vec3 calculateSpotLight(const in uint index)
    {
        SpotLightInfo l = u_SpotLights[index];

        // Ambient factor
        #ifdef JMAT_DIFFUSEMAP
            vec3 ambient = l.ambient * vec3(texture2D(u_DiffuseMap, out_TexCoords));
        #else
            vec3 ambient = l.ambient;
        #endif
        #ifdef JMAT_MATERIAL
            ambient *= out_MatAmbient;
        #endif

        // Diffuse factor
        vec3 norm = normalize(out_Normal);
        vec3 lightDir = normalize(l.position - out_Position);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse =
        #ifdef JMAT_DIFFUSEMAP
            l.diffuse * diff * vec3(texture2D(u_DiffuseMap, out_TexCoords));
        #else
            l.diffuse * diff;
        #endif
        #ifdef JMAT_MATERIAL
            diffuse *= out_MatDiffuse;
        #endif

        // Specular factor
        vec3 viewDir = normalize(l.position - out_Position);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec =
        #ifdef JMAT_MATERIAL
            pow(max(dot(viewDir, reflectDir), 0.0), out_MatShininess);
        #else
            max(dot(viewDir, reflectDir), 0.0);
        #endif
        vec3 specular =
        #ifdef JMAT_SPECULARMAP
            l.specular * spec * vec3(texture2D(u_SpecularMap, out_TexCoords));
        #else
            l.specular * spec;
        #endif
        #ifdef JMAT_MATERIAL
            specular *= out_MatSpecular;
        #endif

        // Spotlight softness
        float theta = dot(lightDir, normalize(-l.direction));
        float epsilon = (l.cutoff.x - l.cutoff.y);
        float intensity = clamp((theta - l.cutoff.y) / epsilon, 0.0, 1.0);
        diffuse *= intensity;
        specular *= intensity;

        // Attenuation
        float dist = length(l.position - out_Position);
        float attenuation = 1.0f / (l.attenuation.x + l.attenuation.y * dist + l.attenuation.z * (dist * dist));
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        return ambient + diffuse + specular;
    }
#endif

// Temporary color from vertex shader
in vec3 out_TempColor;

// Final fragment color
out vec4 out_FinalColor;

void main() 
{
    vec3 tempColor = out_TempColor;

    #if defined(JMAT_PHONG)
        // In case both lighting and materials are used, calculate the spot light effect
        for (uint i = 0u; i < u_NumSpotLights; i++)
            tempColor += calculateSpotLight(i);
    #elif defined(JMAT_MATERIAL)
        // Otherwise just use a single color (material's ambient reflection component)
        tempColor *= out_MatAmbient;
    #endif

    #ifdef JMAT_DIFFUSEMAP
        tempColor *= texture2D(u_DiffuseMap, out_TexCoords).xyz;
    #endif

    #ifndef JMAT_MATERIAL
        tempColor *= out_SolidColor;
    #endif

    // Finally assign to the fragment output
    out_FinalColor = vec4(tempColor.xyz, 1.0);;
}