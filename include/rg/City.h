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
#include <rg/LightUfo.h>

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
            PointLight* pointLights, DirLight& dirLight, SpotLight& spotLight, float currentFrame)
{

    cityShader.use();
    cityShader.setVec3("dirLight.ambient", dirLight.ambient);
    cityShader.setVec3("dirLight.diffuse", dirLight.diffuse);
    cityShader.setVec3("dirLight.specular", dirLight.specular);
    cityShader.setVec3("dirLight.direction", dirLight.direction);


    for (int i = 0; i < NUM_LIGHT_UFOS; i++) {
        cityShader.setVec3("pointLights[" + to_string(i) + "].position", pointLights[i].position);
        cityShader.setVec3("pointLights[" + to_string(i) + "].ambient", pointLights[i].ambient);
        cityShader.setVec3("pointLights[" + to_string(i) + "].diffuse", pointLights[i].diffuse);
        cityShader.setVec3("pointLights[" + to_string(i) + "].specular", pointLights[i].specular);
        cityShader.setFloat("pointLights[" + to_string(i) + "].constant", pointLights[i].constant);
        cityShader.setFloat("pointLights[" + to_string(i) + "].linear", pointLights[i].linear);
        cityShader.setFloat("pointLights[" + to_string(i) + "].quadratic", pointLights[i].quadratic);
        cityShader.setVec3("viewPosition", viewPosition);
        cityShader.setFloat("material.shininess", 32.0f);
    }

    cityShader.setVec3("spotLight.position", spotLight.position);
    cityShader.setVec3("spotLight.direction", spotLight.direction);
    cityShader.setVec3("spotLight.ambient", spotLight.ambient);
    cityShader.setVec3("spotLight.diffuse", spotLight.diffuse);
    cityShader.setVec3("spotLight.specular", spotLight.specular);
    cityShader.setFloat("spotLight.constant", spotLight.constant);
    cityShader.setFloat("spotLight.linear", spotLight.linear);
    cityShader.setFloat("spotLight.quadratic", spotLight.quadratic);
    cityShader.setFloat("spotLight.cutOff", spotLight.cutOff);
    cityShader.setFloat("spotLight.outerCutOff", spotLight.outerCutOff);



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