#version 130
 
// Output color
out vec4 fragcolor;

// Variables from vertex to fragment shader
in vec3 _normal; //normal vector
in vec4 _position; //position vector

vec4 snowlineColor = vec4(0.7, 0.7, 0.7, 1.0);

void main()
{
	//simplified lighting model
    vec4 basecolor = vec4(_position.x/513, _position.y/513, _position.z/513, 1.0);
    fragcolor = _normal.z*basecolor;

	if(_position.z>80)
		fragcolor = _normal.z*snowlineColor;
		
}
