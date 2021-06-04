////////////////////////////////////////////////////////////////////////////////
#include <glad/glad.h>
#include <GLFW/glfw3.h>

////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <cmath>
#include <string>

////////////////////////////////////////////////////////////////////////////////
#include "config.h"
#include "error.h"
#include "io.h"
#include "log.h"
#include "fpscamera.h"
#include "glmesh.h"
#include "glmaterial.h"
#include "gltexture_loader.h"
#include "glshader_loader.h"
#include "glmodel.h"

////////////////////////////////////////////////////////////////////////////////
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

////////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG
#include "gldebug.h"
#endif

////////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG
        #define WINDOW_TITLE    "Lovelace Engine v0.0.1 (DEBUG)"
#else
        #define WINDOW_TITLE     "Lovelace Engine v0.0.1 (RELEASE)"
#endif
#define WINDOW_WIDTH    1600
#define WINDOW_HEIGHT   900

////////////////////////////////////////////////////////////////////////////////
#define CAMERA_MIN_FOV  30.0f
#define CAMERA_MAX_FOV  120.0f

////////////////////////////////////////////////////////////////////////////////
#define SUCCESS         0x0     // program finished successfully
#define WINDOW_ERR      0x1     // window initialization failed
#define GLAD_ERR        0x2     // could't initialize GLAD
#define INIT_ERR        0x3     // GLFW initialization failed
#define EXCEPT_ERR      0x4     // an exception has occurred

////////////////////////////////////////////////////////////////////////////////
int fwidth = WINDOW_WIDTH;
int fheight = WINDOW_HEIGHT;
bool cursorCaptured;

////////////////////////////////////////////////////////////////////////////////
al::fpscamera camera((float)fwidth, (float)fheight, {
        GLFW_KEY_W,                     // forward
        GLFW_KEY_S,                     // backward
        GLFW_KEY_A,                     // left
        GLFW_KEY_D,                     // right
        GLFW_KEY_Q,                     // up
        GLFW_KEY_E,                     // down
        GLFW_KEY_LEFT_SHIFT,            // faster
        GLFW_PRESS,                     // press
        GLFW_RELEASE                    // release
});

////////////////////////////////////////////////////////////////////////////////
void error_callback(int error, const char* description)
{
        std::cerr << "[GLFW] Error (" << error << "): " << description << '\n';
}

////////////////////////////////////////////////////////////////////////////////
void window_close_callback(GLFWwindow* window)
{

}

////////////////////////////////////////////////////////////////////////////////
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
        glViewport(0, 0, width, height);
        fwidth = width;
        fheight = height;
        camera.mScreenWidth = (float)fwidth;
        camera.mScreenHeight = (float)fheight;
}

////////////////////////////////////////////////////////////////////////////////
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GLFW_TRUE);

        camera.updateState(key, action);
}

////////////////////////////////////////////////////////////////////////////////
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
        camera.orient((float)xpos, (float)ypos);
}

////////////////////////////////////////////////////////////////////////////////
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
                cursorCaptured = !cursorCaptured;

        if (cursorCaptured) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                camera.mActive = true;
        }
        else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                camera.mActive = false;
        }
}

////////////////////////////////////////////////////////////////////////////////
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
        float fov = camera.mFov;
        fov -= (float)yoffset * 5;
        if (fov < CAMERA_MIN_FOV)
                fov = CAMERA_MIN_FOV;
        else if (fov > CAMERA_MAX_FOV)
                fov = CAMERA_MAX_FOV;
        camera.mFov = fov;
}

////////////////////////////////////////////////////////////////////////////////
void drop_callback(GLFWwindow* window, int count, const char** paths)
{

}

////////////////////////////////////////////////////////////////////////////////
void lovelace_clear()
{
        al::gl::texture_loader::clear();
        al::gl::shader_loader::clear();
}

