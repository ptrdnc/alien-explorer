#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <rg/Terrain.h>
#include <rg/Lights.h>
#include <rg/Sky.h>
#include <rg/City.h>
#include <rg/Ufo.h>
#include <rg/LightUfo.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void processInput(GLFWwindow *window);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


struct ProgramState {
    glm::vec3 clearColor = glm::vec3(0);
    bool ImGuiEnabled = false;
    Camera camera;
    bool CameraMouseMovementUpdateEnabled = true;
    glm::vec3 backpackPosition = glm::vec3(0.0f);
    float backpackScale = 0.01f;
    PointLight pointLight;
    ProgramState()
            : camera(glm::vec3(0.0f, 0.0f, 3.0f)) {}

    void SaveToFile(std::string filename);

    void LoadFromFile(std::string filename);
};

void ProgramState::SaveToFile(std::string filename) {
    std::ofstream out(filename);
    out << clearColor.r << '\n'
        << clearColor.g << '\n'
        << clearColor.b << '\n'
        << ImGuiEnabled << '\n'
        << camera.Position.x << '\n'
        << camera.Position.y << '\n'
        << camera.Position.z << '\n'
        << camera.Front.x << '\n'
        << camera.Front.y << '\n'
        << camera.Front.z << '\n';
}

void ProgramState::LoadFromFile(std::string filename) {
    std::ifstream in(filename);
    if (in) {
        in >> clearColor.r
           >> clearColor.g
           >> clearColor.b
           >> ImGuiEnabled
           >> camera.Position.x
           >> camera.Position.y
           >> camera.Position.z
           >> camera.Front.x
           >> camera.Front.y
           >> camera.Front.z;
    }
}

ProgramState *programState;

void DrawImGui(ProgramState *programState);

