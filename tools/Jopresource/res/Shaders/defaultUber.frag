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

// Vertex attribute data
JOP_VARYING_IN vec3 vf_Position;
JOP_VARYING_IN vec2 vf_TexCoords;
JOP_VARYING_IN vec3 vf_Normal;
JOP_VARYING_IN vec4 vf_Color;

// Light info
#ifdef JMAT_LIGHTING

    #include <Jopnal/DefaultLighting/Uniforms>

    #ifdef JMAT_PHONG

        #include <Jopnal/DefaultLighting/Shadows>
        #include <Jopnal/DefaultLighting/Lighting>

    #else

        #ifdef JMAT_FLAT
            JOP_FLAT
        #endif
        JOP_VARYING_IN vec3 vf_AmbDiffLight;

        #ifdef JMAT_FLAT
            JOP_FLAT
        #endif
        JOP_VARYING_IN vec3 vf_SpecLight;

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

        vec3 refl = vec3(JOP_TEXTURE_CUBE(u_EnvironmentMap, reflect(normalize(vf_Position), vf_Normal)))

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

        vec3 tempLight[3];

        #ifdef JMAT_PHONG

            tempLight[0] = vec3(0.0);
            tempLight[1] = vec3(0.0);
            tempLight[2] = vec3(0.0);

            float shininessMult =
            #ifdef JMAT_GLOSSMAP
                JOP_TEXTURE_2D(u_GlossMap, vf_TexCoords).a
            #else
                1.0
            #endif
            ;

            if (u_ReceiveLights)
            {
                vec3 light[3];

            #if JMAT_MAX_POINT_LIGHTS > 0

                // Point lights
                for (int i = 0; i < JOP_POINT_LIMIT; ++i)
                {
                    jop_CalculatePointLight(i, shininessMult, light[0], light[1], light[2]);

                    // Shadow calculation
                    float shadow = 1.0;
                    if (u_PointLights[i].castShadow && u_ReceiveShadows)
                        shadow -= jop_CalculatePointShadow(u_PointLights[i].position - vf_Position, u_PointLights[i].farPlane, u_PointLightShadowMaps[i]);

                    tempLight[0] += light[0];
                    tempLight[1] += light[1] * shadow;
                    tempLight[2] += light[2] * shadow;
                }

            #endif
            
            #if JMAT_MAX_DIRECTIONAL_LIGHTS > 0

                // Directional lights
                for (int i = 0; i < JOP_DIR_LIMIT; ++i)
                {
                    jop_CalculateDirectionalLight(i, shininessMult, light[0], light[1], light[2]);

                    // Shadow calculation
                    float shadow = 1.0;
                    if (u_DirectionalLights[i].castShadow && u_ReceiveShadows)
                        shadow -= jop_CalculateDirSpotShadow(vec3(u_DirectionalLights[i].lsMatrix * vec4(vf_Position, 1.0)) * 0.5 + 0.5, vf_Normal, -u_DirectionalLights[i].direction,  u_DirectionalLightShadowMaps[i]);

                    tempLight[0] += light[0];
                    tempLight[1] += light[1] * shadow;
                    tempLight[2] += light[2] * shadow;
                }

            #endif
                    
            #if JMAT_MAX_SPOT_LIGHTS > 0

                // Spot lights
                for (int i = 0; i < JOP_SPOT_LIMIT; ++i)
                {
                    jop_CalculateSpotLight(i, shininessMult, light[0], light[1], light[2]);

                    // Shadow calculation
                    float shadow = 1.0;
                    if (u_SpotLights[i].castShadow && u_ReceiveShadows)
                    {
                        vec4 tempCoords = u_SpotLights[i].lsMatrix * vec4(vf_Position, 1.0);
                        shadow -= jop_CalculateDirSpotShadow((tempCoords.xyz / tempCoords.w) * 0.5 + 0.5, vf_Normal, u_SpotLights[i].position - vf_Position, u_SpotLightShadowMaps[i]);
                    }
                    
                    tempLight[0] += light[0];
                    tempLight[1] += light[1] * shadow;
                    tempLight[2] += light[2] * shadow;
                }

            #endif
            }

        #else

            tempLight[0] = vf_AmbDiffLight;
            tempLight[1] = vec3(0.0);
            tempLight[2] = vf_SpecLight;

        #endif //JMAT_PHONG

        #ifdef JMAT_SPECULARMAP
            tempLight[2] *= vec3(JOP_TEXTURE_2D(u_SpecularMap, vf_TexCoords));
        #endif

        tempColor *= vec4(tempLight[0] + tempLight[1] + tempLight[2], u_Material.ambient.a * u_Material.diffuse.a * u_Material.specular.a);
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