#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

int main(int argc, char *arv[]){
    printf("test\n");
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL){
        printf("Failed to create GLFW Window");
        glfwTerminate();
        return -1;
    }else{
        printf("GLFW Window Created\n");
    }
    glfwMakeContextCurrent(window);
    
    
    
    return 0;
}