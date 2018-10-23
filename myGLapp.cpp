#define GLFW_DLL
// 注意 glad的引用一定要在GLFW之前
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

// 窗口大小改变时的回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    // 初始glfw，设置版本号为3.3，使用核心模式，应用设置
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // 创建一个窗口，将窗口的上下文应用到当前的主上下文
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }   
    glfwMakeContextCurrent(window);
    // 初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }
    // 初始化渲染窗口，及视口ViewPort
    glViewport(0, 0, 800, 600);
    // 绑定回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // 准备引擎
    // 循环不停的渲染，及渲染循环
    while(!glfwWindowShouldClose(window)){
        // 应用程序采用着双缓冲模式，一张在前面显示，一张在后面慢慢加载
        // Swap交换缓冲，完成立刻刷新
        glfwSwapBuffers(window);
        // 检查出发时间
        glfwPollEvents();
    }
    // 释放glfw的资源
    glfwTerminate();

    return 0;
}