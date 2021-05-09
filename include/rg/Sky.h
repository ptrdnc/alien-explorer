//
// Created by root on 5/7/21.
//
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

#ifndef PROJECT_BASE_CUBEMAPS_H
#define PROJECT_BASE_CUBEMAPS_H

#endif //PROJECT_BASE_CUBEMAPS_H

class Sky{
public:



    std::vector<std::string> dayFaces={
            "resources/textures/sky/day/right.bmp",
            "resources/textures/sky/day/left.bmp",
            "resources/textures/sky/day/bottom.bmp",
            "resources/textures/sky/day/top.bmp",
            "resources/textures/sky/day/front.bmp",
            "resources/textures/sky/day/back.bmp"
    };
//    std::vector<std::string> dayFaces={
//            "resources/textures/sky/day/right.jpg",
//            "resources/textures/sky/day/left.jpg",
//            "resources/textures/sky/day/bottom.jpg",
//            "resources/textures/sky/day/top.jpg",
//            "resources/textures/sky/day/front.jpg",
//            "resources/textures/sky/day/back.jpg"
//    };

//    std::vector<std::string> nightFaces={
//            "resources/textures/sky/night/right.png",
//            "resources/textures/sky/night/left.png",
//            "resources/textures/sky/night/bottom.png",
//            "resources/textures/sky/night/top.png",
//            "resources/textures/sky/night/front.png",
//            "resources/textures/sky/night/back.png"
//    };
    std::vector<std::string> duskFaces={
            "resources/textures/sky/dusk/front.png",
            "resources/textures/sky/dusk/back.png",
            "resources/textures/sky/dusk/bottom.png",
            "resources/textures/sky/dusk/top.png",
            "resources/textures/sky/dusk/right.png",
            "resources/textures/sky/dusk/left.png"
    };
    std::vector<std::string> nightFaces={
            "resources/textures/sky/night/front.png",
            "resources/textures/sky/night/back.png",
            "resources/textures/sky/night/bottom.png",
            "resources/textures/sky/night/top.png",
            "resources/textures/sky/night/right.png",
            "resources/textures/sky/night/left.png"
    };
    Shader skyShader;
    unsigned int skyboxVAO, skyboxVBO;
    std::vector<std::string> &activeFaces = dayFaces;
    glm::mat4 model = glm::mat4(1.0f);
    unsigned int cubemapTexture;
    float skyboxVertices[108] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };
    Sky()
            : skyShader("resources/shaders/skybox.vs", "resources/shaders/skybox.fs") {

//        std::reverse(dayFaces.begin(), dayFaces.end());
//        std::reverse(nightFaces.begin(), nightFaces.end());


        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


        cubemapTexture = loadCubemap(activeFaces);

        skyShader.use();
        skyShader.setInt("skybox", GL_TEXTURE0);
    };

    void setup()
    {

        skyShader.use();
        skyShader.setInt("skybox", GL_TEXTURE0);

    }

    void refresh(int dayTime){
        if (dayTime == 1)
            activeFaces = nightFaces;
        else if (dayTime == 2)
            activeFaces = duskFaces;
        else if (dayTime == 3)
            activeFaces = dayFaces;

        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


        cubemapTexture = loadCubemap(activeFaces);

        skyShader.use();
        skyShader.setInt("skybox", GL_TEXTURE0);

    }

    void draw(glm::mat4 projection, glm::mat4 view){
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        glDepthMask(GL_FALSE);
        skyShader.use();
        skyShader.setMat4("projection", projection);


        glm::vec3 axis = glm::vec3(1, 0, 0);
        float angle = glm::radians(180.0);

        glm::mat4 cubeView = glm::rotate(view, angle, axis);
        cubeView = glm::mat4(glm::mat3(cubeView));

        skyShader.setMat4("view", cubeView);
        skyShader.setMat4("model", model);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS); // set depth function back to default
    }

    unsigned int loadCubemap(vector<std::string>& faces)
    {

        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }
};

