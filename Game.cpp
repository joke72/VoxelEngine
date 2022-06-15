#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "Camera.h"
#include "ChunkRenderer.h"
#include "Chunk.h"
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
    glfwSetWindowPos(*window, (2560 - screenWidth) / 2, (1440 - screenHeight) / 2);

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

    ChunkRenderer chunkRenderer;

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

    GLuint vertices_old_indexed[] = {
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

    GLuint vertices[] = {
        // 8 bits : ID, 2 bits unused, 4 bits : normals, 18 bits : chunk_position  
        // FAR
        //0b1001
        0b00000000001001000000000000000000,
        0b00000000011001000000000001000000,
        0b00000000111001000001000001000000,
        0b00000000101001000001000000000000,

        // NEAR
        //0b0001
        0b00000000000001000000000000000001,
        0b00000000010001000001000000000001,
        0b00000000110001000001000001000001,
        0b00000000100001000000000001000001,

        // LEFT
        //0b1100
        0b00000000001100000000000001000001,
        0b00000000011100000000000001000000,
        0b00000000111100000000000000000000,
        0b00000000101100000000000000000001,

        // RIGHT
        //0b0100
        0b00000000000100000001000001000000,
        0b00000000010100000001000001000001,
        0b00000000110100000001000000000001,
        0b00000000100100000001000000000000,

        // BOTTOM
        //0b1010
        0b00000000001010000000000000000000,
        0b00000000011010000001000000000000,
        0b00000000111010000001000000000001,
        0b00000000101010000000000000000001,

        //TOP
        //0b0010
        0b00000000000010000000000001000000,
        0b00000000010010000000000001000001,
        0b00000000110010000001000001000001,
        0b00000000100010000001000001000000
    };

    Shader shader("default.vert", "default.frag");

    float accumulator = 0.0f;
    int frames = 0;
    glm::vec3 chunkPos0(0.0, 0.0, 0.0);
    glm::vec3 chunkPos1(1.0, 0.0, 0.0);

    shader.use();
    shader.setInt("uChunkSize", CHUNK_SIZE);

    const int len = 4;

    std::vector<Chunk*> chunks;

    for (int x = 0; x < len; x++)
    {
        for (int z = 0; z < len; z++)
        {
            Chunk* c = new Chunk(glm::vec3(x, 0, z), chunkRenderer);
            chunks.push_back(c);
        }
    }


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

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        for (int i = 0; i < len * len; i++)
        {
            chunkRenderer.Render(chunks[i]->m_RenderData, shader);
        }
        


        glfwSwapBuffers(window);
        glfwPollEvents();


    }

    for (int i = 0; i < len * len; i++)
    {
        delete chunks[i];
    }

    glfwTerminate();
}

int main()
{

    std::cout << "test" << std::endl;
    //Initialize window
    GLFWwindow* window;
    
    if (initGLFW(&window) == -1)
    {
        return -1;
    }

    loop(window);

	return 0;
}


