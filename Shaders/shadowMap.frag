#version 330

in vec3 pos_world;

uniform vec3 lightPos;

out float out_Color;

void main()
{
    vec3 LightToVertex = pos_world - lightPos;

    float LightToPixelDistance = length(LightToVertex);

    out_Color = LightToPixelDistance;
}

