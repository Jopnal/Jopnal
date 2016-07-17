// JOPNAL BRIGHT FILTER FRAGMENT SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

uniform sampler2D u_Texture;
uniform float u_Threshold;

in vec2 vf_TexCoords;

layout(location = 0) out vec4 out_FinalColor;

const float offset[3] = float[](0.0, 1.3846153846, 3.2307692308);
const float weight[3] = float[](0.4162162162, 0.2270270270, 0.0702702703);

void main()
{
    vec2 size = textureSize(u_Texture, 0);

	vec4 tempColor = texture(u_Texture, vf_TexCoords) * weight[0];

    for (int i = 1; i < 3; ++i)
    {
        tempColor += texture(u_Texture, vf_TexCoords + vec2(offset[i] / size.x, 0.0)) * weight[i];
        tempColor += texture(u_Texture, vf_TexCoords - vec2(offset[i] / size.x, 0.0)) * weight[i];
    }
    for (int i = 1; i < 3; ++i)
    {
        tempColor += texture(u_Texture, vf_TexCoords + vec2(0.0, offset[i] / size.y)) * weight[i];
        tempColor += texture(u_Texture, vf_TexCoords - vec2(0.0, offset[i] / size.y)) * weight[i];
    }

	const float exponent = 4;

	if (dot(tempColor.rgb, vec3(0.2126, 0.7152, 0.0722)) > u_Threshold)
		out_FinalColor = tempColor;
	else
		out_FinalColor = pow(tempColor + vec4(1.0 - u_Threshold), vec4(exponent));
}