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
public:
    Model ufoModel;
    Shader ufoShader;
    float ufoScale = 0.005f;
    glm::vec3 ufoOffset = glm::vec3(0.0f, -0.25f, 0.0f);
    glm::mat4 model = glm::mat4(1.0f);
    Ufo()
            : ufoModel("resources/objects/ufo/scene.gltf"),
              ufoShader("resources/shaders/2.model_lighting.vs", "resources/shaders/2.model_lighting.fs")
    {

        ufoModel.SetShaderTextureNamePrefix("material.");
        ufoModel.SetShaderTextureNamePrefix("material.");
    }
    void setup(glm::vec3& viewPosition, glm::mat4& projection, glm::mat4& view,
               PointLight& pointLight, DirLight& dirLight, float currentFrame)
    {

        ufoShader.use();
        ufoShader.setVec3("dirLight.ambient", dirLight.ambient);
        ufoShader.setVec3("dirLight.diffuse", dirLight.diffuse);
        ufoShader.setVec3("dirLight.specular", dirLight.specular);
        ufoShader.setVec3("dirLight.direction", dirLight.direction);


        pointLight.position = glm::vec3(4.0 * cos(currentFrame), 4.0f, 4.0 * sin(currentFrame));
        ufoShader.setVec3("pointLight.position", pointLight.position);
        ufoShader.setVec3("pointLight.ambient", pointLight.ambient);
        ufoShader.setVec3("pointLight.diffuse", pointLight.diffuse);
        ufoShader.setVec3("pointLight.specular", pointLight.specular);
        ufoShader.setFloat("pointLight.constant", pointLight.constant);
        ufoShader.setFloat("pointLight.linear", pointLight.linear);
        ufoShader.setFloat("pointLight.quadratic", pointLight.quadratic);
        ufoShader.setVec3("viewPosition", viewPosition);
        ufoShader.setFloat("material.shininess", 32.0f);

        ufoShader.setMat4("projection", projection);
        ufoShader.setMat4("view", view);

        model = glm::mat4(1.0f);
        glm::vec3 ufoPosition = viewPosition + ufoOffset;
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