////////////////////////////////////////////////////////////////////////////////
int main(void)
{
        glfwSetErrorCallback(error_callback);
        if (!glfwInit()) {
                al::log(std::cerr, "[GLFW] Error: failed to initialized");
                return INIT_ERR;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
#ifdef DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

        GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
        if (!window) {
                al::log(std::cerr, "[GLFW] Error: couldn't create a window");
                glfwTerminate();
                return WINDOW_ERR;
        }

        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                al::log(std::cerr, "[GLAD] Error: couldn't initialize GLAD");
                glfwTerminate();
                return GLAD_ERR;
        }

        al::log(std::cout, "[LovelaceEngine] Version ", LOVELACE_VERSION_MAJOR, '.', LOVELACE_VERSION_MINOR);

#ifdef DEBUG
        int dflags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &dflags);
        if (dflags & GL_CONTEXT_FLAG_DEBUG_BIT) {
                glEnable(GL_DEBUG_OUTPUT);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                glDebugMessageCallback(al::gl::debugOutput, nullptr);
                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
                al::log(std::cout, "[OpenGL] Running in debug mode");
        }
#endif

        al::log(std::cout, "[OpenGL] Renderer: ", glGetString(GL_RENDERER));
        al::log(std::cout, "[OpenGL] Version: ", glGetString(GL_VERSION));

        glfwSetWindowCloseCallback(window, window_close_callback);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, cursor_pos_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetDropCallback(window, drop_callback);

        if (glfwRawMouseMotionSupported())
                glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

        glfwGetFramebufferSize(window, &fwidth, &fheight);
        glViewport(0, 0, fwidth, fheight);

        glfwSwapInterval(1);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        try {
                using namespace std::string_literals;

                camera.mSpeed = 25.0f;

                // meshes
                al::gl::model scene(LOVELACE_ROOT_DIR "models/sponza/sponza.obj");
                al::gl::mesh cube = al::gl::genCube();

                // directional light
                al::dir_light sun = {
                        glm::vec3(0.2f, 0.2f, 0.2f),            // ambient
                        glm::vec3(1.0f, 1.0f, 1.0f),            // diffuse
                        glm::vec3(1.0f, 1.0f, 1.0f),            // specular
                        glm::vec3(glm::cos(glm::radians(-60.0f)), glm::sin(glm::radians(-60.0f)), 0.0f),      // direction
                        0.5f                                    // intensity
                };

                // material for scene
                al::gl::phong_textured_material sceneMat;
                sceneMat.mShininess = 32.0f;
                sceneMat.mDirLights.push_back(&sun);

                // material for cube
                al::gl::basic_color_material cubeMat;

                // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                while (!glfwWindowShouldClose(window)) {
                        float dt = []() {
                                static float lastTime;
                                static bool firstRun = true;

                                float currentTime = (float)glfwGetTime();
                                if (firstRun) {
                                        firstRun = false;
                                        lastTime = currentTime;
                                }

                                float deltaTime = currentTime - lastTime;
                                lastTime = currentTime;
                                return deltaTime;
                        }();

                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                        camera.update(dt);

                        glm::mat4 projection = camera.getProjection();
                        glm::mat4 view = camera.getView();

                        // draw scene
                        glm::mat4 sceneModel = glm::mat4(1.0f);
                        sceneModel = glm::scale(sceneModel, glm::vec3(0.1f));
                        glm::mat4 sceneNormal = glm::transpose(glm::inverse(sceneModel));
                        glm::mat4 scenePVM = projection * view * sceneModel;

                        sceneMat.use();
                        sceneMat.update(camera.mPosition, sceneNormal, sceneModel, scenePVM);
                        scene.draw();
                        sceneMat.halt();

                        // draw cube
                        glm::mat4 cubeModel = glm::mat4(1.0f);
                        cubeModel = glm::translate(cubeModel, glm::vec3(5.0f, 5.0f, 0.0f));
                        glm::mat4 cubeNormal = glm::transpose(glm::inverse(cubeModel));
                        glm::mat4 cubePVM = projection * view * cubeModel;

                        cubeMat.use();
                        cubeMat.update(glm::vec4(1.0f, 0.5f, 0.25f, 1.0f), cubePVM);
                        cube.draw();
                        cubeMat.halt();

                        glfwSwapBuffers(window);
                        glfwPollEvents();
                }
        }
        catch (const std::exception& e) {
                al::log(std::cerr, e.what());
                lovelace_clear();
                glfwTerminate();
                return EXCEPT_ERR;
        }
        catch (...) {
                al::log(std::cerr, "unknown exception has occurred");
                lovelace_clear();
                glfwTerminate();
                return EXCEPT_ERR;
        }

        lovelace_clear();
        glfwTerminate();
        return SUCCESS;
}
