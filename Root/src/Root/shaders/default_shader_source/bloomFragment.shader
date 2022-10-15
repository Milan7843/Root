#version 460 core

out vec4 FragColor;

in vec2 textureCoords;

uniform sampler2D baseTexture;
uniform vec2 windowSize;

uniform float intensity;
uniform float threshold;

// Gaussian blur weights
float weights[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

float brightness(vec3 color)
{
    return (color.x + color.y + color.z) / 3.0;
}

void main()
{
    int bloomSize = 4;

    // Get offset of a single pixel
    vec2 pixelSize = 1.0 / windowSize;

    // Get the weighted colour of the current pixel
    vec3 bloomColor = vec3(0.);

    for (int y = -bloomSize; y <= bloomSize; y++)
    {
        for (int x = -bloomSize; x <= bloomSize; x++)
        {
            // Get the offset sample
            vec3 sampledColor = texture(baseTexture, textureCoords + vec2(pixelSize.x * x, pixelSize.y * y)).rgb;

            // Only use the sample if it has enough brightness
            if (brightness(sampledColor) > threshold)
                bloomColor += sampledColor * weights[abs(x)] * weights[abs(y)];
        }
    }

    FragColor = vec4(texture(baseTexture, textureCoords).rgb + bloomColor * intensity, 1.0);
}
