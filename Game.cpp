#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Chunk.h"
#include "ChunkRenderer.h"
#include "Input.h"
#include "Shader.h"
#include "Time.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"



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
    //glfwSwapInterval(0);

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

    //Grab cursor
    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void loop(GLFWwindow* window)
{

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);

    GLuint vertices_old[] = {
        // 4 bits : normals, 12 bits : chunk_position, 16 bits : ID    
        //0b1001
        0x09000,
        0x09110,
        0x09100,
        0x09110,
        0x09000,
        0x09010,
       
        //0b0001
        0x01001,
        0x01101,
        0x01111,
        0x01111,
        0x01011,
        0x01001,
        
        //0b1100
        0x0C011,
        0x0C010,
        0x0C000,
        0x0C000,
        0x0C001,
        0x0C011,
        
        //0b0100
        0x04111,
        0x04100,
        0x04110,
        0x04100,
        0x04111,
        0x04101,
        
        //0b1010
        0x0A000,
        0x0A100,
        0x0A101,
        0x0A101,
        0x0A001,
        0x0A000,
        
        //0b0010
        0x02010,
        0x02111,
        0x02110,
        0x02111,
        0x02010,
        0x02011
    };

    GLuint vertices[] = {
        // 16 bits : ID, 4 bits : normals, 12 bits : chunk_position  
        
        // FAR
        //0b1001
        0x09000,
        0x09010,
        0x09110,
        0x09100,

        // NEAR
        //0b0001
        0x01001,
        0x01101,
        0x01111,
        0x01011,

        // LEFT
        //0b1100
        0x0C011,
        0x0C010,
        0x0C000,
        0x0C001,

        // RIGHT
        //0b0100
        0x04110,
        0x04111,
        0x04101,
        0x04100,

        // BOTTOM
        //0b1010
        0x0A000,
        0x0A100,
        0x0A101,
        0x0A001,

        //TOP
        //0b0010
        0x02010,
        0x02011,
        0x02111,
        0x02110
    };

    ChunkRenderer chunkRenderer;


    Shader shader("default.vert", "default.frag");

    float accumulator = 0.0f;
    int frames = 0;
    glm::vec3 chunkPos0(0.0, 0.0, 0.0);
    glm::vec3 chunkPos1(1.0, 0.0, 0.0);

    shader.use();
    shader.setInt("uChunkSize", CHUNK_SIZE);

    Chunk chunk0(glm::vec3(0.0, 0.0, 0.0));
    Chunk chunk1(glm::vec3(1.0, 0.0, 0.0));
    Chunk chunk2(glm::vec3(0.0, 0.0, 1.0));
    Chunk chunk3(glm::vec3(1.0, 0.0, 1.0));


    while (!glfwWindowShouldClose(window))
    {
        /* CALCULATE FPS */

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        accumulator += deltaTime;
        frames++;

        if (accumulator >= 1.0f)
        {
            std::cout << "FPS: " << frames << std::endl;
            accumulator = 0;
            frames = 0;
        }

        /* PROCESS INPUT */

        processInput(window);

        /* PERFORM CALCULATIONS */

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 1000.0f);
        glm::mat4 view = Camera::MAIN.getViewMatrix();

        shader.use();
        shader.setMat4("uProjection", projection);
        shader.setMat4("uView", view);

        /* RENDER SCENE */
//        chunkRenderer.Render(shader);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        chunkRenderer.Render(chunk0, shader);
        chunkRenderer.Render(chunk1, shader);
        chunkRenderer.Render(chunk2, shader);
        chunkRenderer.Render(chunk3, shader);



        glfwSwapBuffers(window);
        glfwPollEvents();


    }

    glfwTerminate();
}

int main()
{
    
    //Initialize window
    GLFWwindow* window;
    
    if (initGLFW(&window) == -1)
    {
        return -1;
    }

    loop(window);

	return 0;
}


