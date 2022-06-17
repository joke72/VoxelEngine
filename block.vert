#version 460 core
layout (location = 0) in int aChunkInfo;

uniform mat4 uView;
uniform mat4 uProjection;
uniform vec3 uChunkPosition;
uniform int  uChunkSize;

out vec3 FragPos;
out vec3 normal;
out int blockID;

void main()
{

    vec3 aPos = vec3((aChunkInfo >> 12) & 63, (aChunkInfo >> 6) & 63, (aChunkInfo) & 63) + uChunkPosition * uChunkSize;

    normal = (vec3((aChunkInfo >> 20) & 1, (aChunkInfo >> 19) & 1, (aChunkInfo >> 18) & 1));
    blockID = (aChunkInfo >> 24) & 255;

    if(((aChunkInfo >> 21) & 1) == 1)
        normal *= -1;

    gl_Position = uProjection * uView * vec4(aPos, 1.0);

	FragPos = aPos;
}
