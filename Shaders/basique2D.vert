#version 150 core


//Entrée shader
in vec2 in_Vertex;

void main()
{	
	gl_Position = vec4(in_Vertex, 0, 1);
}