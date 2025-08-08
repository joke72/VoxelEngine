#include <iostream>
#include <random>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "ChunkManager.h"
#include "Constants.h"
#include "Shader.h"
#include "ShaderStorageBuffer.h"
#include "Texture.h"
#include "VertexArray.h"


int screenWidth = 1920, screenHeight = 1080;

#define PI 3.14159265359

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = screenWidth / 2, lastY = screenHeight / 2;
bool firstMouse = true;


Shader shader;
Shader shader2;
Shader gShader;
Shader ssaoShader;
Shader lightingShader;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    //screenWidth = width;
    //screenHeight = height;
    //gShader.use();
    //gShader.setVec2("uScreenSize", glm::vec2(screenWidth, screenHeight));
    //lightingShader.use();
    //lightingShader.setVec2("uScreenSize", glm::vec2(screenWidth, screenHeight));
}
bool paused = false;
bool PBR = false;
bool hasReleased = false;
int timePass = 0;
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (hasReleased)
        {
            PBR = !PBR;
            std::cout << "Using PBR: " << PBR << std::endl;
            //paused = !paused;
            hasReleased = false;
        }

    }
    else
    {
        hasReleased = true;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        timePass = 0;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        timePass = 1;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        timePass = 2;

    bool fast = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
    bool slow = (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Camera::MAIN.processKeyboard(FORWARD, deltaTime, fast, slow);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Camera::MAIN.processKeyboard(BACKWARD, deltaTime, fast, slow);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Camera::MAIN.processKeyboard(RIGHT, deltaTime, fast, slow);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Camera::MAIN.processKeyboard(LEFT, deltaTime, fast, slow);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        Camera::MAIN.processKeyboard(UP, deltaTime, fast, slow);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        Camera::MAIN.processKeyboard(DOWN, deltaTime, fast, slow);
}
GLFWwindow* window0;

void mouseCallback(GLFWwindow* window, double x, double y)
{
    if (firstMouse)
    {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    float xOffset = x - lastX;
    float yOffset = lastY - y;
    lastX = x;
    lastY = y;

    Camera::MAIN.processMouseMovement(xOffset, yOffset, deltaTime);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    Camera::MAIN.Zoom(yOffset, deltaTime);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    Camera::MAIN.right = button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS;
    Camera::MAIN.middle = button == GLFW_MOUSE_BUTTON_3 && action == GLFW_PRESS;

    if (Camera::MAIN.right || Camera::MAIN.middle)
        firstMouse = true;

    glfwSetInputMode(window0, GLFW_CURSOR, (Camera::MAIN.right || Camera::MAIN.middle) ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}


int initGLFW(GLFWwindow** window)
{

    //OPENGL VERSION 4.6
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Multi sampling for antialiasing
    //glfwWindowHint(GLFW_SAMPLES, 4);

    *window = glfwCreateWindow(screenWidth, screenHeight, "Voxel Engine", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(*window);

    //Disable vsync
    glfwSwapInterval(0);

    //Center window on 4k screen
    glfwSetWindowPos(*window, (3840 - screenWidth) / 2, (2160 - screenHeight) / 2);

    //Return -1 if initialization fails
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, screenWidth, screenHeight);

    //Set callbacks
    glfwSetFramebufferSizeCallback(*window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(*window, mouseCallback);
    glfwSetScrollCallback(*window, scrollCallback);
    glfwSetMouseButtonCallback(*window, mouseButtonCallback);

    //Grab cursor
}


int main()
{
    //Initialize window

    if (initGLFW(&window0) == -1)
    {
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_ALWAYS);
    glEnable(GL_FRAMEBUFFER_SRGB);
    //glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_FRAMEBUFFER_SRGB);

    std::cout << glGetString(GL_VERSION) << std::endl;
    
    float quad[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
                      
        -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
    };

    GLuint* chunk = new GLuint[ARRAY_SIZE];

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_HEIGHT; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                int r = rand();
                if (y < x / 8 + z / 8)
                    chunk[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_HEIGHT + CHUNKS] = r < 15000 ? 1 : 2;
                else
                    chunk[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_HEIGHT + CHUNKS] = 0;
            }
        }
    }

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_HEIGHT; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                int r = rand();
                if (y < x / 8 + z / 8) // 4 * sin(x * 0.2) + 4 * cos(z * 0.2) + 10
                    chunk[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_HEIGHT + CHUNKS + CHUNK_VOLUME*1] = r < 15000 ? 1 : 1;
                else
                    chunk[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_HEIGHT + CHUNKS + CHUNK_VOLUME*1] = 0;
            }
        }
    }
    /*
    for (int i = 0; i < CHUNK_VOLUME * CHUNKS + CHUNKS; i++)
    {
        chunk[i] = 1;
    }*/
    //std::cout << (CHUNK_VOLUME * CHUNKS + CHUNKS) << std::endl;

    for (int i = 0; i < CHUNKS; i++)
    {
        int r = rand();
        chunk[i] = r < 15000 ? 0 : 1;
    }
    chunk[0] = -1;


    VertexArray VAO;
    VertexBuffer VBO(quad, sizeof(quad), GL_STATIC_DRAW);

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glfwSwapBuffers(window0);

    ChunkManager manager;

    std::cout << "manager done" << std::endl;


    shader = Shader("ray.vert", "rayvoxel.frag");
    shader2 = Shader("sky.vert", "sky.frag");
    gShader = Shader("raycol.comp");
    ssaoShader = Shader("rayssao.comp");
    lightingShader = Shader("raylighting.comp");

    std::cout << "test0" << std::endl;
    Shader quadShader("screenquad.vert", "screenquad.frag");

    Texture grass("tiles.png", true, 0);

    Texture colTexture(screenWidth, screenHeight, 1, 1);
    Texture posTexture(screenWidth, screenHeight, 2, 2);
    Texture normHitTexture(screenWidth, screenHeight, 3, 3);

    Texture finalTexture(screenWidth, screenHeight, 4, 4);

    Texture ssaoTexture(screenWidth, screenHeight, 6, 6);
    
    std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between [0.0, 1.0]
    std::default_random_engine generator;
    std::vector<glm::vec3> ssaoKernel;
    for (unsigned int i = 0; i < 64; ++i)
    {
        glm::vec3 sample(
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator)
        );
        float scale = (float)i / 64.0;
        
        scale = 0.1f + scale * scale * (1.0f - 0.1f);
        sample *= scale;
        ssaoKernel.push_back(sample);
    }
    
    std::vector<glm::vec3> ssaoNoise;
    for (unsigned int i = 0; i < 16; i++)
    {
        glm::vec3 noise(
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator) * 2.0 - 1.0,
            0.0f);
        ssaoNoise.push_back(noise);
    }
    std::cout << "test1" << std::endl;

    Texture noiseTexture(4, 4, 5, 5, &ssaoNoise[0]);

    gShader.use();
    gShader.setVec2("uScreenSize", glm::vec2(screenWidth, screenHeight));

    gShader.setVec3("uCameraPosition", Camera::MAIN.m_Position);
    gShader.setInt("tex", grass.m_ActiveID);
    gShader.setInt("CHUNKS_X", CHUNKS_X);
    gShader.setInt("CHUNKS_Y", CHUNKS_Y);
    gShader.setInt("CHUNKS_Z", CHUNKS_Z);

    lightingShader.use();
    lightingShader.setVec2("uScreenSize", glm::vec2(screenWidth, screenHeight));
    lightingShader.setInt("CHUNKS_X", CHUNKS_X);
    lightingShader.setInt("CHUNKS_Y", CHUNKS_Y);
    lightingShader.setInt("CHUNKS_Z", CHUNKS_Z);

    lightingShader.setVec3("uCameraPosition", Camera::MAIN.m_Position);
    ssaoShader.use();
    ssaoShader.setVec2("uScreenSize", glm::vec2(screenWidth, screenHeight));

    ssaoShader.setVec3("uCameraPosition", Camera::MAIN.m_Position);

    for (int i = 0; i < 64; i++)
    {
        ssaoShader.setVec3("uSamples[" + std::to_string(i) + "]", ssaoKernel[i]);
    }


    std::cout << "test2" << std::endl;
    quadShader.use();
    quadShader.setInt("tex", finalTexture.m_ActiveID);


    shader2.use();
    shader2.setVec2("uScreenSize", glm::vec2(screenWidth, screenHeight));
    //shader.setIntArray("uChunk", chunk, sizeof(chunk));

    VertexBufferLayout layout;
    layout.AddVertexAttribute(GL_FLOAT, 2);
    layout.AddVertexAttribute(GL_FLOAT, 2);

    VAO.AddBuffer(VBO, layout);
