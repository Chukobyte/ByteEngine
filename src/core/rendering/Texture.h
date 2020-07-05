#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture2D {
  public:
    GLuint ID = 0;
    unsigned char* data;
    int width;
    int height;
    int nrChannels;
    // format
    unsigned int internalFormat;
    unsigned int imageFormat;
    // configuration
    unsigned int wrapS;
    unsigned int wrapT;
    unsigned int filterMin;
    unsigned int filterMax;

    Texture2D(const char* fileName);
    void Bind() const;
    bool IsValid() const;
  private:
    void Generate();
};

#endif
