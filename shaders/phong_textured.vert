#version 400 core

////////////////////////////////////////////////////////////////////////////////
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;

////////////////////////////////////////////////////////////////////////////////
uniform mat4 uPVM;
uniform mat4 uModel;
uniform mat4 uNormal;

uniform vec2 uTextureTile;
uniform vec2 uTextureOffset;

////////////////////////////////////////////////////////////////////////////////
out vec3 vNorm;
out vec2 vTexCoord;
out vec3 vFragPos;

////////////////////////////////////////////////////////////////////////////////
void main()
{
        gl_Position = uPVM * vec4(aPos, 1.0f);
        vNorm = vec3(uNormal * vec4(aNorm, 0.0f));
        vFragPos = vec3(uModel * vec4(aPos, 1.0f));
        vTexCoord = aTexCoord * uTextureTile + uTextureOffset;
}
