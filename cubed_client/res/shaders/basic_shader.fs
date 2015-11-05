#version 120

varying vec2 texCoord0;

uniform sampler2D sampler;

void main()
{
	vec4 color = texture2D(sampler, texCoord0);
	gl_FragColor = color;
}
