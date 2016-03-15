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
#ifndef JMAT_ENVIRONMENT_RECORD
    out vec4
#else
    out vec3
#endif
gf_FragPosition;

void main()
{
    for (int face = 0; face < 6; ++face)
    {
        gl_Layer = face;

        for (int i = 0; i < 3; ++i)
        {
            vec4 temp = gl_in[i].gl_Position;
            gl_Position = u_PVMatrices[face] * temp;

            #ifdef JMAT_ENVIRONMENT_RECORD
                gf_FragPosition = vec3(temp);
            #else
                gf_FragPosition = temp;
            #endif

            EmitVertex();
        }

        EndPrimitive();
    }
}