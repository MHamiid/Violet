#type vertex
#version 330 core

layout(location = 0) in vec4 a_position;			
uniform mat4 u_viewProjection;
uniform mat4 u_transformation;

void main(){
	gl_Position = u_viewProjection * u_transformation * a_position;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 o_color;			

uniform vec4 u_color;
void main(){
	o_color =  u_color;
}