DayTime dayTime = DAY;
bool spotLightOn = true;
int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);


    programState = new ProgramState;
    programState->LoadFromFile("resources/program_state.txt");
    if (programState->ImGuiEnabled) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    /*
    // Init Imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;



    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    */
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    Terrain terrain(512, 512);
    Sky sky;
    City city;
    Ufo ufo;
    vector<LightUfo> lightUfos = {
            LightUfo(glm::vec3(1.0f, 0.0f, 0.0f), UFO_RADIUS, glm::radians(0.0f)),
            LightUfo(glm::vec3(0.0f, 1.0f, 0.0f), UFO_RADIUS, glm::radians(120.0f)),
            LightUfo(glm::vec3(0.0f, 0.0f, 1.0f), UFO_RADIUS, glm::radians(240.0f))
    };

    PointLight pointLights[NUM_LIGHT_UFOS];
    for (int i = 0; i < NUM_LIGHT_UFOS; i++){
        pointLights[i] = {
                .position = lightUfos[i].position,
                .ambient = 0.2f * lightUfos[i].color,
                .diffuse = lightUfos[i].color,
                .specular = lightUfos[i].color,
                .constant = 1.0f,
                .linear = 0.09f,
                .quadratic = 0.032f
        };

    }

    DirLight day, dusk, night;
    day.ambient = glm::vec3(0.3f, 0.3f, 0.3f);
    day.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    day.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    day.direction = glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f));

    dusk.diffuse = glm::vec3(1, 0.447, 0.2);
    dusk.ambient = glm::vec3(0.2,0.2,0.2);
    dusk.specular = glm::vec3(0.5,0.5,0.5);
    dusk.direction = glm::normalize(glm::vec3(-1,1,-1));

    night.diffuse = glm::vec3(0.1,0.1,0.1);
    night.ambient = glm::vec3(0.1,0.1,0.1);
    night.specular = glm::vec3(0.1,0.1,0.1);
    night.direction = glm::normalize(glm::vec3(-1,1,-1));

    std::vector<DirLight> dirLights = { day, dusk, night };

    SpotLight spotLightFull = {
            .position = ufo.ufoPosition,
            .direction = glm::vec3(0.0, -1.0, 0.0),
            .cutOff = glm::cos(glm::radians(10.0f)),
            .outerCutOff = glm::cos(glm::radians(12.0f)),
            .constant = 1.0f,
            .linear = 0.09f,
            .quadratic = 0.032f,
            .ambient = glm::vec3(0.2f, 0.2f, 0.2f),
            .diffuse = glm::vec3(1.0f, 1.0f, 1.0f),
            .specular = glm::vec3(1.0f, 1.0f, 1.0f)
    };
    //0.014	0.0007
    SpotLight spotLightDimmed = {
            .position = ufo.ufoPosition,
            .direction = glm::vec3(0.0, -1.0, 0.0),
            .cutOff = glm::cos(glm::radians(12.5f)),
            .outerCutOff = glm::cos(glm::radians(15.0f)),
            .constant = 1.0f,
            .linear = 0.9,
            .quadratic = 8,
            .ambient = glm::vec3(0.0f, 0.0f, 0.0f),
            .diffuse = glm::vec3(0.0f, 0.0f, 0.0f),
            .specular = glm::vec3(0.0f, 0.0f, 0.0f)
    };
    //1.0	0.7	1.8

    SpotLight spotLight = spotLightFull;
    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
    //

    // render loop
    // -----------

    float r = 0.1;

    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic


        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(programState->clearColor.r, programState->clearColor.g, programState->clearColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(programState->camera.Zoom),(float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = programState->camera.GetViewMatrix();

        if (spotLightOn)
            spotLight = spotLightFull;
        else
            spotLight = spotLightDimmed;

        spotLight.position = ufo.ufoPosition;
        spotLight.direction = glm::vec3(r * cos(currentFrame), -1.0, r * sin(currentFrame));

        city.setup(programState->camera.Position, projection, view, pointLights, dirLights[dayTime], spotLight, currentFrame);
        city.draw();

        ufo.setup(programState->camera.Position, projection, view, pointLights, dirLights[dayTime], spotLight, currentFrame);
        ufo.draw();

        for (int i = 0; i < NUM_LIGHT_UFOS; i++) {
            lightUfos[i].setup(programState->camera.Position, projection, view, pointLights, dirLights[dayTime], spotLight, currentFrame);
            lightUfos[i].draw();

            pointLights[i].position = lightUfos[i].position;
        }


        terrain.setup(projection, view, programState->camera.Position, pointLights, dirLights[dayTime], spotLight);
        terrain.draw();

        sky.setup(dayTime);
        sky.draw(projection, view);


        /*if (programState->ImGuiEnabled)
            DrawImGui(programState);
        */


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    programState->SaveToFile("resources/program_state.txt");
    delete programState;
    /*
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    */
     // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        dayTime = DAY;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        dayTime = DUSK;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        dayTime = NIGHT;

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        spotLightOn = true;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        spotLightOn = false;


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        programState->camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        programState->camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        programState->camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        programState->camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        programState->camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        programState->camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
        programState->camera.ProcessKeyboard(DOWN, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    if (programState->CameraMouseMovementUpdateEnabled)
        programState->camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    programState->camera.ProcessMouseScroll(yoffset);
}

void DrawImGui(ProgramState *programState) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    {
        static float f = 0.0f;
        ImGui::Begin("Hello window");
        ImGui::Text("Hello text");
        ImGui::SliderFloat("Float slider", &f, 0.0, 1.0);
        ImGui::ColorEdit3("Background color", (float *) &programState->clearColor);
        ImGui::DragFloat3("Backpack position", (float*)&programState->backpackPosition);
        ImGui::DragFloat("Backpack scale", &programState->backpackScale, 0.05, 0.1, 4.0);

        ImGui::DragFloat("pointLight.constant", &programState->pointLight.constant, 0.05, 0.0, 1.0);
        ImGui::DragFloat("pointLight.linear", &programState->pointLight.linear, 0.05, 0.0, 1.0);
        ImGui::DragFloat("pointLight.quadratic", &programState->pointLight.quadratic, 0.05, 0.0, 1.0);
        ImGui::End();
    }

    {
        ImGui::Begin("Camera info");
        const Camera& c = programState->camera;
        ImGui::Text("Camera position: (%f, %f, %f)", c.Position.x, c.Position.y, c.Position.z);
        ImGui::Text("(Yaw, Pitch): (%f, %f)", c.Yaw, c.Pitch);
        ImGui::Text("Camera front: (%f, %f, %f)", c.Front.x, c.Front.y, c.Front.z);
        ImGui::Checkbox("Camera mouse update", &programState->CameraMouseMovementUpdateEnabled);
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        programState->ImGuiEnabled = !programState->ImGuiEnabled;
        if (programState->ImGuiEnabled) {
            programState->CameraMouseMovementUpdateEnabled = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
}
