#ifndef TEXTURE_H
#define TEXTURE_H

class Texture{
public:
	GLuint id;
	
	Texture(const char* path){
		id = LoadTexture(path);
	}
	
	GLuint LoadTexture(const char* path) {
		GLuint textureImage;
		int w, h, channels;
		unsigned char* image;
		image = stbi_load(path, &w, &h, &channels, STBI_rgb);

		if (image == nullptr)
			printf("Failed to load texture %s !", path);

		glGenTextures(1, &textureImage);
		glCheckError();
		glBindTexture(GL_TEXTURE_2D, textureImage);
		glCheckError();
		// 3 channels = RGB ; 4 channel = RGBA
		if (channels==3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			glCheckError();
		} else if (channels==4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glCheckError();
		}
		glGenerateMipmap(GL_TEXTURE_2D);
		glCheckError();
		// we set how to consider UVs outside [0,1] range
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glCheckError();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glCheckError();
		// we set the filtering for minification and magnification
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glCheckError();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glCheckError();

		// we free the memory once we have created an OpenGL texture
		stbi_image_free(image);

		return textureImage;
	}
	
	void Delete() { glDeleteTextures(1, &id); }
};
#endif //	TEXTURE_H