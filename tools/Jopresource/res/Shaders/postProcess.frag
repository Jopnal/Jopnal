// JOPNAL POST PROCESS FRAGMENT SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

in vec2 vf_TexCoords;

uniform sampler2D u_Scene;

uniform float u_Exposure;
uniform sampler2D u_Bloom;
uniform sampler2D u_DitherMatrix;
uniform float u_Gamma;

layout(location = 0) out vec4 out_FinalColor;

void main()
{
    vec4 tempColor = texture(u_Scene, vf_TexCoords);

    #ifdef JPP_BLOOM
        tempColor += texture(u_Bloom, vf_TexCoords);
    #endif

    tempColor = vec4(
        
    #ifdef JPP_TONEMAP
        vec3(1.0) - exp(-tempColor.rgb * u_Exposure)
    #else
        tempColor.rgb
    #endif
    , 1.0);

    #ifdef JPP_DITHER
        tempColor += vec4(texture(u_DitherMatrix, gl_FragCoord.xy / 8.0).r / 64.0 - (1.0 / 128.0));
    #endif

	#ifdef JPP_GAMMACORRECTION
		out_FinalColor = vec4(pow(tempColor.rgb, vec3(1.0 / u_Gamma)), 1.0);
	#else
		out_FinalColor = tempColor;
	#endif
}