// JOPNAL GAUSSIAN BLUR FRAGMENT SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

in vec2 vf_TexCoords;

uniform sampler2D u_Buffer;

uniform bool u_Horizontal;
uniform float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

layout(location = 0) out vec4 out_FinalColor;

void main()
{
    vec2 tcOffset = 1.0 / textureSize(u_Buffer, 0);
    vec3 result = texture(u_Buffer, vf_TexCoords).rgb * weight[0];

    if (u_Horizontal)
    {
        for (int i = 1; i < 5; ++i)
        {
            result += texture(u_Buffer, vf_TexCoords + vec2(tcOffset.x * i, 0.0)).rgb * weight[i];
            result += texture(u_Buffer, vf_TexCoords - vec2(tcOffset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for (int i = 1; i < 5; ++i)
        {
            result += texture(u_Buffer, vf_TexCoords + vec2(0.0, tcOffset.y * i)).rgb * weight[i];
            result += texture(u_Buffer, vf_TexCoords - vec2(0.0, tcOffset.y * i)).rgb * weight[i];
        }
    }

    out_FinalColor = vec4(result, 1.0);
}