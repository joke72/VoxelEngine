#version 460 core

uniform vec3 uCameraFront;
uniform vec2 uScreenSize;

out vec4 FragColor;

void main()
{
	// Pixel position in screen space, x = [-1, 1], y = [-1, 1]
	vec2 screenPos = (gl_FragCoord.xy / uScreenSize) * 2 - 1;
	
    //vec3 uCameraPosition = vec3(30, 10, 30);
    vec3 worldUp = vec3(0,1,0);
    vec3 camDir = normalize(uCameraFront);//normalize(lookAt - uCameraPosition);
    vec3 camRight = normalize(cross(camDir, worldUp));
    vec3 camUp = cross(camRight, camDir);
    
    vec2 filmSize = vec2(1,uScreenSize.y / uScreenSize.x);
    
    vec3 filmPos = normalize(camDir + screenPos.x*filmSize.x*camRight + screenPos.y*filmSize.y*camUp);
    
    float val = filmPos.y * 0.8 + 0.4 + 0.3;

	FragColor = vec4(val * 0.2, val * 0.2, val, 1.0);
}
