// JOPNAL BRIGHT FILTER FRAGMENT SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

uniform sampler2D u_Texture;
uniform float u_Threshold;

in vec2 vf_TexCoords;

layout(location = 0) out vec4 out_FinalColor;

void main()
{
	vec4 tempColor = texture(u_Texture, vf_TexCoords);

	const float exponent = 2.8;

	if (dot(tempColor.rgb, vec3(0.2126, 0.7152, 0.0722)) > u_Threshold)
		out_FinalColor = tempColor;
	else
		out_FinalColor = pow(tempColor + vec4(1.0 - u_Threshold), vec4(exponent));
}