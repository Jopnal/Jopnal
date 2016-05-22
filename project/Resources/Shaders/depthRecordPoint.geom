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

#ifdef JMAT_ENVIRONMENT_RECORD
    in VertexData
    {
        vec3 Position;
        vec2 TexCoords;
        vec3 Normal;
        //vec3 Tangent;
        //vec3 BiTangent;
        vec4 Color;

    } inVert[];
    
    out FragVertexData
    {
        vec3 Position;
        vec2 TexCoords;
        vec3 Normal;
        //vec3 Tangent;
        //vec3 BiTangent;
        vec4 Color;

    } outVert;
#endif

void main()
{
    for (int face = 0; face < 6; ++face)
    {
        gl_Layer = face;

        for (int i = 0; i < 3; ++i)
        {
            vec4 temp = gl_in[i].gl_Position;
            gl_Position = (u_PVMatrices[face] * temp)
                
            #if defined(JMAT_SKYBOX) || defined(JMAT_SKYSPHERE)
                .xyww
            #endif
            ;

            vgf_FragPosition = vec3(temp);

            #ifdef JMAT_ENVIRONMENT_RECORD
                outVert.Position = inVert[i].Position;
                outVert.TexCoords = inVert[i].TexCoords;
                outVert.Normal = inVert[i].Normal;
                //outVert.Tangent = inVert[i].Tangent;
                //outVert.BiTangent = inVert[i].BiTangent;
                outVert.Color = inVert[i].Color;
            #endif

            EmitVertex();
        }

        EndPrimitive();
    }
}