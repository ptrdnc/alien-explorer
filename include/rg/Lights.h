//
// Created by root on 1.5.21..
//

#ifndef PROJECT_BASE_LIGHTS_H
#define PROJECT_BASE_LIGHTS_H

struct PointLight {
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirLight {

    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

};



#endif //PROJECT_BASE_LIGHTS_H
