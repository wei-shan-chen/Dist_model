#ifndef DRAWMODEL_H
#define DRAWMODEL_H
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "matrixStack.h"
#include "shader.h"
// #include "Item.h"

void OurShader_Create();
void Shader_init(glm::vec3 position, glm::vec3 front, bool blinn);

void ViewProjection_Create(glm::mat4 viewMatrix, float zoom, int width, int height);
void Model_Floor_Create(unsigned int VAO);
void Model_Cube_Create(unsigned int VAO);
void Model_Create(unsigned int VAO, int numVoxelFace, float x, float z);


extern MatrixStack model;
extern MatrixStack view;
extern MatrixStack projection;
extern Shader ourShader;

#endif