//    std::cout << GL_MAX_UNIFORM_BUFFER_BINDINGS << std::endl;


    float accumulator = 0.0f;
    float loadAccumulator = 0.0f;

    int frames = 0;
    std::cout << "starting thread" << std::endl;
    std::thread loader = std::thread(&ChunkManager::UpdateChunk, &manager);
    bool running = false;
    int i = 0;

    float time = 0.0f;
    float timeSpeed = 0.0f;
    while (!glfwWindowShouldClose(window0))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        accumulator += deltaTime;
        loadAccumulator += deltaTime;
        frames++;


        if (accumulator >= 1.0f)
        {
            std::cout << "FPS: " << frames << std::endl;
            accumulator = 0.0f;
            frames = 0;
            running = true;
            //std::cout << manager.m_LoadedChunk[0].x << ", " << manager.m_LoadedChunk[0].y << ", " << manager.m_LoadedChunk[0].z << ", " << std::endl;
            //manager.UpdateChunk();
        }

        if (loadAccumulator >= 1.0f / 60.0f)
        {
            i = (i + 1) % (CHUNKS / 4);
            loadAccumulator = 0.0f;
        }
        manager.BufferChunks();

        //glClearColor(0.2, 0.2, 0.8, 1);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window0);

        //std::cout << Camera::MAIN.m_Position.x << ", " << Camera::MAIN.m_Position.y << ", " << Camera::MAIN.m_Position.z << ", " << std::endl;
        
        grass.Bind();

        gShader.use();
        gShader.setVec3("uCameraPosition", Camera::MAIN.m_Position);
        gShader.setVec3("uCameraFront", Camera::MAIN.m_Front);
        gShader.setVec3("uCameraRight", Camera::MAIN.m_Right);
        gShader.setVec3("uCameraUp", Camera::MAIN.m_Up);

        timeSpeed = ((timePass == 0) ? 0.01f : (timePass == 1) ? 0.1f : 1.0f);
        time = paused ? time : (time + timeSpeed * deltaTime);

        gShader.setFloat("uXAngle", cos(time + PI / 8));
        gShader.setFloat("uYAngle", sin(time + PI / 8));
        gShader.setFloat("uZAngle", cos(0));


        glDispatchCompute((unsigned int)screenWidth / 30, (unsigned int)screenHeight / 30, 1);

        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        
        ssaoShader.use();
        ssaoShader.setVec3("uCameraPosition", Camera::MAIN.m_Position);
        ssaoShader.setVec3("uCameraFront", Camera::MAIN.m_Front);
        ssaoShader.setVec3("uCameraRight", Camera::MAIN.m_Right);
        ssaoShader.setVec3("uCameraUp", Camera::MAIN.m_Up);

        glDispatchCompute((unsigned int)screenWidth / 30, (unsigned int)screenHeight / 30, 1);

        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        grass.Bind();

        lightingShader.use();
        lightingShader.setVec3("uCameraPosition", Camera::MAIN.m_Position);
        lightingShader.setVec3("uCameraFront", Camera::MAIN.m_Front);
        lightingShader.setVec3("uCameraRight", Camera::MAIN.m_Right);
        lightingShader.setVec3("uCameraUp", Camera::MAIN.m_Up);
        lightingShader.setVec3("uChunkPosition", glm::vec3(0, 0, 0));
        lightingShader.setBool("uPBR", PBR);

        lightingShader.setFloat("uXAngle", cos(time + PI / 8));
        lightingShader.setFloat("uYAngle", sin(time + PI / 8));
        lightingShader.setFloat("uZAngle", cos(0));



        glDispatchCompute((unsigned int)screenWidth / 30, (unsigned int)screenHeight / 30, 1);

        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        finalTexture.Bind();
        VAO.Bind();
        quadShader.use();
        quadShader.setInt("tex", finalTexture.m_ActiveID);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window0);
        glfwPollEvents();
    }

    manager.working = false;
    loader.join();
    delete[] chunk;

    return 0;
}


/*//std::cout << Camera::MAIN.m_Position.x << ", " << Camera::MAIN.m_Position.y << ", " << Camera::MAIN.m_Position.z << ", " << std::endl;

        grass.Bind();
        VAO.Bind();

        shader.use();
        shader.setVec3("uCameraPosition", Camera::MAIN.m_Position);
        shader.setVec3("uCameraFront", Camera::MAIN.m_Front);

        shader.setVec3("uChunkPosition", glm::vec3(0, 0, 0));



        shader.setFloat("uXAngle", cos(currentFrame * 0.1 + PI / 4));

        shader.setFloat("uYAngle", sin(currentFrame * 0.1 + PI / 4));
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
shader2.use();
shader2.setVec3("uCameraFront", Camera::MAIN.m_Front); */