// JOPNAL DEFAULT FRAGMENT UBERSHADER
//
// Jopnal license applies

//////////////////////////////////////////////

#include <Jopnal/Compat/FragmentColor>
#include <Jopnal/Compat/Varyings>
#include <Jopnal/Compat/Samplers>

// Diffuse map
uniform sampler2D u_DiffuseMap;

// Specular map
uniform sampler2D u_SpecularMap;

// Emission map
uniform sampler2D u_EmissionMap;

// Environment map
uniform samplerCube u_EnvironmentMap;

// Reflection map
uniform sampler2D u_ReflectionMap;

// Opacity map
uniform sampler2D u_OpacityMap;

// Gloss map
uniform sampler2D u_GlossMap;

// Does the object receive lights?
uniform bool u_ReceiveLights;

// Does the object receive shadows?
uniform bool u_ReceiveShadows;

// Fragment position from vertex/geometry shader
JOP_VARYING_IN vec3 vf_FragPosition;

// Camera position
uniform vec3 u_CameraPosition;

// Vertex attribute data
JOP_VARYING_IN vec3 vf_Position;
JOP_VARYING_IN vec2 vf_TexCoords;
JOP_VARYING_IN vec3 vf_Normal;
JOP_VARYING_IN vec4 vf_Color;

// Surface material
struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 emission;

    float shininess;
    float reflectivity;
};
uniform Material u_Material;

