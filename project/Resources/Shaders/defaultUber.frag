// JOPNAL DEFAULT FRAGMENT UBERSHADER
//
// Jopnal license applies
//
// Lot of techniques used courtesy of http://learnopengl.com/

//////////////////////////////////////////////


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

// Environment map
#ifdef JMAT_ENVIRONMENTMAP
    uniform samplerCube u_EnvironmentMap;
#endif

// Reflection map
#ifdef JMAT_REFLECTIONMAP
    uniform sampler2D u_ReflectionMap;
#endif

// Opacity map
#ifdef JMAT_OPACITYMAP
    uniform sampler2D u_OpacityMap;
    float specularComponent = 0.0;
#endif

// Gloss map
#ifdef JMAT_GLOSSMAP
    uniform sampler2D u_GlossMap;
#endif

#ifdef JMAT_PHONG

    // Does the object receive lights?
    uniform bool u_ReceiveLights;

    // Does the object receive shadows?
    uniform bool u_ReceiveShadows;

    // Fragment position from vertex/geometry shader
    in vec3 vgf_FragPosition;

#endif

// Camera position
#if defined(JMAT_PHONG) || defined(JMAT_ENVIRONMENTMAP)
    uniform vec3 u_CameraPosition;
#endif

// Vertex attribute data
in FragVertexData
{
    vec3 Position;
    vec2 TexCoords;
    vec3 Normal;
    //vec3 Tangent;
    //vec3 BiTangent;
    vec4 Color;

} outVert;

