// JOPNAL DEFAULT FRAGMENT UBERSHADER
//
// Jopnal license applies

//////////////////////////////////////////////


// Diffuse map
#ifdef JMAT_DIFFUSEMAP
    uniform sampler2D u_DiffuseMap;
    #define JMAT_USE_TEXTURE
#endif

// Specular map
#ifdef JMAT_SPECULARMAP
    uniform sampler2D u_SpecularMap;
    #define JMAT_USE_TEXTURE
#endif

// Emission map
#ifdef JMAT_EMISSIONMAP
    uniform sampler2D u_EmissionMap;
    #define JMAT_USE_TEXTURE
#endif

// Attributes from vertex shader
#if defined(JMAT_USE_TEXTURE)
    in vec2 vf_TexCoords;
#endif
#ifdef JMAT_PHONG

    // Camera position
    uniform vec3 u_CameraPosition;

    // Does the object receive lights?
    uniform bool u_ReceiveLights;

    // Does the object receive shadows?
    uniform bool u_ReceiveShadows;

    // Normal vector
    in vec3 vf_Normal;

    // Fragment position from vertex shader
    in vec3 vf_FragPosition;

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

        // Shadow map info
        bool castShadow;        ///< Cast shadows?
        mat4 lsMatrix[6];       ///< Light space matrices
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

    // Point light calculation
    vec3 calculatePointLight(const in uint index)
    {
        PointLightInfo l = u_PointLights[index];

        // Ambient impact
        vec3 ambient = l.ambient
        #ifdef JMAT_MATERIAL
            * u_Material.ambient
        #endif
        ;

        // Normal vector
        vec3 norm = normalize(vf_Normal);

        // Direction from fragment to light
        vec3 lightDir = normalize(l.position - vf_FragPosition);

        // Diffuse impact
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * l.diffuse
        #ifdef JMAT_MATERIAL
            * u_Material.diffuse
        #endif
        #ifdef JMAT_DIFFUSEMAP
            * vec3(texture(u_DiffuseMap, vf_TexCoords))
        #endif
        ;

        // Direction from fragment to eye
        vec3 viewDir = normalize(u_CameraPosition - vf_FragPosition);

        // Calculate reflection direction (use a half-way vector)
        vec3 reflectDir = normalize(lightDir + viewDir);

        // Specular impact
        float spec =
        #ifdef JMAT_MATERIAL
            (8.0 + u_Material.shininess) / (8.0 * 3.14159265) /*<< energy conservation */ * pow(max(dot(norm, reflectDir), 0.0), u_Material.shininess)
        #else
            max(dot(norm, reflectDir), 0.0)
        #endif
        ;
        vec3 specular = l.specular * spec
        #ifdef JMAT_MATERIAL
            * u_Material.specular
        #endif
        #ifdef JMAT_SPECULARMAP
            * vec3(texture(u_SpecularMap, vf_TexCoords))
        #endif
        ;

        // Attenuation
        float dist = length(l.position - vf_FragPosition);
        float attenuation = 1.0 / (l.attenuation.x + l.attenuation.y * dist + l.attenuation.z * (dist * dist));
        ambient *= attenuation; diffuse *= attenuation; specular *= attenuation;

        // Shadow calculation
        #ifdef JMAT_PHONG
            if (l.castShadow && u_ReceiveShadows)
            {

            }
        #endif

        return ambient + diffuse + specular;
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
        ;

        // Normal vector
        vec3 norm = normalize(vf_Normal);

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
            * vec3(texture(u_DiffuseMap, vf_TexCoords))
        #endif
        ;

        // Direction from fragment to eye
        vec3 viewDir = normalize(u_CameraPosition - vf_FragPosition);

        // Calculate reflection direction (use a half-way vector)
        vec3 reflectDir = normalize(lightDir + viewDir);

        // Specular impact
        float spec =
        #ifdef JMAT_MATERIAL
            (8.0 + u_Material.shininess) / (8.0 * 3.14159265) /*<< energy conservation */ * pow(max(dot(norm, reflectDir), 0.0), u_Material.shininess)
        #else
            max(dot(norm, reflectDir), 0.0)
        #endif
        ;
        vec3 specular = l.specular * spec
        #ifdef JMAT_MATERIAL
            * u_Material.specular
        #endif
        #ifdef JMAT_SPECULARMAP
            * vec3(texture(u_SpecularMap, vf_TexCoords))
        #endif
        ;

        // No attenuation calculations here
        // Directional Light is infinite, Directional Light is eternal

        // Shadow calculation
        #ifdef JMAT_PHONG
            if (l.castShadow && u_ReceiveShadows)
            {
                vec3 projCoords = vec3(l.lsMatrix * vec4(vf_FragPosition, 1.0));

                // Transform to [0,1] range
                projCoords = projCoords * 0.5 + 0.5;

                // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
                float closestDepth = texture(u_DirectionalLightShadowMaps[index], projCoords.xy).r; 

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
                    vec2 texelSize = 1.0 / textureSize(u_DirectionalLightShadowMaps[index], 0);
                    for(int x = -1; x <= 1; ++x)
                    {
                        for(int y = -1; y <= 1; ++y)
                        {
                            float pcfDepth = texture(u_DirectionalLightShadowMaps[index], projCoords.xy + vec2(x, y) * texelSize).r; 
                            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
                        }    
                    }
                    shadow /= 9.0;
                }

                return (ambient + (1.0 - shadow) * (diffuse + specular));
            }
        #endif
        
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
        ;

        // Normal vector
        vec3 norm = normalize(vf_Normal);

        // Direction from fragment to light
        vec3 lightDir = normalize(l.position - vf_FragPosition);

        // Diffuse impact
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = l.diffuse * diff
        #ifdef JMAT_MATERIAL
            * u_Material.diffuse
        #endif
        #ifdef JMAT_DIFFUSEMAP
            * vec3(texture(u_DiffuseMap, vf_TexCoords))
        #endif
        ;

        // Direction from fragment to eye
        vec3 viewDir = normalize(u_CameraPosition - vf_FragPosition);

        // Calculate reflection direction (use a half-way vector)
        vec3 reflectDir = normalize(lightDir + viewDir);

        // Specular impact
        float spec =
        #ifdef JMAT_MATERIAL
            (8.0 + u_Material.shininess) / (8.0 * 3.14159265) /*<< energy conservation */ * pow(max(dot(norm, reflectDir), 0.0), u_Material.shininess)
        #else
            max(dot(norm, reflectDir), 0.0)
        #endif
        ;
        vec3 specular = l.specular * spec
        #ifdef JMAT_MATERIAL
            * u_Material.specular
        #endif
        #ifdef JMAT_SPECULARMAP
            * vec3(texture(u_SpecularMap, vf_TexCoords))
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
        float dist = length(l.position - vf_FragPosition);
        float attenuation = 1.0 / (l.attenuation.x + l.attenuation.y * dist + l.attenuation.z * (dist * dist));
        ambient *= attenuation; diffuse *= attenuation; specular *= attenuation;

        // Shadow calculation
        #ifdef JMAT_PHONG
            if (l.castShadow && u_ReceiveShadows)
            {
                vec4 coords = l.lsMatrix * vec4(vf_FragPosition, 1.0);

                // Perspective divide
                vec3 projCoords = coords.xyz / coords.w;

                // Transform to [0,1] range
                projCoords = projCoords * 0.5 + 0.5;

                // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
                float closestDepth = texture(u_SpotLightShadowMaps[index], projCoords.xy).r;

                // Get depth of current fragment from light's perspective
                float currentDepth = projCoords.z;

                // Check whether current frag pos is in shadow
                float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);
                float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

                if (projCoords.z > 1.0)
                    shadow = 0.0;

                return (ambient + (1.0 - shadow) * (diffuse + specular));
            }
        #endif

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
            tempColor += u_Material.emission * vec3(texture(u_EmissionMap, vf_TexCoords));
        #else
            tempColor += vec3(texture(u_EmissionMap, vf_TexCoords));
        #endif
    #else
        #ifdef JMAT_MATERIAL
            tempColor += u_Material.emission;
        #else
            tempColor += u_Emission;
        #endif
    #endif

    // Finally assign to the fragment output
    out_FinalColor = vec4(tempColor, 1.0);
}