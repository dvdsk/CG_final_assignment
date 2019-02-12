#version 130

uniform vec3 camera_position;

// Output color
out vec4 fragcolor; //instead using build in variable gl_FragColor

// Variables from vertex to fragment shader
in vec4 _waterpos; //position vector

// Light properties
vec3 light_position = vec3(0.,300.,300); // Position in world coordinates
//vec3 light_targetposition; // only important for spot lights
vec3 light_intensity = vec3(1, 1, 1);

void main()
{
	fragcolor = vec4(1,1,1,1);
}
