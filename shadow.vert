#version 460 core
layout (location = 0) in int aChunkInfo;

uniform mat4 uLightSpaceMatrix;
uniform vec3 uChunkPosition;
uniform int  uChunkSize;

void main()
{

    vec3 aPos = vec3((aChunkInfo >> 12) & 63, (aChunkInfo >> 6) & 63, (aChunkInfo) & 63) + uChunkPosition * uChunkSize;

    gl_Position = uLightSpaceMatrix * vec4(aPos, 1.0);

}
