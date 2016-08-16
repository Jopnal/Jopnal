// JOPNAL DEPTH RECORD SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

// Input triangle
layout(triangles) in;

// Output triangles
layout(triangle_strip, max_vertices = 18) out;

// Light space matrices
uniform mat4 u_PVMatrices[6];

// Fragment position to fragment shader
out vec3 vgf_FragPosition;

void main()
{
    for (int face = 0; face < 6; ++face)
    {
        gl_Layer = face;

        for (int i = 0; i < 3; ++i)
        {
            vec4 temp = gl_in[i].gl_Position;
            gl_Position = (u_PVMatrices[face] * temp);

            vgf_FragPosition = vec3(temp);

            EmitVertex();
        }

        EndPrimitive();
    }
}