// Light info
#ifdef JMAT_LIGHTING

    #if JMAT_MAX_POINT_LIGHTS > 0

        // Point lights
        struct PointLightInfo
        {
            bool enabled;

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
        uniform int u_NumPointLights;

    #endif

    #if JMAT_MAX_DIRECTIONAL_LIGHTS > 0

        // Directional lights
        struct DirectionalLightInfo
        {
            bool enabled;

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
        uniform int u_NumDirectionalLights;

    #endif

    #if JMAT_MAX_SPOT_LIGHTS > 0

        // Spot lights
        struct SpotLightInfo
        {
            bool enabled;

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
        uniform int u_NumSpotLights;

    #endif

    #if JMAT_MAX_POINT_LIGHTS > 0

        #if __VERSION__ >= 300
        
            // Offset directions for sampling point shadows
            const vec3 g_gridSamplingDisk[20] = vec3[]
            (
                vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1),
                vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
                vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
                vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
                vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
            );

        #endif
        
        // Point light calculation
        vec3 calculatePointLight(const in int index)
        {
            PointLightInfo l = u_PointLights[index];

            #if __VERSION__ < 300
                if (!l.enabled)
                    return vec3(0.0);
            #endif

            // Ambient impact
            vec3 ambient = l.ambient * vec3(u_Material.ambient);

            // Normal vector
            vec3 norm = normalize(vf_Normal);

            // Direction from fragment to light
            vec3 lightDir = normalize(l.position - vf_FragPosition);

            // Diffuse impact
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * l.diffuse * vec3(u_Material.diffuse);

            // Direction from fragment to eye
            vec3 viewDir = normalize(u_CameraPosition - vf_FragPosition);

            // Calculate reflection direction (use a half-way vector)
            vec3 reflectDir = normalize(lightDir + viewDir);

            float shininess = max(1.0, u_Material.shininess

            #ifdef JMAT_GLOSSMAP
                * JOP_TEXTURE_2D(u_GlossMap, vf_TexCoords).a
            #endif
            );

            // Specular impact
            float spec = (8.0 + shininess) / (8.0 * 3.14159265) /*<< energy conservation */ * pow(max(dot(norm, reflectDir), 0.0), shininess);

            vec3 specular = l.specular * spec * vec3(u_Material.specular)

            #ifdef JMAT_SPECULARMAP
                * vec3(JOP_TEXTURE_2D(u_SpecularMap, vf_TexCoords))
            #endif
            ;

            // Attenuation
            float dist = length(l.position - vf_FragPosition);
            float attenuation = 1.0 / (l.attenuation.x + l.attenuation.y * dist + l.attenuation.z * (dist * dist));
            ambient *= attenuation; diffuse *= attenuation; specular *= attenuation;

            // Shadow calculation
            if (l.castShadow && u_ReceiveShadows)
            {
                // Get a vector between fragment and light positions
                vec3 fragToLight = vf_FragPosition - l.position;

                // Test for shadows with PCF
            #if __VERSION__ >= 300

                // Get current linear depth as the length between the fragment and light position
                float currentDepth = length(fragToLight);

                float shadow = 0.0;
                const float bias = 0.15;
                const int samples = 20;

                float viewDistance = length(u_CameraPosition - vf_FragPosition);
                float diskRadius = (1.0 + (viewDistance / l.farPlane)) / 25.0;
                for (int i = 0; i < samples; ++i)
                {
                    vec3 samp = fragToLight + g_gridSamplingDisk[i] * diskRadius;
                    
                    float closestDepth = JOP_TEXTURE_CUBE(u_PointLightShadowMaps[index], samp).r;

                    // Undo mapping [0,1]
                    closestDepth *= l.farPlane;

                    if (currentDepth - bias > closestDepth)
                        shadow += 1.0;
                }
                shadow /= float(samples);

            #else

                float closestDepth = JOP_TEXTURE_CUBE(u_PointLightShadowMaps[index], fragToLight).r;

                closestDepth *= l.farPlane;

                float currentDepth = length(fragToLight);

                const float bias = 0.05;

                float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

            #endif
            
                return (ambient + (1.0 - shadow) * (diffuse + specular));
            }

            return ambient + diffuse + specular;
        }

    #endif

    #if JMAT_MAX_DIRECTIONAL_LIGHTS > 0 || JMAT_MAX_SPOT_LIGHTS > 0

        // Shadow calculation for directional and spot lights
        float calculateDirSpotShadow(const in vec3 projCoords, const in vec3 norm, const in vec3 lightDir, const in sampler2D samp)
        {
            // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
            float closestDepth = JOP_TEXTURE_2D(samp, projCoords.xy).r;

            // Get depth of current fragment from light's perspective
            float currentDepth = projCoords.z;

            // Check whether current frag pos is in shadow
            float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);

            float shadow = 0.0;

            if (projCoords.z > 1.0)
                shadow = 0.0;

            // Do percentage-closer filtering
        #if __VERSION__ >= 300

            else
            {
                vec2 texelSize = vec2(1.0) / vec2(textureSize(samp, 0));
                for(int x = -1; x <= 1; ++x)
                {
                    for(int y = -1; y <= 1; ++y)
                    {
                        float pcfDepth = JOP_TEXTURE_2D(samp, projCoords.xy + vec2(x, y) * texelSize).r;
                        shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
                    }    
                }
                shadow /= 9.0;
            }

        #endif

            return shadow;
        }

    #endif

    #if JMAT_MAX_DIRECTIONAL_LIGHTS > 0

        // Directional light calculation
        vec3 calculateDirectionalLight(const in int index)
        {
            DirectionalLightInfo l = u_DirectionalLights[index];

            #if __VERSION__ < 300
                if (!l.enabled)
                    return vec3(0.0);
            #endif

            // Ambient impact
            vec3 ambient = l.ambient * vec3(u_Material.ambient);

            // Normal vector
            vec3 norm = normalize(vf_Normal);

            // Direction from light to fragment.
            // Directional light shines infinitely in the same direction,
            // so no need to take fragment position into account
            vec3 lightDir = normalize(-l.direction);

            // Diffuse impact
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = l.diffuse * diff * vec3(u_Material.diffuse);

            // Direction from fragment to eye
            vec3 viewDir = normalize(u_CameraPosition - vf_FragPosition);

            // Calculate reflection direction (use a half-way vector)
            vec3 reflectDir = normalize(lightDir + viewDir);

            float shininess = max(1.0, u_Material.shininess

            #ifdef JMAT_GLOSSMAP
                * JOP_TEXTURE_2D(u_GlossMap, vf_TexCoords).a
            #endif
            );

            // Specular impact
            float spec = (8.0 + shininess) / (8.0 * 3.14159265) /*<< energy conservation */ * pow(max(dot(norm, reflectDir), 0.0), shininess);

            vec3 specular = l.specular * spec * vec3(u_Material.specular)

            #ifdef JMAT_SPECULARMAP
                * vec3(JOP_TEXTURE_2D(u_SpecularMap, vf_TexCoords))
            #endif
            ;

            // No attenuation calculations here
            // Directional Light is infinite, Directional Light is eternal

            // Shadow calculation
            if (l.castShadow && u_ReceiveShadows)
                return (ambient + (1.0 - calculateDirSpotShadow(vec3(l.lsMatrix * vec4(vf_FragPosition, 1.0)) * 0.5 + 0.5, norm, lightDir, u_DirectionalLightShadowMaps[index])) * (diffuse + specular));
            
            return ambient + diffuse + specular;
        }

    #endif

    #if JMAT_MAX_SPOT_LIGHTS > 0

        // Spot light calculation
        vec3 calculateSpotLight(const in int index)
        {
            SpotLightInfo l = u_SpotLights[index];

            #if __VERSION__ < 300
                if (!l.enabled)
                    return vec3(0.0);
            #endif

            // Ambient impact
            vec3 ambient = l.ambient * vec3(u_Material.ambient);

            // Normal vector
            vec3 norm = normalize(vf_Normal);

            // Direction from fragment to light
            vec3 lightDir = normalize(l.position - vf_FragPosition);

            // Diffuse impact
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = l.diffuse * diff * vec3(u_Material.diffuse);

            // Direction from fragment to eye
            vec3 viewDir = normalize(u_CameraPosition - vf_FragPosition);

            // Calculate reflection direction (use a half-way vector)
            vec3 reflectDir = normalize(lightDir + viewDir);

            float shininess = max(1.0, u_Material.shininess

            #ifdef JMAT_GLOSSMAP
                * JOP_TEXTURE_2D(u_GlossMap, vf_TexCoords).a
            #endif
            );

            // Specular impact
            float spec = (8.0 + shininess) / (8.0 * 3.14159265) /*<< energy conservation */ * pow(max(dot(norm, reflectDir), 0.0), shininess);

            vec3 specular = l.specular * spec * vec3(u_Material.specular)

            #ifdef JMAT_SPECULARMAP
                * vec3(JOP_TEXTURE_2D(u_SpecularMap, vf_TexCoords))
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
            if (l.castShadow && u_ReceiveShadows)
            {
                vec4 tempCoords = l.lsMatrix * vec4(vf_FragPosition, 1.0);

                return (ambient + (1.0 - calculateDirSpotShadow((tempCoords.xyz / tempCoords.w) * 0.5 + 0.5, norm, lightDir, u_SpotLightShadowMaps[index])) * (diffuse + specular));
            }

            return ambient + diffuse + specular;
        }

    #endif

#endif

// Final fragment color
JOP_COLOR_OUT(0)

void main() 
{
#ifdef JDRW_SKYBOX
    JOP_FRAG_COLOR(0) = JOP_TEXTURE_CUBE(u_EnvironmentMap, vf_Position) * vf_Color;
#else

    // Assign the initial color
    vec4 tempColor = vf_Color

    #ifdef JMAT_DIFFUSEMAP
        * JOP_TEXTURE_2D(u_DiffuseMap, vf_TexCoords)
    #endif
    ;

    #ifdef JMAT_ENVIRONMENTMAP

        vec3 refl = vec3(JOP_TEXTURE_CUBE(u_EnvironmentMap, reflect(normalize(vf_Position - u_CameraPosition), normalize(vf_Normal))))

        #ifdef JMAT_REFLECTIONMAP
            * JOP_TEXTURE_2D(u_ReflectionMap, vf_TexCoords).a
        #endif
        #ifdef JMAT_LIGHTING
            * u_Material.reflectivity
        #endif
        ;

        tempColor += vec4(refl, 0.0);

    #endif

    // Do lighting calculations
    #ifdef JMAT_LIGHTING

        #if __VERSION__ >= 300
            #define JOP_POINT_LIMIT u_NumPointLights
            #define JOP_DIR_LIMIT u_NumDirectionalLights
            #define JOP_SPOT_LIMIT u_NumSpotLights
        #else
            #define JOP_POINT_LIMIT JMAT_MAX_POINT_LIGHTS
            #define JOP_DIR_LIMIT JMAT_MAX_DIRECTIONAL_LIGHTS
            #define JOP_SPOT_LIMIT JMAT_MAX_SPOT_LIGHTS
        #endif

        vec3 tempLight = vec3(0.0);

        if (u_ReceiveLights)
        {
        #if JMAT_MAX_POINT_LIGHTS > 0
            // Point lights
            for (int i = 0; i < JOP_POINT_LIMIT; ++i)
                tempLight += calculatePointLight(i);
        #endif
        
        #if JMAT_MAX_DIRECTIONAL_LIGHTS > 0
            // Directional lights
            for (int i = 0; i < JOP_DIR_LIMIT; ++i)
                tempLight += calculateDirectionalLight(i);
        #endif
                
        #if JMAT_MAX_SPOT_LIGHTS > 0
            // Spot lights
            for (int i = 0; i < JOP_SPOT_LIMIT; ++i)
                tempLight += calculateSpotLight(i);
        #endif
        }

        tempColor *= vec4(tempLight, u_Material.ambient.a * u_Material.diffuse.a * u_Material.specular.a);
        tempColor += u_Material.emission
        
        #ifdef JMAT_EMISSIONMAP
            * JOP_TEXTURE_2D(u_EmissionMap, vf_TexCoords)
        #endif
        ;

    #endif
    
    #ifdef JMAT_OPACITYMAP
        tempColor.a *= JOP_TEXTURE_2D(u_OpacityMap, vf_TexCoords).a;
    #endif

    // Finally assign to the fragment output
    JOP_FRAG_COLOR(0) = tempColor;

    #endif // Sky box
}