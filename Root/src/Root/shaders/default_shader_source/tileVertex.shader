#version 460 core

layout(location = 0) in int tileIndex;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

uniform float renderDepth;

uniform ivec2 tileGridSize;

uniform float tileSize;

out int GeoIn_TileIndex;
out int GeoIn_LayerIndex;
out int GeoIn_TileID;

void main()
{
    vec2 position = -vec2(tileGridSize) / 2.0 + vec2(0.5, 0.5);

    int layer = int(gl_VertexID / (tileGridSize.x * tileGridSize.y));
    int layerIndex = int(mod(gl_VertexID, tileGridSize.x * tileGridSize.y));
    int x = int(mod(gl_VertexID,tileGridSize.x));
    int y = int(mod(int(gl_VertexID / tileGridSize.x), tileGridSize.y));

    position += vec2(x, y);

    // Screen position
    gl_Position = projection * view * model * vec4(position * tileSize * 2, 0.0, 1.0);
    // Depth
    gl_Position.z = renderDepth;

    GeoIn_TileIndex = tileIndex;
    GeoIn_LayerIndex = layer;
    GeoIn_TileID = gl_VertexID;
}