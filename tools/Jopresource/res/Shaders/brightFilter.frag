// JOPNAL BRIGHT FILTER FRAGMENT SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

#include <Jopnal/Compat/FragmentColor>

uniform sampler2D u_Texture;
uniform float u_Threshold;
uniform float u_SubExponent;

in vec2 vf_TexCoords;

JOP_COLOR_OUT(0)

const float offset[3] = float[](0.0, 1.3846153846, 3.2307692308);
const float weight[3] = float[](0.4162162162, 0.2270270270, 0.0702702703);

void main()
{
    ivec2 size = textureSize(u_Texture, 0);

	vec3 tempColor = texture(u_Texture, vf_TexCoords).rgb * weight[0];

    for (int i = 1; i < 3; ++i)
    {
        tempColor += texture(u_Texture, vf_TexCoords + vec2(offset[i] / float(size.x), 0.0)).rgb * weight[i];
        tempColor += texture(u_Texture, vf_TexCoords - vec2(offset[i] / float(size.x), 0.0)).rgb * weight[i];
    }
    for (int i = 1; i < 3; ++i)
    {
        tempColor += texture(u_Texture, vf_TexCoords + vec2(0.0, offset[i] / float(size.y))).rgb * weight[i];
        tempColor += texture(u_Texture, vf_TexCoords - vec2(0.0, offset[i] / float(size.y))).rgb * weight[i];
    }

    JOP_FRAG_COLOR(0) = vec4(max
    (
        float(dot(tempColor.rgb, vec3(0.2126, 0.7152, 0.0722)) > u_Threshold) * tempColor,  //< If the brightness exceeds the threshold
        pow(tempColor / vec3(u_Threshold), vec3(u_SubExponent))                             //< Otherwise dim the output color exponentally
                                                                                            //  to achieve a smooth transition
    ), 1.0);
}