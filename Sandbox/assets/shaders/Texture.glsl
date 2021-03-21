#type vertex
#version 330 core

layout(location = 0) in vec4 a_position;			
layout(location = 1) in vec2 a_texCoord;
uniform mat4 u_viewProjection;
uniform mat4 u_transformation;
out vec2 v_texCoord;

void main(){
	v_texCoord = a_texCoord * 2.0f;
	gl_Position = u_viewProjection * u_transformation * a_position;
}
#type fragment
#version 330 core

layout(location = 0) out vec4 o_color;			
in vec2 v_texCoord;

uniform sampler2D u_texture;
void main(){
	o_color =  texture(u_texture, v_texCoord);
}