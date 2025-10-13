#include "glad/glad.h"
#include "RenderCore.h"
#include "ChunkRender.h"
#include "world/chunk/chunk.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <filesystem>
#include <GLFW/glfw3.h>
#include "Camera.h"
RenderCore::RenderCore()
    : window_(std::make_unique<WindowGLFW>("FractalHorizon", 1280, 720)),
      chunk_(nullptr),
      chunkRenderer_(nullptr),
      _animAngle(0.0f),
      _rotationSpeed(glm::radians(30.0f)),
      _rotateModel(true)
{}

RenderCore::~RenderCore() {
    // Release chunk renderer and chunk (ChunkRenderer destructor deletes GL objects)
    chunkRenderer_.reset();
    chunk_.reset();

    // Destroy window and terminate GLFW
    if (window_ && window_->getHandle()) {
        glfwDestroyWindow(window_->getHandle());
    }
    glfwTerminate();
}

bool RenderCore::init() {
    if (!window_) return false;
    if (!window_->init()) {
        std::cerr << "[RenderCore] Window init failed\n";
        return false;
    }
    Camera camera;
    // Print GL version info to validate context / glad
    const GLubyte* ver = glGetString(GL_VERSION);
    const GLubyte* glsl = glGetString(GL_SHADING_LANGUAGE_VERSION);
    std::cout << "[RenderCore] GL Version: " << (ver ? reinterpret_cast<const char*>(ver) : "unknown")
              << ", GLSL: " << (glsl ? reinterpret_cast<const char*>(glsl) : "unknown") << std::endl;

    // Ensure viewport matches framebuffer size
    int w, h;
    glfwGetFramebufferSize(window_->getHandle(), &w, &h);
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);

    // Try to install GL debug callback if available (via glfwGetProcAddress)
    using DebugCallbackType = void(*)(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar*, const void*);
    using DebugMessageCallbackProc = void(*)(DebugCallbackType, const void*);
    void* proc = reinterpret_cast<void*>(glfwGetProcAddress("glDebugMessageCallback"));
    if (proc) {
        DebugMessageCallbackProc dbg = reinterpret_cast<DebugMessageCallbackProc>(proc);
        if (dbg) {
            // GL_DEBUG_OUTPUT may be missing in headers on some platforms; use numeric value 0x92E0
            glEnable(0x92E0);
            dbg(
                +[](GLenum source, GLenum type, GLuint id, GLenum severity,
                   GLsizei length, const GLchar* message, const void* userParam)
                {
                    std::cerr << "[GL DEBUG] sev=" << severity << " type=" << type
                              << " id=" << id << " msg=" << (message ? message : "(null)") << "\n";
                },
                nullptr);
            std::cerr << "[RenderCore] GL debug callback installed\n";
        }
    } else {
        std::cerr << "[RenderCore] glDebugMessageCallback not available via glfwGetProcAddress\n";
    }

    // Build shader paths relative to this source file so they are found regardless of CWD
    namespace fs = std::filesystem;
    fs::path srcDir = fs::path(__FILE__).parent_path(); // .../src/render
    fs::path shaderDir = srcDir / "shaders";
    fs::path vertPath = shaderDir / "circle.vertex";
    fs::path fragPath = shaderDir / "circle.fragment";
    std::cout << "[RenderCore] loading shaders from: " << vertPath << " and " << fragPath << std::endl;
    if (!fs::exists(vertPath) || !fs::exists(fragPath)) {
        std::cerr << "[RenderCore] Shader files not found\n";
    } else {
        shader_.compile(vertPath.string().c_str(), fragPath.string().c_str());
        std::cout << "[RenderCore] shader.compile() finished, ID=" << shader_.getID()
                  << " valid=" << (shader_.isValid() ? "yes" : "no") << std::endl;
    }

    // Initialize uniforms
    uniforms_.initDefault();
    uniforms_.resolution = glm::vec2(static_cast<float>(w), static_cast<float>(h));

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    // Create and upload a single chunk and its GPU buffers
    chunk_ = std::make_unique<Chunk>();
    chunk_->load();

    chunkRenderer_ = std::make_unique<ChunkRenderer>();
    chunkRenderer_->upload(*chunk_);

    return true;
}

void RenderCore::update(float dt) {
    if (window_) window_->pollEvents();

    // advance animation angle
    _animAngle += _rotationSpeed * dt;
    if (_animAngle > glm::two_pi<float>()) _animAngle -= glm::two_pi<float>();

    // toggle rotate mode on T press (edge-trigger)
    static bool tPrev = false;
    bool tNow = (glfwGetKey(window_->getHandle(), GLFW_KEY_T) == GLFW_PRESS);
    if (tNow && !tPrev) {
        _rotateModel = !_rotateModel;
        std::cout << "[RenderCore] rotateModel = " << (_rotateModel ? "model" : "camera") << std::endl;
    }
    tPrev = tNow;
}

void RenderCore::render() {
    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // compute matrices
    int w, h;
    glfwGetFramebufferSize(window_->getHandle(), &w, &h);
    if (h == 0) h = 1;
    float aspect = static_cast<float>(w) / static_cast<float>(h);

    // projection: perspective
    glm::mat4 proj = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f);

    // center of chunk in world coords
    glm::vec3 chunkCenter = glm::vec3(CHUNK_X * 0.5f, CHUNK_Y * 0.5f, CHUNK_Z * 0.5f);

    // base model: translate chunk so its center is at origin
    glm::mat4 model = glm::translate(glm::mat4(1.0f), -chunkCenter);

    // rotation around Y
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), _animAngle, glm::vec3(0.0f, 1.0f, 0.0f));

    // camera and model behavior
    glm::vec3 camPos;
    if (_rotateModel) {
        // rotate the model around Y
        model = rot * model;
        camPos = glm::vec3(0.0f, CHUNK_Y * 0.5f + 8.0f, CHUNK_Z * 2.0f);
    } else {
        // orbit the camera around the chunk center
        float radius = std::max<float>(CHUNK_X, CHUNK_Z) * 2.5f + 10.0f;
        camPos.x = cosf(_animAngle) * radius;
        camPos.z = sinf(_animAngle) * radius;
        camPos.y = CHUNK_Y * 0.5f + 8.0f;
    }

    // look at the center of the chunk
    glm::mat4 view = glm::lookAt(camPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // final MVP
    glm::mat4 mvp = proj * view * model;

    // Safety: ensure shader valid before using / uploading uniforms
    if (!shader_.isValid()) {
        std::cerr << "[RenderCore] shader invalid (ID=" << shader_.getID() << "), skipping draw for this frame\n";
        if (window_) window_->swapBuffers();
        return;
    }

    // Use shader and upload uniforms
    shader_.use();

    // push computed MVP into UniformState so upload() sends it to the shader
    uniforms_.uMVP = mvp;
    uniforms_.resolution = glm::vec2(static_cast<float>(w), static_cast<float>(h));

    // Upload other uniforms (this will call shader.setMat4("uMVP", uMVP) from UniformState::upload)
    uniforms_.upload(shader_);

    // Draw chunk
    if (chunkRenderer_) {
        chunkRenderer_->draw();
    }

    // Present
    if (window_) window_->swapBuffers();
}

bool RenderCore::shouldClose() const {
    return window_ ? window_->shouldClose() : true;
}

GLFWwindow* RenderCore::getWindowHandle() const {
    return window_ ? window_->getHandle() : nullptr;
}
