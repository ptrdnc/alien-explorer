//
// Created by root on 9.5.21..
//

#ifndef PROJECT_BASE_LIGHTUFO_H
#define PROJECT_BASE_LIGHTUFO_H

#include <learnopengl/model.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <rg/Lights.h>

#define NUM_LIGHT_UFOS 3
#define UFO_RADIUS (15.0f)


class LightUfo {
public:

    Model ufoModel;
    Shader ufoShader;
//    float ufoScale = 0.005f;
    float ufoScale = 0.02f;

    float radius = 32.0f;
    float height = 6.60f;
    float initialAngle = 0.0f;
    glm::vec3 color;
    glm::vec3 position;

    glm::mat4 model = glm::mat4(1.0f);
    LightUfo(glm::vec3 color, float radius, float initialAngle)
            : ufoModel("resources/objects/ufo/scene.gltf"),
              ufoShader("resources/shaders/lightUfo.vs", "resources/shaders/lightUfo.fs")
    {
        this->color = color;
        this->radius = radius;
        this->initialAngle = initialAngle;
    }
    void setup(glm::vec3& viewPosition, glm::mat4& projection, glm::mat4& view,
               PointLight* pointLights, DirLight& dirLight, SpotLight& spotLight, float currentFrame)
    {

        ufoShader.use();

        ufoShader.setVec3("color", color);

        ufoShader.setVec3("dirLight.ambient", dirLight.ambient);
        ufoShader.setVec3("dirLight.diffuse", dirLight.diffuse);
        ufoShader.setVec3("dirLight.specular", dirLight.specular);
        ufoShader.setVec3("dirLight.direction", dirLight.direction);

        currentFrame += initialAngle;
        position = glm::vec3(radius * cos(currentFrame), height, radius * sin(currentFrame));


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
        glm::vec3 ufoPosition = position;
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


#endif //PROJECT_BASE_LIGHTUFO_H
