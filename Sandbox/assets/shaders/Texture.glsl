#type vertex
#version 330 core

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_color;			
layout(location = 2) in vec2 a_texCoord;

uniform mat4 u_viewProjection;
uniform float u_sizeFactor;

out vec4 v_color;
out vec2 v_texCoord;

void main(){
	gl_Position = u_viewProjection * a_position;

	v_color = a_color;
	v_texCoord = a_texCoord * u_sizeFactor;
}
#type fragment
#version 330 core

layout(location = 0) out vec4 o_color;	

uniform sampler2D u_texture;

in vec4 v_color;
in vec2 v_texCoord;

void main(){
	//o_color =  texture(u_texture, v_texCoord) * u_color;
	o_color = v_color;
}