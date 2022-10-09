#version 460 core
layout(location = 0) in vec3 aPos;
//layout(location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Color;

uniform mat4 model;
uniform mat4 rotation; // Only the rotation part of the model matrix
uniform mat4 view;
uniform mat4 projection;
uniform vec3 inputColor;

void main()
{
	gl_PointSize = 2.;

	// Applying the model matrix to apply model transformations
	FragPos = vec3(model * vec4(aPos, 1.0));

	// Calculating screen position
	gl_Position = projection * view * vec4(FragPos, 1.0);

    Color = inputColor;

	// Rotating the model according to the model's rotation
	//Normal = normalize(vec3(rotation * vec4(aNormal, 1.0)));
}