#include "TextureData.h"

// stb_image for image loading
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stbi_image.h>
#include "TextureData.h"

TextureData::TextureData(char const* filename, int* x, int* y, int* comp, int req_comp)
{
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(filename, x, y, comp, req_comp);
}

TextureData::~TextureData()
{
    if (data)
        stbi_image_free(data);
}

bool TextureData::hasData()
{
    // Is this necessary?
    if (data)
        return true;
    return false;
}

unsigned char* TextureData::getData()
{
    return data;
}

const char* TextureData::getFailureReason()
{
    return stbi_failure_reason();
}
