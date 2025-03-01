//
// Created by root on 29.4.21..
//

#ifndef PROJECT_BASE_TERRAIN_H
#define PROJECT_BASE_TERRAIN_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <learnopengl/shader.h>
#include <stb_image.h>
#include <rg/Error.h>

#include <rg/Lights.h>
#include <rg/LightUfo.h>

class Terrain {
public:

    unsigned int diffuseMap;
    unsigned int specularMap;
    unsigned int normalMap;
    unsigned int dispMap;
    unsigned int shininess = 2;
    float expand = 1.0f;
    float tex = 1.0f;
    Shader terrainShader;
    float heightScale = 0.03f;
    glm::mat4 model = glm::mat4(1.0f);


    unsigned int quadVAO = 0;
    unsigned int quadVBO;

    Terrain(int expand = 1.0, int tex = 1.0)
            : terrainShader("resources/shaders/terrain.vs", "resources/shaders/terrain.fs") {

        this->expand = expand;
        this->tex = tex;

        glm::vec3 axis = glm::vec3(1, 0, 0);
        float angle = glm::radians(90.0);

        model = glm::rotate(model, angle, axis);
        glm::vec3 tinyPush = glm::vec3(0.0, 0.0, 0.1);
        model = glm::translate(model, tinyPush);

//        diffuseMap = loadTexture("resources/textures/grass/Green-Grass-Ground-Texture-DIFFUSE.jpg");
//        specularMap = loadTexture("resources/textures/grass/Green-Grass-Ground-Texture-SPECULAR.jpg");
//        normalMap = loadTexture("resources/textures/grass/Green-Grass-Ground-Texture-NORMAL.jpg");
//        dispMap = loadTexture("resources/textures/grass/Green-Grass-Ground-Texture-DISP.jpg");


        diffuseMap = loadTexture("resources/textures/sand/Seamless_cracked_sand_ground_texture.jpg");
        specularMap = loadTexture("resources/textures/sand/Seamless_cracked_sand_ground_texture_SPECULAR.jpg");
        normalMap = loadTexture("resources/textures/sand/Seamless_cracked_sand_ground_texture_NORMAL.jpg");
        dispMap = loadTexture("resources/textures/sand/Seamless_cracked_sand_ground_texture_DISP.jpg");


        terrainShader.use();
        terrainShader.setInt("material.texture_diffuse1", GL_TEXTURE0);
        terrainShader.setInt("material.texture_specular1", GL_TEXTURE1);
        terrainShader.setInt("material.texture_normal1", GL_TEXTURE2);
        terrainShader.setInt("material.texture_disp1", GL_TEXTURE3);
        renderQuad();


    }
    void setup(glm::mat4& projection, glm::mat4& view, glm::vec3& viewPosition, PointLight* pointLights, DirLight& dirLight, SpotLight& spotLight)
    {
        terrainShader.use();
        terrainShader.setMat4("projection", projection);
        terrainShader.setMat4("view", view);
        terrainShader.setMat4("model", model);
        terrainShader.setVec3("viewPos", viewPosition);

        terrainShader.setVec3("dirLight.direction", dirLight.direction);
        terrainShader.setVec3("dirLight.diffuse", dirLight.diffuse);
        terrainShader.setVec3("dirLight.ambient", dirLight.ambient);
        terrainShader.setVec3("dirLight.specular", dirLight.specular);
        terrainShader.setFloat("material.shininess", 0.0f);
        for (int i = 0; i < NUM_LIGHT_UFOS; i++) {
            terrainShader.setVec3("pointLights[" + to_string(i) + "].position", pointLights[i].position);
            terrainShader.setVec3("pointLights[" + to_string(i) + "].ambient", pointLights[i].ambient);
            terrainShader.setVec3("pointLights[" + to_string(i) + "].diffuse", pointLights[i].diffuse);
            terrainShader.setVec3("pointLights[" + to_string(i) + "].specular", pointLights[i].specular);
            terrainShader.setFloat("pointLights[" + to_string(i) + "].constant", pointLights[i].constant);
            terrainShader.setFloat("pointLights[" + to_string(i) + "].linear", pointLights[i].linear);
            terrainShader.setFloat("pointLights[" + to_string(i) + "].quadratic", pointLights[i].quadratic);

        }

        terrainShader.setVec3("spotLight.position", spotLight.position);
        terrainShader.setVec3("spotLight.direction", spotLight.direction);
        terrainShader.setVec3("spotLight.ambient", spotLight.ambient);
        terrainShader.setVec3("spotLight.diffuse", spotLight.diffuse);
        terrainShader.setVec3("spotLight.specular", spotLight.specular);
        terrainShader.setFloat("spotLight.constant", spotLight.constant);
        terrainShader.setFloat("spotLight.linear", spotLight.linear);
        terrainShader.setFloat("spotLight.quadratic", spotLight.quadratic);
        terrainShader.setFloat("spotLight.cutOff", spotLight.cutOff);
        terrainShader.setFloat("spotLight.outerCutOff", spotLight.outerCutOff);

        terrainShader.setFloat("heightScale", heightScale);
        terrainShader.setFloat("tex", tex);
    }
    void draw()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, normalMap);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
    unsigned int loadTexture(const char *filename) {

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(filename, &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1) {
                format = GL_RED;
            } else if (nrComponents == 3) {
                format = GL_RGB;
            } else if (nrComponents == 4) {
                format = GL_RGBA;
            }
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        } else {
            ASSERT(false, "Failed to load texture image");
        }
        stbi_image_free(data);
        return textureID;
    }
    void renderQuad() {
        if (quadVAO == 0) {
            // positions
            glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
            glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
            glm::vec3 pos3(1.0f, -1.0f, 0.0f);
            glm::vec3 pos4(1.0f, 1.0f, 0.0f);
            // texture coordinates

            glm::vec2 uv1(0.0f, 1.0f);
            glm::vec2 uv2(0.0f, 0.0f);
            glm::vec2 uv3(1.0f, 0.0f);
            glm::vec2 uv4(1.0f, 1.0f);
            // normal vector

            glm::vec3 nm(0.0f, 0.0f, 1.0f);

            // calculate tangent/bitangent vectors of both triangles
            glm::vec3 tangent1, bitangent1;
            glm::vec3 tangent2, bitangent2;
            // triangle 1
            // ----------
            glm::vec3 edge1 = pos2 - pos1;
            glm::vec3 edge2 = pos3 - pos1;
            glm::vec2 deltaUV1 = uv2 - uv1;
            glm::vec2 deltaUV2 = uv3 - uv1;

            float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

            bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
            bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
            bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

            // triangle 2
            // ----------
            edge1 = pos3 - pos1;
            edge2 = pos4 - pos1;
            deltaUV1 = uv3 - uv1;
            deltaUV2 = uv4 - uv1;

            f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


            bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
            bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
            bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


            float quadVertices[] = {
                    // positions            // normal         // texcoords  // tangent                          // bitangent
                    pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z,
                    bitangent1.x, bitangent1.y, bitangent1.z,
                    pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z,
                    bitangent1.x, bitangent1.y, bitangent1.z,
                    pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z,
                    bitangent1.x, bitangent1.y, bitangent1.z,

                    pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z,
                    bitangent2.x, bitangent2.y, bitangent2.z,
                    pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z,
                    bitangent2.x, bitangent2.y, bitangent2.z,
                    pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z,
                    bitangent2.x, bitangent2.y, bitangent2.z
            };

            int j;




            //AAAAAAAAAAAAA

            for (int i = 0; i < 84; i += 14) {
                for (int j = 0; j < 3; j++)
                    quadVertices[i+j] *= expand;
                quadVertices[i+6] *= tex;
                quadVertices[i+7] *= tex;
            }




            // configure plane VAO
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *) 0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *) (3 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *) (6 * sizeof(float)));
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *) (8 * sizeof(float)));
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *) (11 * sizeof(float)));
        }
    }


};





#endif //PROJECT_BASE_TERRAIN_H
