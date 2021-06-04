#ifdef VERTEX_SHADER

////////////////////////////////////////////////////////////////////////////////
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

////////////////////////////////////////////////////////////////////////////////
uniform mat4 uPVM;

////////////////////////////////////////////////////////////////////////////////
out vec2 vTexCoord;

////////////////////////////////////////////////////////////////////////////////
void main()
{
        gl_Position = uPVM * vec4(aPos, 1.0f);
        vTexCoord = aTexCoord;
}

#elif defined(FRAGMENT_SHADER)

////////////////////////////////////////////////////////////////////////////////
in vec2 vTexCoord;

////////////////////////////////////////////////////////////////////////////////
uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform float uInterpolation;

////////////////////////////////////////////////////////////////////////////////
out vec4 color;

////////////////////////////////////////////////////////////////////////////////
void main()
{
        color = mix(texture(uTexture0, vTexCoord), texture(uTexture1, vTexCoord), uInterpolation);
}

#endif
