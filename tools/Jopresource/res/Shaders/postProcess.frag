// JOPNAL POST PROCESS FRAGMENT SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

#include <Jopnal/Compat/FragmentColor>
#include <Jopnal/Compat/Varyings>
#include <Jopnal/Compat/Samplers>

JOP_VARYING_IN vec2 vf_TexCoords;

uniform sampler2D u_Scene;

uniform float u_Exposure;
uniform sampler2D u_DitherMatrix;
uniform float u_Gamma;

#ifdef JPP_BLOOM
    uniform sampler2D u_Bloom[JPP_BLOOM_TEXTURES];
#endif

JOP_COLOR_OUT(0)

void main()
{
    vec4 tempColor = JOP_TEXTURE_2D(u_Scene, vf_TexCoords);

    #ifdef JPP_BLOOM

        for (int i = 0; i < int(u_Bloom.length()); ++i)
            tempColor += (JOP_TEXTURE_2D(u_Bloom[i], vf_TexCoords) * max(1.0, float(i) * 2.0));

    #endif

    tempColor = vec4(
        
    #ifdef JPP_TONEMAP
        vec3(1.0) - exp(-tempColor.rgb * u_Exposure)
    #else
        tempColor.rgb
    #endif
    , 1.0);

    #ifdef JPP_DITHER
        tempColor += vec4(JOP_TEXTURE_2D(u_DitherMatrix, gl_FragCoord.xy / 8.0).r / 64.0 - (1.0 / 128.0));
    #endif

    JOP_FRAG_COLOR(0) =

    #ifdef JPP_GAMMACORRECTION
        vec4(pow(tempColor.rgb, vec3(1.0 / u_Gamma)), 1.0)
    #else
        tempColor
    #endif
    ;
}