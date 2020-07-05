#include "Texture.h"
#include "../../../lib/stb_image/stb_image.h"

Texture2D::Texture2D(const char* fileName) : width(0), height(0), internalFormat(GL_RGBA), imageFormat(GL_RGBA), wrapS(GL_CLAMP_TO_BORDER), wrapT(GL_CLAMP_TO_BORDER), filterMin(GL_NEAREST), filterMax(GL_NEAREST) {
    // load image, create texture, and generate mipmaps
    stbi_set_flip_vertically_on_load(false);
    this->data = stbi_load(fileName, &width, &height, &nrChannels, 0);
    if(this->data) {
        Generate();
    }
}

void Texture2D::Generate() {
    // Create texture
    glGenTextures(1, &this->ID);
    glBindTexture(GL_TEXTURE_2D, this->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, this->width, this->height, 0, this->imageFormat, GL_UNSIGNED_BYTE, this->data);
    glGenerateMipmap(GL_TEXTURE_2D);
    // Wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMax);
    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const {
    glBindTexture(GL_TEXTURE_2D, this->ID);
}

bool Texture2D::IsValid() const {
    if(this->data) {
        return true;
    }
    return false;
}
