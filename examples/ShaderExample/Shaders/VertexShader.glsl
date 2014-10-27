#version 120
attribute vec3 vertexPosition;
attribute vec2 vertexUV;
varying vec2 UV;
uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4( vertexPosition, 1 );
	UV = vertexUV;
}