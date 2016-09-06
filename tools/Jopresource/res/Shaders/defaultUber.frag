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

// Camera position
uniform vec3 u_CameraPosition;

// Vertex attribute data
JOP_VARYING_IN vec3 vf_Position;
JOP_VARYING_IN vec2 vf_TexCoords;
JOP_VARYING_IN vec3 vf_Normal;
JOP_VARYING_IN vec4 vf_Color;

// Light info
#ifdef JMAT_LIGHTING

    #include <Jopnal/DefaultLighting/Uniforms>
    #include <Jopnal/DefaultLighting/Shadows>
    #include <Jopnal/DefaultLighting/Lighting>

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
            vec3 light[3];

        #if JMAT_MAX_POINT_LIGHTS > 0

            // Point lights
            for (int i = 0; i < JOP_POINT_LIMIT; ++i)
            {
                jop_CalculatePointLight(i, light[0], light[1], light[2]);
                tempLight += light[0] + light[1] + light[2];
            }

        #endif
        
        #if JMAT_MAX_DIRECTIONAL_LIGHTS > 0

            // Directional lights
            for (int i = 0; i < JOP_DIR_LIMIT; ++i)
            {
                jop_CalculateDirectionalLight(i, light[0], light[1], light[2]);
                tempLight += light[0] + light[1] + light[2];
            }

        #endif
                
        #if JMAT_MAX_SPOT_LIGHTS > 0

            // Spot lights
            for (int i = 0; i < JOP_SPOT_LIMIT; ++i)
            {
                jop_CalculateSpotLight(i, light[0], light[1], light[2]);
                tempLight += light[0] + light[1] + light[2];
            }

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