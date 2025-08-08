#version 460 core

uniform vec2 uScreenSize;

uniform vec3 uCameraPosition;
uniform vec3 uCameraFront;

uniform vec3 uLightPosition;

uniform float uFOV;

uniform vec3  uSpherePosition;
uniform float uSphereRadius;


out vec4 FragColor;

vec4 ray(vec3 origin, vec3 dir)
{
	dir = normalize(dir);
	float dist = distance(origin, uSpherePosition);
//	if(dist < uSphereRadius)
//		return vec4(0);

	vec3 dirSq = dir * dir;
	vec3 oriSq = origin * origin;
	
	
	vec3 B = origin * dir - 2 * dir * uSpherePosition;
	vec3 C = oriSq - 2 * dirSq * uSpherePosition + uSpherePosition * uSpherePosition;

	float a = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;//dirSq.x + dirSq.y + dirSq.z;
	float b = B.x + B.y + B.z;
	float c = C.x + C.y + C.z - uSphereRadius * uSphereRadius;

	if(b * b - 4 * a * c < 0)
		return vec4(0);

	float t1 = (-b * sqrt(b*b - 4 * a * c)) / (2 * a);
	float t2 = (-b * -sqrt(b*b - 4 * a * c)) / (2 * a);
	
	float t;
	
	
	vec3 result1 = origin + dir * t1;
	vec3 result2 = origin + dir * t2;
	
	float dist1 = distance(result1, origin);
	float dist2 = distance(result2, origin);
	vec3 result;
	if(dist1 < dist2) {
		result = result1;
		t = t1;
	}
	else {
		result = result2;
		t = t2;
	}

	if(t > 0)
		return vec4(result, 0);
	
	return vec4(result, 1);

/*	if(dis > 0)
		return normalize(vec3(0.8, 0.2, 0.2)) * dis / 50;
	return vec3(0);*/
}

void main()
{
	// Top right view frustum coordinates in local space
	float z = -5;

	float x = tan(uFOV / 2) * -z;
	float y = x * uScreenSize.y / uScreenSize.x;

	// View frustum coordinates in world space	
	vec3 topLeft     = uCameraPosition + vec3(-x, y, z);
	vec3 topRight    = uCameraPosition + vec3( x, y, z);
	vec3 bottomLeft  = uCameraPosition + vec3(-x,-y, z);
	vec3 bottomRight = uCameraPosition + vec3(-x, y, z);
	
	// Width and height of view frustum
	float xDiff = 2 * x;
	float yDiff = 2 * y;

	// Pixel position in screen space, x = [-1, 1], y = [-1, 1]
	vec2 screenPos = (gl_FragCoord.xy / uScreenSize) * 2 - 1;

	// Vector from camera to current pixel in world space
	vec3 offsetVector = vec3(x * screenPos.x, y*screenPos.y, z);

	// Pixel position in world space
	vec3 pixelWorldPos = offsetVector + uCameraPosition;


	vec4 primary = ray(pixelWorldPos, offsetVector);
	vec3 col = vec3(0);
	if(primary.w > 0)
	{
		vec3 dir =  primary.xyz - uLightPosition;
		dir = normalize(dir);
		vec4 secondary = ray(primary.xyz, dir);
			float dist = distance(uLightPosition, primary.xyz);
		if(secondary.w > 0)
			col = vec3(1/dist);
		else
		{
			col = vec3(1/dist);
		}
	}
	if(primary.w > 0)
		primary.w = 1/distance(primary.xyz, uCameraPosition);
	FragColor = vec4(1);//vec4(primary.w,primary.w,primary.w,1);//vec4(gl_FragCoord.x / uScreenSize.x, gl_FragCoord.y / uScreenSize.y, 1.0, 1.0);
	gl_FragDepth = 0.99;
}
