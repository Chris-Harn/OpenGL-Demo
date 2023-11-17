#shader vertex
#version 460 core

layout( location = 0 ) in vec2 aPos;
layout( location = 1 ) in vec2 aTexCoords;

out vec2 TexCoords;

void main() {
	gl_Position = vec4( aPos, 0.0, 1.0 );
	TexCoords = aTexCoords;
}

#shader fragment
#version 460 core

in vec2 TexCoords;
out vec4 FragColor;

void main() {
	// Test Code
	FragColor = vec4( TexCoords.x, TexCoords.y, 0.0, 1.0 );
}