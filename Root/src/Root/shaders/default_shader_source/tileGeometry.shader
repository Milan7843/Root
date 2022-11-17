#version 460 core

layout(points) in;
layout(triangle_strip) out;

out vec2 FragIn_TexCoords;
out vec3 FragIn_BaseColor;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform float renderDepth;

vec2 offsets[] = {
	vec2( 0.5, 0.5),
	vec2( 0.5,-0.5),
	vec2(-0.5, 0.5),
	vec2( 0.5,-0.5),
	vec2(-0.5,-0.5),
	vec2(-0.5, 0.5)
};

void main()
{
	for (int i = 0; i < 6; i++)
	{
		FragIn_TexCoords = offsets[i] + vec2(0.5, 0.5);
		FragIn_BaseColor = baseColor[0];
		gl_Position = gl_in[0].gl_Position
			+ projection * view * model * vec4(rotationMatrix * vec2(offsets[i].x * particleSize[0].x, offsets[i].y * particleSize[0].y), 0.0, 1.0);
		// Depth
		gl_Position.z = renderDepth;
		EmitVertex();
		if (i == 2 || i == 5)
		{
			EndPrimitive();
		}
	}
}