// Surface material
#ifdef JMAT_MATERIAL
    struct Material
    {
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        vec3 emission;
        float shininess;

        #ifdef JMAT_ENVIRONMENTMAP
            float reflectivity;
        #endif
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

        // Shadow map info
        bool castShadow;    ///< Cast shadows?
        float farPlane;     ///< The light's far plane
    };
    uniform PointLightInfo u_PointLights[JMAT_MAX_POINT_LIGHTS];
    uniform samplerCube u_PointLightShadowMaps[JMAT_MAX_POINT_LIGHTS];
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

        // Shadow map info
        bool castShadow;        ///< Cast shadows?
        mat4 lsMatrix;          ///< Light space matrix
    };
    uniform DirectionalLightInfo u_DirectionalLights[JMAT_MAX_DIRECTIONAL_LIGHTS];
    uniform sampler2D u_DirectionalLightShadowMaps[JMAT_MAX_DIRECTIONAL_LIGHTS];
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

        // Shadow map info
        bool castShadow;        ///< Cast shadows?
        mat4 lsMatrix;          ///< Light space matrix
    };
    uniform SpotLightInfo u_SpotLights[JMAT_MAX_SPOT_LIGHTS];
    uniform sampler2D u_SpotLightShadowMaps[JMAT_MAX_SPOT_LIGHTS];
    uniform uint u_NumSpotLights;

    // Offset directions for sampling point shadows
    const vec3 g_gridSamplingDisk[20] = vec3[]
    (
        vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1),
        vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
        vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
        vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
        vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
    );

    // Point light calculation
    vec3 calculatePointLight(const in uint index)
    {
        PointLightInfo l = u_PointLights[index];

        // Ambient impact
        vec3 ambient = l.ambient
        #ifdef JMAT_MATERIAL
            * u_Material.ambient
        #endif
        #ifdef JMAT_DIFFUSEMAP
            * vec3(texture(u_DiffuseMap, outVert.TexCoords))
        #endif
        ;

        // Normal vector
        vec3 norm = normalize(outVert.Normal);

        // Direction from fragment to light
        vec3 lightDir = normalize(l.position - vgf_FragPosition);

        // Diffuse impact
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * l.diffuse
        #ifdef JMAT_MATERIAL
            * u_Material.diffuse
        #endif
        #ifdef JMAT_DIFFUSEMAP
            * vec3(texture(u_DiffuseMap, outVert.TexCoords))
        #endif
        ;

        // Direction from fragment to eye
        vec3 viewDir = normalize(u_CameraPosition - vgf_FragPosition);

        // Calculate reflection direction (use a half-way vector)
        vec3 reflectDir = normalize(lightDir + viewDir);

        float shininess = max(1.0, u_Material.shininess

        #ifdef JMAT_GLOSSMAP
            * texture(u_GlossMap, outVert.TexCoords).r
        #endif
        );

        // Specular impact
        float spec =
        #ifdef JMAT_MATERIAL
            (8.0 + shininess) / (8.0 * 3.14159265) /*<< energy conservation */ * pow(max(dot(norm, reflectDir), 0.0), shininess)
        #else
            0.0
        #endif
        ;

        #ifdef JMAT_OPACITYMAP
            specularComponent += spec;
        #endif

        vec3 specular = l.specular * spec
        #ifdef JMAT_MATERIAL
            * u_Material.specular
        #endif
        #ifdef JMAT_SPECULARMAP
            * vec3(texture(u_SpecularMap, outVert.TexCoords))
        #endif
        ;

        // Attenuation
        float dist = length(l.position - vgf_FragPosition);
        float attenuation = 1.0 / (l.attenuation.x + l.attenuation.y * dist + l.attenuation.z * (dist * dist));
        ambient *= attenuation; diffuse *= attenuation; specular *= attenuation;

        // Shadow calculation
        if (l.castShadow && u_ReceiveShadows)
        {
            // Get a vector between fragment and light positions
            vec3 fragToLight = vgf_FragPosition - l.position;

            // Get current linear depth as the length between the fragment and light position
            float currentDepth = length(fragToLight);

            // Test for shadows with PCF
            float shadow = 0.0;
            const float bias = 0.15;
            const int samples = 20;

            float viewDistance = length(u_CameraPosition - vgf_FragPosition);
            float diskRadius = (1.0 + (viewDistance / l.farPlane)) / 25.0;
            for (int i = 0; i < samples; ++i)
            {
                vec3 samp = fragToLight + g_gridSamplingDisk[i] * diskRadius;
                
                float closestDepth = texture(u_PointLightShadowMaps[index], samp).r;

                // Undo mapping [0,1]
                closestDepth *= l.farPlane;

                if (currentDepth - bias > closestDepth)
                    shadow += 1.0;
            }
            shadow /= float(samples);

            return (ambient + (1.0 - shadow) * (diffuse + specular));
        }

        return ambient + diffuse + specular;
    }

    // Shadow calculation for directional and spot lights
    float calculateDirSpotShadow(const in vec3 projCoords, const in vec3 norm, const in vec3 lightDir, const in sampler2D samp)
    {
        // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
        float closestDepth = texture(samp, projCoords.xy).r; 

        // Get depth of current fragment from light's perspective
        float currentDepth = projCoords.z;

        // Check whether current frag pos is in shadow
        float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);

        float shadow = 0.0;

        if (projCoords.z > 1.0)
            shadow = 0.0;

        // Do percentage-closer filtering
        else
        {
            vec2 texelSize = 1.0 / textureSize(samp, 0);
            for(int x = -1; x <= 1; ++x)
            {
                for(int y = -1; y <= 1; ++y)
                {
                    float pcfDepth = texture(samp, projCoords.xy + vec2(x, y) * texelSize).r; 
                    shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
                }    
            }
            shadow /= 9.0;
        }

        return shadow;
    }

    // Directional light calculation
    vec3 calculateDirectionalLight(const in uint index)
    {
        DirectionalLightInfo l = u_DirectionalLights[index];

        // Ambient impact
        vec3 ambient = l.ambient
        #ifdef JMAT_MATERIAL
            * u_Material.ambient
        #endif
        #ifdef JMAT_DIFFUSEMAP
            * vec3(texture(u_DiffuseMap, outVert.TexCoords))
        #endif
        ;

        // Normal vector
        vec3 norm = normalize(outVert.Normal);

        // Direction from light to fragment.
        // Directional light shines infinitely in the same direction,
        // so no need to take fragment position into account
        vec3 lightDir = normalize(-l.direction);

        // Diffuse impact
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = l.diffuse * diff
        #ifdef JMAT_MATERIAL
            * u_Material.diffuse
        #endif
        #ifdef JMAT_DIFFUSEMAP
            * vec3(texture(u_DiffuseMap, outVert.TexCoords))
        #endif
        ;

        // Direction from fragment to eye
        vec3 viewDir = normalize(u_CameraPosition - vgf_FragPosition);

        // Calculate reflection direction (use a half-way vector)
        vec3 reflectDir = normalize(lightDir + viewDir);

        float shininess = max(1.0, u_Material.shininess

        #ifdef JMAT_GLOSSMAP
            * texture(u_GlossMap, outVert.TexCoords).r
        #endif
        );

        // Specular impact
        float spec =
        #ifdef JMAT_MATERIAL
            (8.0 + shininess) / (8.0 * 3.14159265) /*<< energy conservation */ * pow(max(dot(norm, reflectDir), 0.0), shininess)
        #else
            0.0
        #endif
        ;

        #ifdef JMAT_OPACITYMAP
            specularComponent += spec;
        #endif

        vec3 specular = l.specular * spec
        #ifdef JMAT_MATERIAL
            * u_Material.specular
        #endif
        #ifdef JMAT_SPECULARMAP
            * vec3(texture(u_SpecularMap, outVert.TexCoords))
        #endif
        ;

        // No attenuation calculations here
        // Directional Light is infinite, Directional Light is eternal

        // Shadow calculation
        if (l.castShadow && u_ReceiveShadows)
            return (ambient + (1.0 - calculateDirSpotShadow(vec3(l.lsMatrix * vec4(vgf_FragPosition, 1.0)) * 0.5 + 0.5, norm, lightDir, u_DirectionalLightShadowMaps[index])) * (diffuse + specular));
        
        return ambient + diffuse + specular;
    }

    // Spot light calculation
    vec3 calculateSpotLight(const in uint index)
    {
        SpotLightInfo l = u_SpotLights[index];

        // Ambient impact
        vec3 ambient = l.ambient
        #ifdef JMAT_MATERIAL
            * u_Material.ambient
        #endif
        #ifdef JMAT_DIFFUSEMAP
            * vec3(texture(u_DiffuseMap, outVert.TexCoords))
        #endif
        ;

        // Normal vector
        vec3 norm = normalize(outVert.Normal);

        // Direction from fragment to light
        vec3 lightDir = normalize(l.position - vgf_FragPosition);

        // Diffuse impact
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = l.diffuse * diff
        #ifdef JMAT_MATERIAL
            * u_Material.diffuse
        #endif
        #ifdef JMAT_DIFFUSEMAP
            * vec3(texture(u_DiffuseMap, outVert.TexCoords))
        #endif
        ;

        // Direction from fragment to eye
        vec3 viewDir = normalize(u_CameraPosition - vgf_FragPosition);

        // Calculate reflection direction (use a half-way vector)
        vec3 reflectDir = normalize(lightDir + viewDir);

        float shininess = max(1.0, u_Material.shininess

        #ifdef JMAT_GLOSSMAP
            * texture(u_GlossMap, outVert.TexCoords).r
        #endif
        );

        // Specular impact
        float spec =
        #ifdef JMAT_MATERIAL
            (8.0 + shininess) / (8.0 * 3.14159265) /*<< energy conservation */ * pow(max(dot(norm, reflectDir), 0.0), shininess)
        #else
            0.0
        #endif
        ;

        #ifdef JMAT_OPACITYMAP
            specularComponent += spec;
        #endif

        vec3 specular = l.specular * spec
        #ifdef JMAT_MATERIAL
            * u_Material.specular
        #endif
        #ifdef JMAT_SPECULARMAP
            * vec3(texture(u_SpecularMap, outVert.TexCoords))
        #endif
        ;

        // Spotlight soft edges
        float theta = dot(lightDir, normalize(-l.direction));
        float epsilon = (l.cutoff.x - l.cutoff.y);
        float intensity = clamp((theta - l.cutoff.y) / epsilon, 0.0, 1.0);
        ambient *= intensity;
        diffuse *= intensity;
        specular *= intensity;

        // Attenuation
        float dist = length(l.position - vgf_FragPosition);
        float attenuation = 1.0 / (l.attenuation.x + l.attenuation.y * dist + l.attenuation.z * (dist * dist));
        ambient *= attenuation; diffuse *= attenuation; specular *= attenuation;

        // Shadow calculation
        if (l.castShadow && u_ReceiveShadows)
        {
            vec4 tempCoords = l.lsMatrix * vec4(vgf_FragPosition, 1.0);

            return (ambient + (1.0 - calculateDirSpotShadow((tempCoords.xyz / tempCoords.w) * 0.5 + 0.5, norm, lightDir, u_SpotLightShadowMaps[index])) * (diffuse + specular));
        }

        return ambient + diffuse + specular;
    }

