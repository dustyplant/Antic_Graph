#version 120
varying vec2 UV;
uniform sampler2D textureSampler;

void main()
{
	gl_FragColor = texture2D( textureSampler, UV );
	gl_FragColor.r = 0.f;
	gl_FragColor.g = 100.f;
	gl_FragColor.b = 100.f;
}