#version 130
 
// Output color
out vec4 fragcolor; //instead using build in variable gl_FragColor

// Variables from vertex to fragment shader
in vec3 _normal; //normal vector
in vec4 _position; //position vector
in vec3 _color;

vec4 snowlineColor = vec4(0.1, 0.1, 0.1, 1.0);

void main()
{
	//vec4 basecolor = vec4(_color, 1.0);
	vec4 basecolor = vec4(_color, 1.0);
	//vec4 basecolor = vec4(0.3, 0.3, 0.3, 1.0);
    //vec4 basecolor = vec4(_position.x/513, _position.y/513, _position.z/513, 1.0);
	
	//simplified lighting model
    fragcolor = _normal.z*basecolor;

	//if(_position.z>80)
	//	fragcolor = _normal.z*snowlineColor;
}