#endif

// Final fragment color
out vec4 out_FinalColor;

void main() 
{
#ifdef JMAT_SKYBOX

    out_FinalColor = texture(u_EnvironmentMap, outVert.Position);

#else

    // Assign the initial color
    vec3 tempColor =

    #if !defined(JMAT_PHONG) && defined(JMAT_DIFFUSEMAP)
        vec3(texture(u_DiffuseMap, outVert.TexCoords))

        #ifdef JMAT_VERTEXCOLOR
            * vec3(outVert.Color)
        #endif
    #else
        #if defined(JMAT_VERTEXCOLOR)
            vec3(outVert.Color)
        #else
            vec3(0.0, 0.0, 0.0)
        #endif
    #endif
    ;

    #ifdef JMAT_ENVIRONMENTMAP

        vec3 I = normalize(outVert.Position - u_CameraPosition);
        vec3 R = reflect(I, normalize(outVert.Normal));

        vec3 refl = vec3(0.0, 0.0, 0.0);

        #ifdef JMAT_REFLECTIONMAP
            float reflIntensity = texture(u_ReflectionMap, outVert.TexCoords).r;
            if (reflIntensity > 0.1)
                refl = vec3(texture(u_EnvironmentMap, R)) * reflIntensity
        #else
            refl = vec3(texture(u_EnvironmentMap, R))
        #endif
        #ifdef JMAT_MATERIAL
            * u_Material.reflectivity
        #endif
        ;

        tempColor += refl;

    #endif

    // Do lighting calculations
    #ifdef JMAT_PHONG

        if (u_ReceiveLights)
        {
            // Point lights
            for (uint i = 0u; i < u_NumPointLights; ++i)
                tempColor += calculatePointLight(i);

            // Directional lights
            for (uint i = 0u; i < u_NumDirectionalLights; ++i)
                tempColor += calculateDirectionalLight(i);

            // Spot lights
            for (uint i = 0u; i < u_NumSpotLights; ++i)
                tempColor += calculateSpotLight(i);
        }

    #endif

    // Emission
    #ifdef JMAT_EMISSIONMAP
        #ifdef JMAT_MATERIAL
            tempColor += u_Material.emission * vec3(texture(u_EmissionMap, outVert.TexCoords));
        #else
            tempColor += vec3(texture(u_EmissionMap, outVert.TexCoords));
        #endif
    #else
        #ifdef JMAT_MATERIAL
            tempColor += u_Material.emission;
        #else
            tempColor += u_Emission;
        #endif
    #endif
    
    float alpha = 1.0;

    #if defined(JMAT_OPACITYMAP)
        alpha = texture(u_OpacityMap, outVert.TexCoords).r + specularComponent;
    #elif defined(JMAT_DIFFUSEALPHA)
        alpha *= texture(u_DiffuseMap, outVert.TexCoords).a;
    #endif

    #ifdef JMAT_VERTEXCOLOR
        alpha *= outVert.Color.a;
    #endif

    #if defined(JMAT_OPACITYMAP) || defined(JMAT_DIFFUSEALPHA)
        if (alpha < 0.1)
            discard;
    #endif

    // Finally assign to the fragment output
    out_FinalColor = vec4(pow(tempColor, vec3(1.0 /
        
    #ifdef JMAT_ENVIRONMENT_RECORD
        // Temporary "fix" for gamma correction getting
        // applied twice. This is a somewhat working
        // approximation, though the colors are
        // still a bit wrong
        2.05
    #else
        2.2
    #endif
        
    )), alpha);

#endif // Sky box
}