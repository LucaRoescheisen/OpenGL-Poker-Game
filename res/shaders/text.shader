#shader vertex
#version 330 core

layout (location = 0) in vec4 coord;
out vec2 v_TexCoord;

void main() {
    gl_Position = vec4(coord.xy, 0, 1);
    v_TexCoord = coord.zw;
};


#shader fragment
#version 330 core


in vec2 v_TexCoord;
out vec4 outColor;
uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main() {
    float alpha = texture(u_Texture, v_TexCoord).r;
    outColor = vec4(1.0, 1.0, 1.0, alpha) * u_Color;
};