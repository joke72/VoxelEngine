#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "Camera.h"
#include "ChunkRenderer.h"
#include "Chunk.h"
#include "Framebuffer.h"
#include "Input.h"
#include "PointLight.h"
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

glm::vec3 getColor(int col)
{
    float r = (float) ((col >> 16) & 0xFF) / 255;
    float g = (float) ((col >> 8) & 0xFF) / 255;
    float b = (float) ((col >> 0) & 0xFF) / 255;

    return glm::vec3(r, g, b);
}
const int len = 2;

glm::vec3 lightDirection(1.0f, 1.0f, 1.0f);

void renderScene(const ChunkRenderer& chunkRenderer, const std::vector<Chunk*>& chunks, const Shader& shader)
{


    //PointLight::DrawLights(view, projection);

    for (int i = 0; i < len * len; i++)
    {
        chunkRenderer.Render(chunks[i]->m_RenderData, shader);
    }
    //pointLight0.Draw(view, projection, defaultShader);
}

uint32_t shadowWidth = 1024*4;
uint32_t shadowHeight = 1024*4;

void loop(GLFWwindow* window)
{
    glm::vec3 col = getColor(0x3E7C17);
    std::cout << col.x << ", " << col.y << ", " << col.z << std::endl;
    
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

    Shader::InitShaders();

    VertexBufferLayout::Init();

    PointLight pointLight0(glm::vec3(10.0f, 34.0f, 10.0f), glm::vec3(0.8f, 0.6f, 0.2f), 100.0f);

    PointLight pointLight1(glm::vec3(100.0f, 60.0f, 30.0f), glm::vec3(0.8f, 0.6f, 0.2f), 100.0f);

    PointLight pointLight2(glm::vec3(100.0f, 60.0f, 100.0f), glm::vec3(0.8f, 0.6f, 0.2f), 100.0f);


    float quad[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         0.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  0.0f,  0.0f, 1.0f,
         0.0f, -1.0f,  1.0f, 0.0f,
         0.0f,  0.0f,  1.0f, 1.0f
    };

    VertexArray frameBufferVAO;
    VertexBuffer vertexBuffer(&quad, sizeof(quad), GL_STATIC_DRAW);
    

    frameBufferVAO.AddBuffer(vertexBuffer, VertexBufferLayout::screenLayout);

    PointLight::InitShaderAll(Shader::blockShader);

    FramebufferSpecification cbSpec = {
        screenWidth, screenHeight, GL_RGBA16F, GL_RGBA, GL_COLOR_ATTACHMENT0, GL_FLOAT
    };
    FramebufferSpecification dbSpec = {
        shadowWidth, shadowHeight, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT, GL_FLOAT
    };

    Framebuffer framebuffer(cbSpec);
    
    framebuffer.AddRenderBuffer(GL_DEPTH24_STENCIL8, GL_DEPTH_ATTACHMENT);
    Framebuffer depthBuffer(dbSpec);


    float accumulator = 0.0f;
    int frames = 0;

    Shader::blockShader.use();
    Shader::blockShader.setInt("uChunkSize", CHUNK_SIZE);

    Shader::blockShader.setVec3("dirLight.ambient", glm::vec3(0.1, 0.1, 0.1));
    Shader::blockShader.setVec3("dirLight.diffuse", glm::vec3(1.0, 1.0, 1.0));
    Shader::blockShader.setVec3("dirLight.specular", glm::vec3(1.0, 1.0, 1.0));
    Shader::blockShader.setInt("shadowMap", 0);

    Shader::shadowShader.use();
    Shader::shadowShader.setInt("uChunkSize", CHUNK_SIZE);

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

        
        lightDirection.x = cos(glfwGetTime());
        lightDirection.y = sin(glfwGetTime());
        lightDirection.z = cos(glfwGetTime());
        
        float light = lightDirection.y * 0.5f + 0.5f;

        glEnable(GL_DEPTH_TEST);


        glm::vec3 col = getColor(0x92B4EC);
        glClearColor(col.x * light, col.y * light, col.z * light, 1.0f);

        float shadow_near_plane = 1.0f, shadow_far_plane = 150.0f;

        glm::mat4 lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, shadow_near_plane, shadow_far_plane);

        glm::vec3 lightPos(32.0f, 32.0f, 32.0f);
        lightPos *= len*0.5;

        glm::mat4 lightView = glm::lookAt(lightPos + normalize(lightDirection)*50.0f,
            lightPos,
            glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        Shader::shadowShader.use();
        Shader::shadowShader.setMat4("uLightSpaceMatrix", lightSpaceMatrix);


        glViewport(0, 0, shadowWidth, shadowHeight);
        depthBuffer.Bind();
        glClear(GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT);
        renderScene(chunkRenderer, chunks, Shader::shadowShader);
        glCullFace(GL_BACK);
        depthBuffer.Unbind();

        glViewport(0, 0, screenWidth, screenHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Shader::blockShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 1000.0f);
        glm::mat4 view = Camera::MAIN.getViewMatrix();

        Shader::blockShader.setMat4("uProjection", projection);
        Shader::blockShader.setMat4("uView", view);


        Shader::blockShader.setVec3("uViewPos", Camera::MAIN.m_Position);
        Shader::blockShader.setVec3("dirLight.direction", lightDirection);
        Shader::blockShader.setMat4("uLightSpaceMatrix", lightSpaceMatrix);
        PointLight::UpdateShaderAll(Shader::blockShader, view);

        glActiveTexture(GL_TEXTURE0);
        depthBuffer.BindTexture();
        renderScene(chunkRenderer, chunks, Shader::blockShader);
        /*
        Shader::screenShader.use();
        Shader::screenShader.setFloat("exposure", exposure);
        frameBufferVAO.Bind();
        glDisable(GL_DEPTH_TEST);
        glActiveTexture(GL_TEXTURE0);
        depthBuffer.BindTexture();
        glDrawArrays(GL_TRIANGLES, 0, 6);*/
        
        /* PERFORM CALCULATIONS 
        float shadow_near_plane = 1.0f, shadow_far_plane = 100.0f;


        glm::mat4 projection = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 1000.0f);
        glm::mat4 view       = Camera::MAIN.getViewMatrix();

        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, shadow_near_plane, shadow_far_plane);

        glm::mat4 lightView = glm::lookAt(-1.0f * 80.0f * lightDirection + Camera::MAIN.m_Position,
            Camera::MAIN.m_Position,
            glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        Shader::blockShader.use();
        Shader::blockShader.setMat4("uProjection", projection);
        Shader::blockShader.setMat4("uView", view);

        Shader::blockShader.setVec3("dirLight.direction", lightDirection);
        Shader::blockShader.setVec3("uViewPos", Camera::MAIN.m_Position);
        Shader::blockShader.setMat4("uLightSpaceMatrix", lightSpaceMatrix);

        Shader::shadowShader.use();
        Shader::shadowShader.setMat4("uLightSpaceMatrix", lightSpaceMatrix);

        /* RENDER SCENE TO SHADOWMAP


        glViewport(0, 0, shadowWidth, shadowHeight);
        depthBuffer.Bind();
        glClear(GL_DEPTH_BUFFER_BIT);

        renderScene(chunkRenderer, chunks, view, projection, Shader::shadowShader);

        depthBuffer.Unbind();

        glViewport(0, 0, screenWidth, screenHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

         CLEAR SCREEN 
        Shader::blockShader.use();
        //glm::vec3 col = getColor(0x92B4EC);
        //glClearColor(col.x, col.y, col.z, 1.0f);

        framebuffer.Bind();

         RENDER SCENE TO FRAMEBUFFER 
        glActiveTexture(GL_TEXTURE0);
        depthBuffer.BindTexture();
        renderScene(chunkRenderer, chunks, view, projection, Shader::blockShader);

        framebuffer.Unbind();
        
         RENDER FRAMEBUFFER 
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Shader::screenShader.use();
        Shader::screenShader.setFloat("exposure", exposure);
        frameBufferVAO.Bind();
        glDisable(GL_DEPTH_TEST);
        glActiveTexture(GL_TEXTURE0);
        framebuffer.BindTexture();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        */
        glfwSwapBuffers(window);
        glfwPollEvents();


    }

    for (int i = 0; i < len * len; i++)
    {
        delete chunks[i];
    }

    PointLight::Deinit();

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


