#version 130
 
uniform mat4 matmodelview;
uniform mat4 matprojection;

in vec4 in_position;
in vec3 in_normal;
in vec3 in_color;

// End of header part

// Variables from vertex to fragment shader
out vec3 _normal;
out vec4 _position;
out vec3 _color;

void main()
{
	// Get a copy of the normal vector
	_normal = 2.0*in_normal;
	_position = in_position;
	_color = in_color;
	
	// Transform the vertex position to screencoordinates
	gl_Position = matprojection * (matmodelview * in_position);
}
