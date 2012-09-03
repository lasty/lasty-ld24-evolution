
uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;

attribute vec3 vertex_position;
attribute vec3 vertex_colour;
//attribute vec2 vertex_texcoords;

varying vec4 varying_colour;
//varying vec2 varying_texcoords;

void main()
{
	varying_colour = vec4(vertex_colour, 1.0); 
	//varying_texcoords = vertex_texcoords;
	
	gl_Position = projection_matrix * modelview_matrix * vec4(vertex_position, 1.0);
}

%%

//uniform sampler2D texture_diffuse;
uniform vec4 tint_colour;

varying vec4 varying_colour;
//varying vec2 varying_texcoords;

void main(void)
{
	//gl_FragColor = texture2D(texture_diffuse, varying_texcoords);

	gl_FragColor = varying_colour;

	gl_FragColor *= tint_colour;	
}
