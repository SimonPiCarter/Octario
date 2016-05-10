#version 330

in vec3 pos_world;

uniform vec3 lightPos;

out float FragColor;
        
void main()
{
    vec3 LightToVertex = pos_world - lightPos;

    float LightToPixelDistance = dot(LightToVertex,LightToVertex);

    FragColor = LightToPixelDistance;
}

