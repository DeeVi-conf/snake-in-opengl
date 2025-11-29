#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void drawsquare(float x, float y, float size, float r, float g, float b);

void processInput(GLFWwindow* window);

int direction;

unsigned int shaderProgram;
unsigned int VAO, VBO, EBO;

float snake_pos_x = 0.9f, snake_pos_y = -0.9f;

enum input {
    UP, DOWN , LEFT, RIGHT
};

struct block {
    float pos_x, pos_y;
};

int size = 1;

const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"uniform vec2 uOffset;\n"
"uniform float uScale;\n"
"void main()\n"
"{\n"
"   vec2 scaled = aPos * uScale;\n"
"   gl_Position = vec4(scaled + uOffset, 0.0, 1.0);\n"
"}\n";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec3 uColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(uColor, 1.0);\n"
"}\n";

int main(int argc, char *arv[]){
    printf("test\n");

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "LearnOpenGL", NULL, NULL);
    if (window == NULL){
        printf("Failed to create GLFW Window");
        glfwTerminate();
        return -1;
    }else{
        printf("GLFW Window Created\n");
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        return -1;
    }

    // Square !!!!
    float vertices[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f,  0.5f,
        -0.5f,  0.5f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Shader !!!
    unsigned int vertexShader = 0;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    double lastTime = .0;
    direction = LEFT;

    struct block array[100]; 
    while(!glfwWindowShouldClose(window)){

        processInput(window);

        glClearColor(0.1f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Game logic Time !!!
        double time = glfwGetTime();
        
        if(time > lastTime + .7){
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) size++;

            for (int i = size - 1; i > 0; i--) {
                array[i] = array[i - 1];
            }

            if(direction == UP) snake_pos_y += .2f;
            if(direction == DOWN) snake_pos_y -= .2f;
            if(direction == LEFT) snake_pos_x -= .2f;
            if(direction == RIGHT) snake_pos_x += .2f;
            
            if(snake_pos_x < -0.9f) snake_pos_x =  0.9f;
            if(snake_pos_x >  0.9f) snake_pos_x = -0.9f;
            if(snake_pos_y < -0.9f) snake_pos_y=  0.9f;
            if(snake_pos_y >  0.9f) snake_pos_y = -0.9f;
            
            lastTime = time;

            array[0].pos_x = snake_pos_x; array[0].pos_y = snake_pos_y;
            drawsquare(snake_pos_x, snake_pos_y, .2f, 0.0f, 0.0f, 1.0f);

            for(int i = 0; i < size; i++){
                drawsquare(array[i].pos_x, array[i].pos_y, .2f, 0.0f, 0.0f, 1.0f);
            }

            glfwSwapBuffers(window);
            
            if(glfwWindowShouldClose(window)){
                break;
            }
        }
        glfwPollEvents();
    }
    
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void drawsquare(float x, float y, float size, float r, float g, float b)
{
    glUseProgram(shaderProgram);

    // uniforms
    int offsetLoc = glGetUniformLocation(shaderProgram, "uOffset");
    glUniform2f(offsetLoc, x, y);

    int scaleLoc = glGetUniformLocation(shaderProgram, "uScale");
    glUniform1f(scaleLoc, size);

    int colorLoc = glGetUniformLocation(shaderProgram, "uColor");
    glUniform3f(colorLoc, r, g, b);

    // draw
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, 1);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) direction = UP;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) direction = DOWN;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) direction  = RIGHT;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) direction = LEFT;

    //if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) size++;
}