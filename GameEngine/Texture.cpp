#include "Texture.h"



Texture::Texture()
{
}

GLuint Texture::LoadTexture(const char* fileName)
{
	FIBITMAP* image = FreeImage_Load(FreeImage_GetFileType(fileName, 0), fileName);
	//ends if image holds nothing
	if (image == nullptr) return false;
	//convert textures to 32bit bmp;
	FIBITMAP* image32bit = FreeImage_ConvertTo32Bits(image);
	//unload old image from memeory 
	FreeImage_Unload(image);
	glGenTextures(1, &texInt);
	glBindTexture(GL_TEXTURE_2D, texInt);

	// upload texture bytes
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, FreeImage_GetWidth(image32bit),
		FreeImage_GetHeight(image32bit), 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image32bit));

	// set min filter instead of minmap linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	return texInt;

}



Texture::~Texture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &texInt);
}
