#ifndef SKYMAP_H
#define SKYMAP_H

#include <string>

#include <utils/gl_error.h>

#include <utils/camera.h>
#include <utils/shader_v1.h>
#include <utils/texture.h>
#include <glm/gtx/string_cast.hpp>

#define SKY_DIM 1000.0f

float points[] = {
  -SKY_DIM,  SKY_DIM, -SKY_DIM,
  -SKY_DIM, -SKY_DIM, -SKY_DIM,
   SKY_DIM, -SKY_DIM, -SKY_DIM,
   SKY_DIM, -SKY_DIM, -SKY_DIM,
   SKY_DIM,  SKY_DIM, -SKY_DIM,
  -SKY_DIM,  SKY_DIM, -SKY_DIM,
  
  -SKY_DIM, -SKY_DIM,  SKY_DIM,
  -SKY_DIM, -SKY_DIM, -SKY_DIM,
  -SKY_DIM,  SKY_DIM, -SKY_DIM,
  -SKY_DIM,  SKY_DIM, -SKY_DIM,
  -SKY_DIM,  SKY_DIM,  SKY_DIM,
  -SKY_DIM, -SKY_DIM,  SKY_DIM,
  
   SKY_DIM, -SKY_DIM, -SKY_DIM,
   SKY_DIM, -SKY_DIM,  SKY_DIM,
   SKY_DIM,  SKY_DIM,  SKY_DIM,
   SKY_DIM,  SKY_DIM,  SKY_DIM,
   SKY_DIM,  SKY_DIM, -SKY_DIM,
   SKY_DIM, -SKY_DIM, -SKY_DIM,
   
  -SKY_DIM, -SKY_DIM,  SKY_DIM,
  -SKY_DIM,  SKY_DIM,  SKY_DIM,
   SKY_DIM,  SKY_DIM,  SKY_DIM,
   SKY_DIM,  SKY_DIM,  SKY_DIM,
   SKY_DIM, -SKY_DIM,  SKY_DIM,
  -SKY_DIM, -SKY_DIM,  SKY_DIM,
  
  -SKY_DIM,  SKY_DIM, -SKY_DIM,
   SKY_DIM,  SKY_DIM, -SKY_DIM,
   SKY_DIM,  SKY_DIM,  SKY_DIM,
   SKY_DIM,  SKY_DIM,  SKY_DIM,
  -SKY_DIM,  SKY_DIM,  SKY_DIM,
  -SKY_DIM,  SKY_DIM, -SKY_DIM,
  
  -SKY_DIM, -SKY_DIM, -SKY_DIM,
  -SKY_DIM, -SKY_DIM,  SKY_DIM,
   SKY_DIM, -SKY_DIM, -SKY_DIM,
   SKY_DIM, -SKY_DIM, -SKY_DIM,
  -SKY_DIM, -SKY_DIM,  SKY_DIM,
   SKY_DIM, -SKY_DIM,  SKY_DIM
};

class SkyMap {
private:
	Shader *shader;
	glm::mat4 projectionMatrix;
	void create_cube_map(const char* front, const char* back, const char* top, const char* bottom, const char* left, const char* right);
	bool load_cube_map_side(GLuint texture, GLenum side_target, const char* file_name);
public:
	GLuint vbo;
	GLuint vao;
	GLuint tex_cube;
	Camera *camera;
	
	SkyMap(Camera *camera, glm::mat4 &projection,
	const char* front, const char* back, const char* top, const char* bottom, const char* left, const char* right);
	void Update();
};

SkyMap::SkyMap(Camera *camera, glm::mat4 &projection,
	const char* front, const char* back, const char* top, const char* bottom, const char* left, const char* right){
	this->camera = camera;
	this->projectionMatrix = projection;
	//setup vbo
	glGenBuffers(1, &vbo);
	glCheckError();
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glCheckError();
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &points, GL_STATIC_DRAW);
	glCheckError();
	//setup vao
	glGenVertexArrays(1, &vao);
	glCheckError();
	glBindVertexArray(vao);
	glCheckError();
	glEnableVertexAttribArray(0);
	glCheckError();
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glCheckError();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glCheckError();
	//setup shader
	shader = new Shader("../progettoGrafica/skymap.vert","../progettoGrafica/skymap.frag");
	shader->Use();
	
	//create cubemap
	create_cube_map(front, back, top, bottom, left, right);
	
	//setup vertex shader
	GLuint p_pos = glGetUniformLocation(shader->Program, "P");
	glCheckError();
	glUniformMatrix4fv(p_pos, 1 , GL_FALSE, glm::value_ptr(projectionMatrix));
	glCheckError();
	
}

void SkyMap::create_cube_map(const char* front, const char* back, const char* top, const char* bottom, const char* left, const char* right){
	// generate a cube-map texture to hold all the sides
	glActiveTexture(GL_TEXTURE0);
	glCheckError();
	glGenTextures(1, &tex_cube);
	glCheckError();

	// load each image and copy into a side of the cube-map texture
	load_cube_map_side(tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front);
	load_cube_map_side(tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back);
	load_cube_map_side(tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top);
	load_cube_map_side(tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom);
	load_cube_map_side(tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left);
	load_cube_map_side(tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right);
	// format cube map texture
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glCheckError();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glCheckError();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glCheckError();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glCheckError();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glCheckError();
}

bool SkyMap::load_cube_map_side(GLuint texture, GLenum side_target, const char* file_name){
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glCheckError();

	int x, y, n;
	int force_channels = 4;
	unsigned char*  image_data = stbi_load(
	file_name, &x, &y, &n, force_channels);
	if (!image_data) {
		fprintf(stderr, "ERROR: could not load %s\n", file_name);
		return false;
	}
	// non-power-of-2 dimensions check
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf(stderr,
			"WARNING: image %s is not power-of-2 dimensions\n",
			file_name);
	}

	// copy image data into 'target' side of cube map
	glTexImage2D(side_target, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	glCheckError();
	free(image_data);
	return true;
}

void SkyMap::Update(){
	shader->Use();
	glm::mat4 view = camera->GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(shader->Program, "V"), 1 , GL_FALSE, glm::value_ptr(view));
	glCheckError();
	
	glDepthMask(GL_FALSE);
	glCheckError();
	glUseProgram(shader->Program);
	glCheckError();
	glActiveTexture(GL_TEXTURE0);
	glCheckError();
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex_cube);
	glCheckError();
	glBindVertexArray(vao);
	glCheckError();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glCheckError();
	glDepthMask(GL_TRUE);
}

#endif // SKYMAP_H
