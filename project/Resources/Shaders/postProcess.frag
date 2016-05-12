// JOPNAL POST PROCESS FRAGMENT SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

in vec2 vf_TexCoords;

uniform sampler2D u_Scene;

#ifdef JPP_TONEMAP
    uniform float u_Exposure;
#endif

#ifdef JPP_BLOOM
    uniform sampler2D u_Bloom;
#endif


layout(location = 0) out vec4 out_FinalColor;

void main()
{
    vec3 tempColor = texture(u_Scene, vf_TexCoords).rgb;

    #ifdef JPP_BLOOM
        tempColor += texture(u_Bloom, vf_TexCoords).rgb;
    #endif

    out_FinalColor = vec4(vec3(1.0) - exp(-tempColor * u_Exposure), 1.0);
}