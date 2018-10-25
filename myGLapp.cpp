#define GLFW_DLL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
// 注意 glad的引用一定要在GLFW之前
using namespace std;

void initializeGLFW();
int initializeGLAD();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void compileMyShader(unsigned int* PvertexShader, unsigned int* PfragmentShader);
void makeTriangle(unsigned int VAO);
void makeRectangle(unsigned int VAO);

// 常量声明
const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main(){\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main(){\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

int main()
{
    // 初始化GLFW
    initializeGLFW();
    // 创建一个窗口，将窗口的上下文应用到当前的主上下文
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL){
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }   
    glfwMakeContextCurrent(window);
    // 初始化GLAD
    if(!initializeGLAD())
        return -1;

    // 初始化渲染窗口，及视口ViewPort
    glViewport(0, 0, 800, 600);
    // 绑定回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 创建和编译我的着色器
    unsigned int vertexShader;
    unsigned int fragmentShader;
    compileMyShader(&vertexShader, &fragmentShader);
    // 创建着色器程序
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // 删除着色器对象，连接后就不需要了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 创建一个三角形的Vertex Array Object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    makeTriangle(VAO);
    // 创建一个矩形
    unsigned int VAORect;
    glGenVertexArrays(1, &VAORect);
    makeRectangle(VAORect);

    // 准备引擎
    // 循环不停的渲染，及渲染循环
    while(!glfwWindowShouldClose(window)){
        // 检查用户的输入
        processInput(window);
        
        // 清空颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 应用着色器程序
        glUseProgram(shaderProgram);
        // 绘制三角形
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // 通过EBO来绘制矩形
        // glBindVertexArray(VAORect);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // 检查触发事件
        glfwPollEvents();
        // 应用程序采用着双缓冲模式，一张在前面显示，一张在后面慢慢加载
        // Swap交换缓冲，完成立刻刷新
        glfwSwapBuffers(window);
    }

    // 释放glfw的资源
    glfwTerminate();

    return 0;
}

void initializeGLFW(){
    // 初始GLFW，设置版本号为3.3，使用核心模式
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

int initializeGLAD(){
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return 0;
    }
    return 1;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void compileMyShader(unsigned int* PvertexShader, unsigned int* PfragmentShader){
    // 创建和编译着色器
    // 向量着色器
    *PvertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(*PvertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(*PvertexShader);
    // 检查向量着色器是否被创建成功
    int success;
    char infoLog[512];
    glGetShaderiv(*PvertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(*PvertexShader, 512, NULL, infoLog);
        cout << "ERROR::SHAFER::VERTEX::COMPILATION_FAILED\n"
             << infoLog << endl;
    }
    // 片段着色器
    *PfragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(*PfragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(*PfragmentShader);
    // 检查片段着色器是否被创建成功
    glGetShaderiv(*PfragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(*PfragmentShader, 512, NULL, infoLog);
        cout << "ERROR::SHAFER::FRAGMENT::COMPILATION_FAILED\n"
             << infoLog << endl;
    }
}

void makeTriangle(unsigned int VAO){
    // 绑定VAO来存储下面的顶点解释和可见性设置
    glBindVertexArray(VAO);

    // 生成VBO对象（顶点缓冲对象）
    float vertices[]{
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
        -0.5f, 0.75f, 0.0f,
         0.5f, 0.75f, 0.0f
    };
    unsigned int VBO;
    // 1是需要创建的缓存数量，&VBO是存储申请的缓存ID的地址
    glGenBuffers(1, &VBO);
    // 将VBO对应的缓冲对象绑定到GL_ARRAY_BUFFER目标上
    // 从这一条语句执行结束后，所有对GL_ARRAY_BUFFER的调用
    // 都会用于配置刚刚绑定的缓冲对象
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 向GL_ARRAY_BUFFER拷贝数据
    // 第四个参数有三种，表示数据的类型
    // GL_STATIC_DRAW: 数据基本不会改变
    // GL_DYNAMIC_DRAW：数据会被改变很多（可能被保存到高速缓冲区）
    // GL_STREAM_DRAW：数据每次绘制都会改变（可能被保存到高速缓冲区）
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 设置如何解释定点数据
    // 第一个参数0和VertexShader中的layout(location=0)对应
    // 第二个参数代表一个顶点有几个属性，vec3
    // 第三个参数代表属性的类型
    // 第四个参数代表数据是否标准化（映射到0，1）
    // 第五个参数代表每个顶点之间的距离，步长stide
    // 第六个参数代表在缓冲中起始位置的偏移量
    // 该函数规定从最近与GL_ARRAY_BUFFER绑定VBO中获取数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    // 设置VAO的第0个数据组可用
    glEnableVertexAttribArray(0);
    // 解绑
    glBindVertexArray(0);
}

void makeRectangle(unsigned int VAO){
    // 绑定VAO
    glBindVertexArray(VAO);
    // 绑定VBO
    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 绑定EBO
    unsigned int indices[] = { // 注意索引从0开始
        0, 1, 3,  // 第一个三角形
        1, 2, 3   // 第二个三角形
    };
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 设置定点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // 解绑
    glBindVertexArray(0);
}