#include "Drawmodel.h"


MatrixStack model;
MatrixStack view;
MatrixStack projection;

Shader ourShader;


void OurShader_Create();
void Shader_init(glm::vec3 position, glm::vec3 front);
void Shader_use();
void ViewProjection_Create(glm::mat4 viewMatrix, float zoom, int width, int height);
void Model_Floor_Create(unsigned int VAO);
void Model_Create(unsigned int VAO, int numVoxelFace, float x, float z);
void Model_Cube_Create(unsigned int VAO);
void OurShader_Create()
{
    ourShader = Shader("shader/vShader.vs", "shader/fShader.fs");
}
void Shader_init(glm::vec3 position, glm::vec3 front, bool blinn){

		ourShader.use();
		ourShader.setVec3("viewPos", position);
        ourShader.setVec3("lightPos", position);
        ourShader.setInt("blinn", blinn);
	    // directional light
        // ourShader.setVec3("dirLight.direction", front);
        // ourShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        // ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        // ourShader.setVec3("dirLight.specular", 0.4f, 0.4f, 0.4f);	
}   

void ViewProjection_Create(glm::mat4 viewMatrix, float zoom, int width, int height){
    view.Save(viewMatrix);
    projection.Save(glm::perspective(glm::radians(zoom), (float)width / (float)height, 0.1f, 100.0f));
    ourShader.setMat4("view", view.Top());
    ourShader.setMat4("projection", projection.Top());
}
    
void Model_Create(unsigned int VAO, int numVoxelFace, float x, float z){
    model.Push();
    model.Save(glm::scale(model.Top(), glm::vec3( 0.1, 0.1, 0.1)));
    model.Save(glm::translate(model.Top(), glm::vec3(x,0,-z)));
    model.Save(glm::rotate(model.Top(),glm::radians(90.0f),glm::vec3(0.0f,0.0f,1.0f)));
    ourShader.setVec3("objectColor", 1.0f, 0.0f, 0.0f);
    ourShader.setMat4("model", model.Top());
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6 * numVoxelFace);
    
    model.Pop();
}
void Model_Floor_Create(unsigned int VAO){
    model.Push();
    model.Save(glm::scale(model.Top(), glm::vec3( 1000.0f, 1.0f, 1000.0f)));
    model.Save(glm::translate(model.Top(), glm::vec3(-0.5f, 0.0f, -0.5)));
    ourShader.setVec3("objectColor", 0.0f, 1.0f, 1.0f);
    ourShader.setMat4("model", model.Top());
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model.Pop();

}
void Model_Cube_Create(unsigned int VAO){
    // model.Push();
    // // model.Save(glm::translate(model.Top(),camera.Position));
    // ourShader.setMat4("model", model.Top());
    // ourShader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
    // glBindVertexArray(cube.VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 36);
    // model.Pop();
}