#version 330 core

out vec4 FragColor;

in vec4 vColor;
in vec2 vTexCoord;
in float vTexId;

uniform sampler2D u_Textures[32];

void main()
{
   int index = int(vTexId);
   if(index == -1) {
        FragColor = vColor;
   } else {
        FragColor = texture(u_Textures[index], vTexCoord);
   }
}