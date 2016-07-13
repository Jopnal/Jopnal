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
    vec3 tempColor = texture(u_Scene, vf_TexCoords).rgb;

    #ifdef JPP_BLOOM
        tempColor += texture(u_Bloom, vf_TexCoords).rgb;
    #endif

    tempColor =
        
    #ifdef JPP_TONEMAP
        vec3(1.0) - exp(-tempColor * u_Exposure)
    #else
        tempColor
    #endif
    ;

	#ifdef JPP_GAMMACORRECTION
		tempColor = pow(tempColor, vec3(1.0 / u_Gamma));
	#endif

	out_FinalColor = vec4(tempColor, 1.0);

    #ifdef JPP_DITHER
        out_FinalColor += vec4(texture2D(u_DitherMatrix, gl_FragCoord.xy / 8.0).r / 64.0 - (1.0 / 128.0));
    #endif
}