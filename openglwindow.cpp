#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>
#include <tiny_obj_loader.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>


void OpenGLWindow::handleEvent(SDL_Event& ev) {
  if (ev.type == SDL_KEYDOWN) {
    if (ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w)
      m_dollySpeed = 1.0f;
    if (ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s)
      m_dollySpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a)
      m_panSpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d)
      m_panSpeed = 1.0f;
    if (ev.key.keysym.sym == SDLK_q) m_truckSpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_e) m_truckSpeed = 1.0f;
  }
  if (ev.type == SDL_KEYUP) {
    if ((ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w) &&
        m_dollySpeed > 0)
      m_dollySpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s) &&
        m_dollySpeed < 0)
      m_dollySpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a) &&
        m_panSpeed < 0)
      m_panSpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d) &&
        m_panSpeed > 0)
      m_panSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_q && m_truckSpeed < 0) m_truckSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_e && m_truckSpeed > 0) m_truckSpeed = 0.0f;
  }
}

void OpenGLWindow::initializeGL() {
  abcg::glClearColor(0, 0, 255, 0);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program = createProgramFromFile(getAssetsPath() + "lookat.vert",
                                    getAssetsPath() + "lookat.frag");                              

  initializeSound(getAssetsPath() + "sons/hino.wav");
  m_ground.initializeGL(m_program);

  m_modelBola.loadObj(getAssetsPath() + "bola/bola.obj");
  m_modelJogador.loadObj(getAssetsPath() + "jogador/jogador.obj");
  m_modelAviao.loadObj(getAssetsPath() + "aviao/aviao.obj");
  m_modelJuiz.loadObj(getAssetsPath() + "juiz/juiz.obj");
  m_modelArvore.loadObj(getAssetsPath() + "arvore/arvore.obj");  

  m_modelBola.setupVAO(m_program);
  m_modelJogador.setupVAO(m_program);
  m_modelAviao.setupVAO(m_program);
  m_modelJuiz.setupVAO(m_program);
  m_modelArvore.setupVAO(m_program);    

  resizeGL(getWindowSettings().width, getWindowSettings().height);
}

void OpenGLWindow::initializeSound(std::string path) {
  // clean up previous sounds
  SDL_CloseAudioDevice(m_deviceId);
  SDL_FreeWAV(m_wavBuffer);

  SDL_AudioSpec wavSpec;
  Uint32 wavLength;

  if (SDL_LoadWAV(path.c_str(), &wavSpec, &m_wavBuffer, &wavLength) == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime(
        fmt::format("Failed to load sound {} ({})", path, SDL_GetError()))};
  }

  m_deviceId = SDL_OpenAudioDevice(nullptr, 0, &wavSpec, nullptr, 0);

  if (SDL_QueueAudio(m_deviceId, m_wavBuffer, wavLength) < 0) {
    throw abcg::Exception{abcg::Exception::Runtime(
        fmt::format("Failed to play sound {} ({})", path, SDL_GetError()))};
  }

  SDL_PauseAudioDevice(m_deviceId, 0);
}


void OpenGLWindow::paintGL() {
  update();

  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  abcg::glUseProgram(m_program);

  // Get location of uniform variables (could be precomputed)
  const GLint viewMatrixLoc{
      abcg::glGetUniformLocation(m_program, "viewMatrix")};
  const GLint projMatrixLoc{
      abcg::glGetUniformLocation(m_program, "projMatrix")};
  const GLint modelMatrixLoc{
      abcg::glGetUniformLocation(m_program, "modelMatrix")};
  const GLint colorLoc{abcg::glGetUniformLocation(m_program, "color")};  

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_projMatrix[0][0]);
  

  // bola vermelha
  glm::mat4 model{1.0f};
  model = glm::translate(model, glm::vec3(0.0f, 0.2f, 2.0f));  
  model = glm::scale(model, glm::vec3(0.25f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(colorLoc,  1.0f, 0.25f, 0.25f, 1.0f);  
  m_modelBola.render();

  // time azul  
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(1.0f, 0.5f, 1.0f));  
  model = glm::scale(model, glm::vec3(0.6f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(colorLoc, 0.0f, 0.8f, 1.0f, 1.0f);  
  m_modelJogador.render();  
  
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(2.0f, 0.5f, 1.0f));  
  model = glm::scale(model, glm::vec3(0.6f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(colorLoc, 0.0f, 0.8f, 1.0f, 1.0f);   
  m_modelJogador.render();                  
  

  // time verde
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-2.0f, 0.5f, 1.0f));  
  model = glm::scale(model, glm::vec3(0.6f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);  
  m_modelJogador.render();  
 
  model = glm::mat4(1.0);  
  model = glm::translate(model, glm::vec3(-1.0f, 0.5f, 1.0f));  
  model = glm::scale(model, glm::vec3(0.6f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);  
  m_modelJogador.render(); 

  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 0.5f, 1.0f));  
  model = glm::scale(model, glm::vec3(0.8f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(colorLoc, 255.0f, 255.0f, 0.0f, 1.0f);  
  m_modelJuiz.render(); 

  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-0.3f, 2.0f, 0.0f));  
  model = glm::scale(model, glm::vec3(0.4f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 0.0f);  
  m_modelAviao.render();   

  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-4.0f, 0.5f, 0.0f));  
  model = glm::scale(model, glm::vec3(1.0f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 0.0f);  
  m_modelArvore.render(); 

  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(4.0f, 0.5f, 0.0f));  
  model = glm::scale(model, glm::vec3(1.0f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 0.0f);  
  m_modelArvore.render(); 

  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-4.0f, 0.5f, 2.0f));  
  model = glm::scale(model, glm::vec3(1.0f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 0.0f);  
  m_modelArvore.render(); 

  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(4.0f, 0.5f, 2.0f));  
  model = glm::scale(model, glm::vec3(1.0f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 0.0f);  
  m_modelArvore.render(); 

  // Draw ground
  m_ground.paintGL();

  abcg::glUseProgram(0);
}

void OpenGLWindow::paintUI() { abcg::OpenGLWindow::paintUI(); }

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  m_camera.computeProjectionMatrix(width, height);
}

void OpenGLWindow::terminateGL() {
  m_modelBola.terminateGL();
  m_modelJogador.terminateGL();
  m_modelAviao.terminateGL();
  m_modelJuiz.terminateGL();
  m_modelArvore.terminateGL();
  m_ground.terminateGL();

  abcg::glDeleteProgram(m_program);  
}

void OpenGLWindow::update() {
  const float deltaTime{static_cast<float>(getDeltaTime())};

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
}