// JOPNAL DEFAULT VERTEX UBERSHADER
//
// Jopnal license applies

//////////////////////////////////////////////

#include <Jopnal/Compat/Attributes>
#include <Jopnal/Compat/Varyings>

// Vertex attributes
JOP_ATTRIBUTE(0) vec4 a_Position;
JOP_ATTRIBUTE(1) vec2 a_TexCoords;
JOP_ATTRIBUTE(2) vec3 a_Normal;
JOP_ATTRIBUTE(3) vec4 a_Color;

// Matrices
uniform mat4 u_VMMatrix;
uniform mat4 u_PVMMatrix;
uniform mat3 u_NMatrix;

// Vertex attributes to fragment shader
JOP_VARYING_OUT vec3 vf_Position;
JOP_VARYING_OUT vec2 vf_TexCoords;
JOP_VARYING_OUT vec3 vf_Normal;
JOP_VARYING_OUT vec4 vf_Color;

#ifdef JMAT_GOURAUD

    #include <Jopnal/DefaultLighting/Uniforms>
    #include <Jopnal/DefaultLighting/Lighting>

    uniform bool u_ReceiveLights;

    #ifdef JMAT_FLAT
        JOP_FLAT
    #endif
    JOP_VARYING_OUT vec3 vf_AmbDiffLight;
    #ifdef JMAT_FLAT
        JOP_FLAT
    #endif
    JOP_VARYING_OUT vec3 vf_SpecLight;

#endif

void main()
{
    // Assign attributes
    vf_Position     = (
    #if !defined(JDRW_SKYBOX) && !defined(JDRW_SKYSPHERE)
        u_VMMatrix * 
    #endif
                      a_Position).xyz;
    vf_TexCoords    = a_TexCoords;
    vf_Normal       = normalize(u_NMatrix * a_Normal);
    vf_Color        = a_Color;

    // Calculate and assign position
    gl_Position = (u_PVMMatrix * a_Position)
    
    #if (defined(JDRW_SKYBOX) || defined(JDRW_SKYSPHERE))
        .xyww
    #endif
    ;

    // Gouraud/flat lighting
    #ifdef JMAT_GOURAUD

        vf_AmbDiffLight = vec3(0.0);
        vf_SpecLight = vec3(0.0);

        if (u_ReceiveLights)
        {
            vec3 light[3];

        #if JMAT_MAX_POINT_LIGHTS > 0

            // Point lights
            for (int i = 0; i < JOP_POINT_LIMIT; ++i)
            {
                jop_CalculatePointLight(i, 1.0, light[0], light[1], light[2]);

                vf_AmbDiffLight += light[0] + light[1];
                vf_SpecLight += light[2];
            }

        #endif
        
        #if JMAT_MAX_DIRECTIONAL_LIGHTS > 0

            // Directional lights
            for (int i = 0; i < JOP_DIR_LIMIT; ++i)
            {
                jop_CalculateDirectionalLight(i, 1.0, light[0], light[1], light[2]);

                vf_AmbDiffLight += light[0] + light[1];
                vf_SpecLight += light[2];
            }

        #endif
                
        #if JMAT_MAX_SPOT_LIGHTS > 0

            // Spot lights
            for (int i = 0; i < JOP_SPOT_LIMIT; ++i)
            {
                jop_CalculateSpotLight(i, 1.0, light[0], light[1], light[2]);
                
                vf_AmbDiffLight += light[0] + light[1];
                vf_SpecLight += light[2];
            }

        #endif
        }

    #endif
}