
// g++ -o myapp main.cpp -lglfw -lGLESv2 -lEGL
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <iostream>
#include <cmath>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include "includes/texture.cpp"  
using namespace std;
// Updated vertex shader

const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord; // Circle texture coordinates
layout (location = 2) in vec2 saPos;
layout (location = 3) in vec2 sTexCoord; // Arrow texture coordinates
uniform mat4 model; // Transformation matrix
uniform vec2 pos;
out vec2 TexCoord;
out vec2 CircletexCoord; // Output for arrow texture coordinates

void main() 
{
    if (saPos.x > 0.0 || saPos.x < 0.0) {
        gl_Position = vec4(saPos.x, -saPos.y, 0.0, 1.0);
    } 
    else {
        gl_Position = model*vec4(aPos.x, -aPos.y, 0.0, 1.0);
        
    }

    TexCoord = aTexCoord;
    CircletexCoord = sTexCoord; // Pass arrow texture coordinates to the fragment shader
}
)";

const char *fragmentShaderSource = R"(
#version 330 core
in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D texture1;
uniform sampler2D CircleTexture; // New uniform for the arrow texture

void main()
{
    // Check if the fragment is inside the arrow's texture coordinate range (the arrow's head and body)
    if (TexCoord.y >= 0.0 && TexCoord.x >  0.0) { // Assuming arrow head is 0.1 units long
        // Display the arrow texture if the fragment is inside the arrow's head
        FragColor = texture(CircleTexture, TexCoord);
    }
    else {
        // Display the original texture if the fragment is outside the arrow's head
        FragColor = vec4(0.0f,0.0f,1.0f,1.0f)*texture(texture1, TexCoord);
    }
}
)";

const char *sfragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(0.678, 0.09, 0.333,1.0f); //BORDO RENK
}
)";

texture tex;
float circleYPosition = 0.0f;  // Vertical position of the circle
float circleRotation = 0.0f;   // Rotation angle of the circle in degrees
float circleYPositions = 0.0f; // Vertical position of the  inner circle
float circleRotations = 0.0f;  // Rotation angle of the inner circle in degrees

float randomnum(float min, float max)
{
    std::random_device rd;                               // Rastgele cihazdan başlatılan bir üreteç
    std::mt19937 gen(rd());                              // Mersenne Twister algoritmasıyla rastgelelik üreteci
    std::uniform_real_distribution<float> dis(min, max); // min ile max arasında uniform dağılım

    float random_number = dis(gen); // Rastgele ondalıklı sayı üret

    std::cout << "Rastgele Sayı: " << random_number << std::endl;
    return random_number;

}
void drawStick()
{
    std::vector<float> needleVertices = {
        -0.0001f,-0.6f,
        -0.1f,-0.5f, // upper triangle
        0.1f,-0.5f,

        -0.5f, 0.02f,
        0.5f,0.02f, // triangle for rectangle 1
        -0.5f,-0.02f,

        -0.5f,-0.02f,
        0.5f, 0.02f, // triangle for rectangle 2
        0.5f,-0.02f,

        0.00001f, -0.1f,
        0.04f,0.02f,  //lower triangle
        -0.04f,0.02f,
    };

    GLuint vertexShaders = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaders, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShaders);

    GLuint fragmentShaders = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaders, 1, &sfragmentShaderSource, nullptr);
    glCompileShader(fragmentShaders);

    // Create shader program
    GLuint shaderPrograms = glCreateProgram();
    glAttachShader(shaderPrograms, vertexShaders);
    glAttachShader(shaderPrograms, fragmentShaders);
    glLinkProgram(shaderPrograms);

    GLuint stickVBO;
    glGenBuffers(1, &stickVBO);
    glBindBuffer(GL_ARRAY_BUFFER, stickVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * needleVertices.size(), needleVertices.data(), GL_STATIC_DRAW);

    GLuint positionLoc = glGetAttribLocation(shaderPrograms, "saPos");
    glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(positionLoc);

    glUseProgram(shaderPrograms);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
    glDrawArrays(GL_TRIANGLES, 3, 6);
    glDrawArrays(GL_TRIANGLES, 9, 3);
  
    glDisableVertexAttribArray(positionLoc);

    glDeleteBuffers(1, &stickVBO);
    glDeleteProgram(shaderPrograms);
    glDeleteShader(vertexShaders);
    glDeleteShader(fragmentShaders);
}

