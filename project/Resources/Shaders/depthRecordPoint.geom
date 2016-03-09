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
out vec4 gf_FragPosition;

void main()
{
    for (int face = 0; face < 6; ++face)
    {
        gl_Layer = face;

        for (int i = 0; i < 3; ++i)
        {
            gf_FragPosition = gl_in[i].gl_Position;
            gl_Position = u_PVMatrices[face] * gf_FragPosition;

            EmitVertex();
        }

        EndPrimitive();
    }
}