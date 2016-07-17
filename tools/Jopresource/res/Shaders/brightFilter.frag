// JOPNAL BRIGHT FILTER FRAGMENT SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

uniform sampler2D u_Texture;
uniform float u_Threshold;

in vec2 vf_TexCoords;

layout(location = 0) out vec4 out_FinalColor;

const float weight[2] = float[](1.0, 0.35);

void main()
{
    vec2 offset = 1.0 / textureSize(u_Texture, 0);

	vec4 tempColor = texture(u_Texture, vf_TexCoords) * weight[0];

    tempColor += texture(u_Texture, vec2(vf_TexCoords.x + offset.x, vf_TexCoords.y)) * weight[1];
    tempColor += texture(u_Texture, vec2(vf_TexCoords.x - offset.x, vf_TexCoords.y)) * weight[1];

    tempColor += texture(u_Texture, vec2(vf_TexCoords.x, vf_TexCoords.y + offset.y)) * weight[1];
    tempColor += texture(u_Texture, vec2(vf_TexCoords.x, vf_TexCoords.y - offset.y)) * weight[1];

	const float exponent = 4;

	if (dot(tempColor.rgb, vec3(0.2126, 0.7152, 0.0722)) > u_Threshold)
		out_FinalColor = tempColor;
	else
		out_FinalColor = pow(tempColor + vec4(1.0 - u_Threshold), vec4(exponent));
}