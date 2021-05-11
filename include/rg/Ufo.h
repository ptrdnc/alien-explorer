//
// Created by root on 8.5.21..
//

#ifndef PROJECT_BASE_UFO_H
#define PROJECT_BASE_UFO_H

#include <learnopengl/model.h>
#include <rg/Lights.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Ufo {
public:
    Model ufoModel;
    Shader ufoShader;
    float ufoScale = 0.005f;
    glm::vec3 ufoOffset = glm::vec3(0.0f, -0.25f, 0.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 ufoPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    Ufo()
            : ufoModel("resources/objects/ufo/scene.gltf"),
              ufoShader("resources/shaders/2.model_lighting.vs", "resources/shaders/2.model_lighting.fs")
    {

        ufoModel.SetShaderTextureNamePrefix("material.");
        ufoModel.SetShaderTextureNamePrefix("material.");
    }
    void setup(glm::vec3& viewPosition, glm::mat4& projection, glm::mat4& view,
               PointLight* pointLights, DirLight& dirLight, SpotLight& spotLight)
    {

        ufoShader.use();
        ufoShader.setVec3("dirLight.ambient", dirLight.ambient);
        ufoShader.setVec3("dirLight.diffuse", dirLight.diffuse);
        ufoShader.setVec3("dirLight.specular", dirLight.specular);
        ufoShader.setVec3("dirLight.direction", dirLight.direction);



        for (int i = 0; i < NUM_LIGHT_UFOS; i++) {
            ufoShader.setVec3("pointLights[" + to_string(i) + "].position", pointLights[i].position);
            ufoShader.setVec3("pointLights[" + to_string(i) + "].ambient", pointLights[i].ambient);
            ufoShader.setVec3("pointLights[" + to_string(i) + "].diffuse", pointLights[i].diffuse);
            ufoShader.setVec3("pointLights[" + to_string(i) + "].specular", pointLights[i].specular);
            ufoShader.setFloat("pointLights[" + to_string(i) + "].constant", pointLights[i].constant);
            ufoShader.setFloat("pointLights[" + to_string(i) + "].linear", pointLights[i].linear);
            ufoShader.setFloat("pointLights[" + to_string(i) + "].quadratic", pointLights[i].quadratic);
            ufoShader.setVec3("viewPosition", viewPosition);
            ufoShader.setFloat("material.shininess", 32.0f);
        }

        ufoShader.setVec3("spotLight.position", spotLight.position);
        ufoShader.setVec3("spotLight.direction", spotLight.direction);
        ufoShader.setVec3("spotLight.ambient", spotLight.ambient);
        ufoShader.setVec3("spotLight.diffuse", spotLight.diffuse);
        ufoShader.setVec3("spotLight.specular", spotLight.specular);
        ufoShader.setFloat("spotLight.constant", spotLight.constant);
        ufoShader.setFloat("spotLight.linear", spotLight.linear);
        ufoShader.setFloat("spotLight.quadratic", spotLight.quadratic);
        ufoShader.setFloat("spotLight.cutOff", spotLight.cutOff);
        ufoShader.setFloat("spotLight.outerCutOff", spotLight.outerCutOff);



        ufoShader.setMat4("projection", projection);
        ufoShader.setMat4("view", view);

        model = glm::mat4(1.0f);
        ufoPosition = viewPosition + ufoOffset;
        model = glm::translate(model, ufoPosition);
        glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f);
        model = glm::rotate(model, (float)glfwGetTime(), axis);
        model = glm::scale(model, glm::vec3(ufoScale));

        ufoShader.setMat4("model", model);

    }
    void draw()
    {
        ufoModel.Draw(ufoShader);
    }





};


#endif //PROJECT_BASE_UFO_H
