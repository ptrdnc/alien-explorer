//
// Created by root on 8.5.21..
//

#ifndef PROJECT_BASE_CITY_H
#define PROJECT_BASE_CITY_H

#include <learnopengl/model.h>
#include <rg/Lights.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class City {
public:
Model cityModel;
Shader cityShader;
float cityScale = 0.01f;
glm::vec3 cityPosition = glm::vec3(0.0f);
glm::mat4 model = glm::mat4(1.0f);
City()
    : cityModel("resources/objects/futuristic/wild town/wild town.obj"),
      cityShader("resources/shaders/2.model_lighting.vs", "resources/shaders/2.model_lighting.fs")
{
//    cityModel.SetShaderTextureNamePrefix("material.");
//    cityModel.SetShaderTextureNamePrefix("material.");

}
void setup(glm::vec3& viewPosition, glm::mat4& projection, glm::mat4& view,
            PointLight& pointLight, DirLight& dirLight, float currentFrame)
{

    cityShader.use();
    cityShader.setVec3("dirLight.ambient", dirLight.ambient);
    cityShader.setVec3("dirLight.diffuse", dirLight.diffuse);
    cityShader.setVec3("dirLight.specular", dirLight.specular);
    cityShader.setVec3("dirLight.direction", dirLight.direction);


    pointLight.position = glm::vec3(4.0 * cos(currentFrame), 4.0f, 4.0 * sin(currentFrame));
    cityShader.setVec3("pointLight.position", pointLight.position);
    cityShader.setVec3("pointLight.ambient", pointLight.ambient);
    cityShader.setVec3("pointLight.diffuse", pointLight.diffuse);
    cityShader.setVec3("pointLight.specular", pointLight.specular);
    cityShader.setFloat("pointLight.constant", pointLight.constant);
    cityShader.setFloat("pointLight.linear", pointLight.linear);
    cityShader.setFloat("pointLight.quadratic", pointLight.quadratic);
    cityShader.setVec3("viewPosition", viewPosition);
    cityShader.setFloat("material.shininess", 32.0f);

    cityShader.setMat4("projection", projection);
    cityShader.setMat4("view", view);

    model = glm::mat4(1.0f);
    model = glm::translate(model, cityPosition);
    model = glm::scale(model, glm::vec3(cityScale));

    cityShader.setMat4("model", model);

}
void draw()
{
    cityModel.Draw(cityShader);
}

};

#endif //PROJECT_BASE_CITY_H