void updateAltitude(GLFWwindow *window)
{
   
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        float a,b,c;
        a=randomnum(-85.0,85.0);
        b=randomnum(-0.1,0.1);
        c=randomnum(-35.0,35.0);
       
          if(a>0){
            cout<<"A=" <<a<<endl;
            for(float i =0;i<=ceil(abs(a));i+=0.0001)
            {
                circleRotation+=0.00001;
                
                            }
        }
        else
        {
            cout<<"A=" <<abs(a)<<endl;
              for(float i =0;i<=ceil(abs(a));i+=0.0001)
            {
                circleRotation-=0.00001;
                
            }
        }

        if(b>0){
            cout<<"b=" <<abs(b)<<endl;
            for(float i =0;i<=abs(b);i+=0.0001)
            {
                
                circleYPositions+=0.00001;
            }
        }
        else
        {
            cout<<"b=" <<abs(b)<<endl;
              for(float i =0;i<=abs(b);i+=0.0001)
            {
                circleYPositions-=0.00001;
            }
        }


        if(c>0){
                cout<<"c=" <<abs(c)<<endl;
            for(float i =0;i<=ceil(abs(c));i+=0.0001)
            {
                circleRotations+=0.00001;
            }
        }
        else
        {
            cout<<"c=" <<abs(c)<<endl;
              for(float i =0;i<=ceil(abs(c));i+=0.0001)
            {
                circleRotation-=0.00001;
            }
        }

    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        circleRotation = 0.0;
        circleYPositions = 0.0;
        circleRotations = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        circleRotation += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        circleRotation -= 1.0f;
    }
    //
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        circleYPositions += 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        circleYPositions -= 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        circleRotations += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        circleRotations -= 1.0f;
    }
}
void drawinnercircle()
{
    const int numSegments = 200;
    float circleVertices[(numSegments + 1) * 2];
    float texCoords[(numSegments + 1) * 2];

    const float radius = 0.80f;
    for (int i = 0; i <= numSegments; ++i)
    {
        float angle = i * 2.0f * M_PI / numSegments;
        circleVertices[i * 2] = radius * std::cos(angle);
        circleVertices[i * 2 + 1] = radius * std::sin(angle);
        texCoords[i * 2] = (circleVertices[i * 2] + radius) / (2 * radius);
        texCoords[i * 2 + 1] = (circleVertices[i * 2 + 1] + radius) / (2 * radius);
    }

    // Create vertex and fragment shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Create shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Create VBO for the circle
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices) + sizeof(texCoords), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(circleVertices), circleVertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(circleVertices), sizeof(texCoords), texCoords);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "aPos");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    GLint texAttrib = glGetAttribLocation(shaderProgram, "sTexCoord");
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)(sizeof(circleVertices)));
    glEnableVertexAttribArray(1);

    GLuint texture = tex.settextureinner();

    glUseProgram(shaderProgram);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(circleRotations), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(0.0f, circleYPositions-0.022, 0.0f));
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture); 

    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments + 1);

    glDeleteTextures(1, &texture);
}

void drawoutercircle()
{
    const int numSegments = 200;
    float circleVertices[(numSegments + 1) * 2];
    float texCoords[(numSegments + 1) * 2];

    const float radius = 1.0f;
    for (int i = 0; i <= numSegments; ++i)
    {
        float angle = i * 2.0f * M_PI / numSegments;
        circleVertices[i * 2] = radius * std::cos(angle);
        circleVertices[i * 2 + 1] = radius * std::sin(angle);
        texCoords[i * 2] = (circleVertices[i * 2] + radius) / (2 * radius);
        texCoords[i * 2 + 1] = (circleVertices[i * 2 + 1] + radius) / (2 * radius);
    }

    // Create vertex and fragment shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Create shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Create VBO for the circle
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices) + sizeof(texCoords), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(circleVertices), circleVertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(circleVertices), sizeof(texCoords), texCoords);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "aPos");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    GLint texAttrib = glGetAttribLocation(shaderProgram, "aTexCoord");
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)(sizeof(circleVertices)));
    glEnableVertexAttribArray(1);

    // Load texture
    GLuint texture = tex.settextureouter();

    glUseProgram(shaderProgram);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(circleRotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(0.0f, circleYPosition, 0.0f));
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture); 

    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments + 1);
}
GLFWwindow *init()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow *window = glfwCreateWindow(800, 800, "OpenGL ES 2.0 Attitude indicator", nullptr, nullptr);
    glfwMakeContextCurrent(window);

      // Initialize EGL
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLint major, minor;
    eglInitialize(display, &major, &minor);

    return window;

}

int main()
{
    // Initialize GLFW and create a window
    GLFWwindow *window = init();

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glEnable(GL_BLEND);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Enable alpha blending to draw lines on top of the texture
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // Use the arrow shader program and draw the arrow
        updateAltitude(window);
        drawinnercircle();
        drawoutercircle();
        drawStick();
        glDisable(GL_BLEND);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}