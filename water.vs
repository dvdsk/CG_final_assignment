#version 130
 
uniform mat4 matmodelview;
uniform mat4 matprojection;

in vec4 in_waterpos;

// End of header part

// Variables from vertex to fragment shader
out vec4 _waterpos;

void main()
{
	// Get a copy of the normal vector
	_waterpos = in_waterpos;
	
	// Transform the vertex position to screencoordinates
	gl_Position = matprojection * (matmodelview * in_waterpos);
}
