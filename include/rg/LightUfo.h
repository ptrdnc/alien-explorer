//
// Created by root on 9.5.21..
//

#ifndef PROJECT_BASE_LIGHTUFO_H
#define PROJECT_BASE_LIGHTUFO_H

#include <learnopengl/model.h>
#include <rg/Lights.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class LightUfo {
public:

    Model ufoModel;
    Shader ufoShader;
//    float ufoScale = 0.005f;
    float ufoScale = 0.02f;

    float radius = 32.0f;
    float height = 10.0f;
    float initialAngle = 0.0f;
    glm::vec3 color;

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
               PointLight& pointLight, DirLight& dirLight, float currentFrame)
    {

        ufoShader.use();

        ufoShader.setVec3("color", color);

        ufoShader.setVec3("dirLight.ambient", dirLight.ambient);
        ufoShader.setVec3("dirLight.diffuse", dirLight.diffuse);
        ufoShader.setVec3("dirLight.specular", dirLight.specular);
        ufoShader.setVec3("dirLight.direction", dirLight.direction);

        currentFrame = 0;
        pointLight.position = glm::vec3(radius * cos(currentFrame), height, radius * sin(currentFrame));
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
        glm::vec3 ufoPosition = pointLight.position;
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
