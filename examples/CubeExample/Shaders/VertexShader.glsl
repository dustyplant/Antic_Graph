#version 120

attribute vec3 vertexPosition;
attribute vec3 vertexColor;

varying vec3 fragmentColor;

uniform mat4 MVP;

void main() {
	gl_Position = MVP * vec4( vertexPosition, 1 );
	
	fragmentColor = vertexColor;
}
