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
    in float out_Shininess;
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
    uniform int u_NumSpotLights;

    #ifdef JMAT_MATERIAL
        vec3 calculateSpotLight(const in int index)
        {
            SpotLightInfo l = u_SpotLights[index];

            // Ambient factor
            #ifdef JMAT_DIFFUSEMAP
                vec3 ambient = l.ambient * out_MatAmbient * vec3(texture2D(u_DiffuseMap, out_TexCoords));
            #else
                vec3 ambient = l.ambient * out_MatAmbient;
            #endif

            // Diffuse factor
            vec3 norm = normalize(out_Normal);
            vec3 lightDir = normalize(light.position - out_Position);
            float diff = max(dot(norm, lightDir), 0.0);
            #ifdef JMAT_DIFFUSEMAP
                vec3 diffuse = light.diffuse * out_MatDiffuse * diff * vec3(texture2D(u_DiffuseMap, out_TexCoords));
            #else
                vec3 diffuse = light.diffuse * diff;
            #endif

            // Specular factor
            vec3 viewDir = normalize(l.position - out_Position);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), out_MatShininess);
            #ifdef JMAT_SPECULARMAP
                vec3 specular = l.specular * spec * vec3(texture2D(u_SpecularMap, out_TexCoords));
            #else
                vec3 specular = l.specular * spec;
            #endif

            // Spot soft
            float theta = dot(lightDir, normalize(-l.direction));
            float epsilon = (light.cutoff.x - light.cutoff.y);
            float intensity = clamp((theta - light.cutoff.y) / epsilon, 0.0, 1.0);
            diffuse *= intensity;
            specular *= intensity;

            // Attenuation
            float distance = length(l.position - out_Position);
            float attenuation = 1.0f / (l.attenuation.x + l.attenuation.y * distance + l.attenuation.z * (distance * distance));
            ambient *= attenuation;
            diffuse *= attenuation;
            specular *= attenuation;

            return ambient + diffuse + specular;
        }
    #else
        vec3 calculateSpotLight(in int index)
        {
            return vec3();
        }
    #endif
#endif

// Ambient constant
#ifdef JMAT_AMBIENT
    uniform vec3 u_AmbientColor;
#endif

// Temporary color from vertex shader
in vec3 tempColor;

// Final fragment color
out vec4 final_Color;

void main() 
{
    #if defined(JMAT_MATERIAL) && defined(JMAT_PHONG)
        // In case both lighting and materials are used, calculate the spot light effect
        for (int i = 0; i < u_NumSpotLights; i++)
            tempColor += calculateSpotLight(i);
    #else
        #ifdef JMAT_DIFFUSEMAP
            tempColor = texture2D(u_DiffuseMap, out_TexCoords);
        #endif
        // Otherwise just use the solid color (material's ambient reflection component)
        tempColor *= vec4(u_SolidColor.xyz, 1.0);
    #endif

    // Add the ambient constant color
    #ifdef JMAT_AMBIENT
        tempColor += vec4(u_AmbientColor.xyz, 1.0);
    #endif

    // Finally assign to the fragment output
    final_Color = tempColor;
}