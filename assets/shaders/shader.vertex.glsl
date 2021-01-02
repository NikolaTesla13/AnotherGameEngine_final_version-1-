#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aTexId;

out vec4 vColor;
out vec2 vTexCoord;
out float vTexId;

uniform mat4 ortho;

void main()
{
   vColor = aColor;
   vTexCoord = aTexCoord;
   vTexId = aTexId;
   gl_Position = ortho * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}