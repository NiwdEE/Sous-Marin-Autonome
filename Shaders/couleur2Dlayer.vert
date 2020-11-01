// Version du GLSL

#version 150 core


// Entr�es

in vec2 in_Vertex;
in vec4 in_Color;

uniform float Layer;

// Sortie

out vec4 color;


// Fonction main

void main()
{
    // Position finale du vertex

    gl_Position = vec4(in_Vertex, Layer, 1.0);


    // Envoi de la couleur au Fragment Shader

    color = in_Color;
}
