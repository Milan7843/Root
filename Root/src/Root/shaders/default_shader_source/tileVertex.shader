#version 460 core

layout(location = 0) in int tileIndex;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

uniform float renderDepth;

uniform ivec2 tileGridSize;

uniform float tileSize;

out int GeoIn_TileIndex;

void main()
{
    vec2 position = -vec2(tileGridSize) / 2.0 + vec2(0.5, 0.5);

    int layer = int(gl_VertexID / (tileGridSize.x * tileGridSize.y));
    int layerIndex = int(mod(gl_VertexID, tileGridSize.x * tileGridSize.y));
    int x = int(mod(gl_VertexID,tileGridSize.x));
    int y = int(gl_VertexID / tileGridSize.x);

    position += vec2(x, y);

    // Screen position
    gl_Position = projection * view * model * vec4(position * tileSize, 0.0, 1.0);
    // Depth
    gl_Position.z = renderDepth;

    GeoIn_TileIndex = tileIndex;
}