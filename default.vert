#version 460 core
layout (location = 0) in int aChunkInfo;

uniform mat4 uView;
uniform mat4 uProjection;
uniform vec3 uChunkPosition;
uniform int  uChunkSize;

out vec3 normal;
out int blockID;

void main()
{

    normal = vec3((aChunkInfo >> 14) & 1, (aChunkInfo >> 13) & 1, (aChunkInfo >> 12) & 1);
    blockID = (aChunkInfo >> 16) & 255;

    if(((aChunkInfo >> 15) & 1) == 1)
        normal *= -1;


    vec3 aPos = vec3((aChunkInfo >> 8) & 15, (aChunkInfo >> 4) & 15, aChunkInfo & 15) + uChunkPosition * uChunkSize;

    gl_Position = uProjection * uView * vec4(aPos, 1.0);
}
