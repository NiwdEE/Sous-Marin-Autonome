// Version du GLSL

#version 330 core


// Entrée

in vec2 coordTexture;
in vec3 InSpace_Position;
in vec3 InScreen_Normal;
in vec3 InScreen_EyeDirection;
in vec3 InScreen_LightDirection;



// Uniform

uniform sampler2D tex;
uniform mat4 MV;
uniform vec3 InSpace_LightPosition;


// Sortie 

out vec4 out_Color;


// Fonction main

void main()
{

	vec3 LightColor = vec3(1.0, 1.0, 1.0);
	float LightPower = 50.0f;

	vec3 MaterialDiffuseColor = texture( tex, coordTexture ).rgb;
	vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

	float LightDistance = length(InSpace_LightPosition - InSpace_Position);

	//Cosinus de l'angle entre la normale du triangle et le vecteur de la lumière ramené à [0;1]
	vec3 n = normalize( InScreen_Normal );
	vec3 l = normalize( InScreen_LightDirection );
	float cosTheta = clamp( dot( n, l ), 0, 1 );

	//Cosinus de l'angle entre la direction du regard et la direction de la lumière reflétée ramené à [0;1]
	float cosAlpha = clamp( dot( normalize(InScreen_EyeDirection),reflect(-l,n) ), 0, 1 );
	
    // Couleur du pixel

    //out_Color = texture(tex, coordTexture);
    out_Color = 
		(// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (LightDistance*LightDistance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (LightDistance*LightDistance), 1);
}
