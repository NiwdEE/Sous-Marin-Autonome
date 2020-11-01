// Version du GLSL

#version 330 core


// Entrées

in vec3 in_Vertex;
in vec2 in_TexCoord0;
in vec3 in_Normals;


// Uniform

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 P;
uniform vec3 InSpace_LightPosition;


// Sortie

out vec2 coordTexture;
out vec3 InSpace_Position;
out vec3 InScreen_Normal;
out vec3 InScreen_EyeDirection;
out vec3 InScreen_LightDirection;

// Fonction main

void main()
{
    // Position finale du vertex en 3D

    gl_Position = MVP * vec4(in_Vertex, 1.0);

    InSpace_Position = ( mat4(1.0) * vec4(in_Vertex, 1)).xyz;

    vec3 InScreen_Vertex = ( MV * vec4(in_Vertex ,1)).xyz;
	InScreen_EyeDirection = vec3(0,0,0) - InScreen_Vertex;

	vec3 InScreen_LightPosition = ( MV * vec4(InSpace_LightPosition,1)).xyz;
	InScreen_LightDirection = InScreen_LightPosition + InScreen_EyeDirection;

	InScreen_Normal = ( MV * vec4(in_Normals, 0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	

    // Envoi des coordonnées de texture au Fragment Shader

    coordTexture = in_TexCoord0;
}
