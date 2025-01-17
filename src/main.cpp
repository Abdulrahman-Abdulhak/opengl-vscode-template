#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static unsigned int CompileShader(unsigned int type, std::string source) {
  // Shader ID
  unsigned int id = glCreateShader(type);
  auto src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  //! For Error Handling
  // Shader-Compilation result status
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (!result) {
    // Shader-Compilation error log message length
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

    // a buffer to store th Shader-Compilation error log message
    char* messageBuffer = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, messageBuffer);

    std::cout << "Failed to compile "
              << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
              << " shader!" << std::endl;
    std::cout << messageBuffer << std::endl;

    glDeleteShader(id);
    return 0;
  }

  return id;
}

static unsigned int CreateShaderProgram(std::string& vertexShader,
                                        std::string& fragmentShader) {
  // Program ID
  unsigned int id = glCreateProgram();

  // Vertex Shader ID
  auto vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  // Fragment Shader ID
  auto fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(id, vs);
  glAttachShader(id, fs);
  glLinkProgram(id);
  glValidateProgram(id);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return id;
}

int main(void) {
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK) {
    return -1;
  }

  std::cout << "Using OpenGL Driver: " << glGetString(GL_VERSION) << std::endl;

  float positions[6] = {-.5f, -.5f, 0.0f, .5f, .5f, -.5f};

  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

  std::string vertexShader =
      "#version 330 core\n"
      ""
      "layout(location = 0) in vec4 position;\n"
      ""
      "void main() {\n"
      "   gl_Position = position;\n"
      "}";
  std::string fragmentShader =
      "#version 330 core\n"
      ""
      "layout(location = 0) out vec4 color;\n"
      ""
      "void main() {\n"
      "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
      "}";
  auto shaderProgram = CreateShaderProgram(vertexShader, fragmentShader);
  glUseProgram(shaderProgram);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}