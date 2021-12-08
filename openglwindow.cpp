#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>
#include <tiny_obj_loader.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <unordered_map>
#include "imfilebrowser.h"


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
  m_programTexture = createProgramFromFile(getAssetsPath() + "shaders/texture.vert",
                                    getAssetsPath() + "shaders/texture.frag");

  m_programLookat = createProgramFromFile(getAssetsPath() + "shaders/lookat.vert",
                                          getAssetsPath() + "shaders/lookat.frag");                                  

  m_ground.initializeGL(m_programLookat);

  m_modelBola.loadObj(getAssetsPath() + "bola/bola.obj");
  m_modelBola.loadDiffuseTexture(getAssetsPath() + "bola/bola.tga"); 
  m_modelBola.setupVAO(m_programTexture); 


  m_modelAviao.loadObj(getAssetsPath() + "aviao/aviao.obj");
  m_modelAviao.loadDiffuseTexture(getAssetsPath() + "aviao/11803_Airplane_body_diff.jpg"); 
  m_modelAviao.setupVAO(m_programTexture);
  //m_Ka = m_modelAviao.getKa();
  //m_Kd = m_modelAviao.getKd();


  m_modelArvore.loadObj(getAssetsPath() + "arvore/arvore.obj");
  m_modelArvore.loadDiffuseTexture(getAssetsPath() + "arvore/arvore.png"); 
  m_modelArvore.setupVAO(m_programTexture);
  //m_Ks = m_modelArvore.getKs(); 

  m_modelJuiz.loadObj(getAssetsPath() + "juiz/juiz.obj");
  m_modelJuiz.loadDiffuseTexture(getAssetsPath() + "juiz/juiz.jpg"); 
  m_modelJuiz.setupVAO(m_programTexture);
  //m_Ka = m_modelJuiz.getKa();
  //m_Kd = m_modelJuiz.getKd();
  //m_Ks = m_modelJuiz.getKs(); 
  

  m_modelJogador.loadObj(getAssetsPath() + "jogador/jogador.obj");
  m_modelJogador.setupVAO(m_programTexture);

  //initializeSound(getAssetsPath() + "sons/hino.wav");  

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

  // Use currently selected program
  //const auto program{m_programs.at(m_currentProgramIndex)};

  abcg::glUseProgram(m_programLookat);
  paintGLLookat();

  abcg::glUseProgram(m_programTexture);
  paintGLTexture();

  abcg::glUseProgram(0);

}

void OpenGLWindow::paintGLLookat() {  

  // Get location of uniform variables (could be precomputed)
  const GLint viewMatrixLoc{
      abcg::glGetUniformLocation(m_programLookat, "viewMatrix")};
  const GLint projMatrixLoc{
      abcg::glGetUniformLocation(m_programLookat, "projMatrix")};  

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_projMatrix[0][0]);

  // Draw ground
  m_ground.paintGL();

}

void OpenGLWindow::paintGLTexture() {
  // Get location of uniform variables (could be precomputed)
  const GLint viewMatrixLoc{abcg::glGetUniformLocation(m_programTexture, "viewMatrix")};
  const GLint projMatrixLoc{abcg::glGetUniformLocation(m_programTexture, "projMatrix")};
  const GLint modelMatrixLoc{
      abcg::glGetUniformLocation(m_programTexture, "modelMatrix")};
  const GLint colorLoc{abcg::glGetUniformLocation(m_programTexture, "color")};
  const GLint normalMatrixLoc{
      abcg::glGetUniformLocation(m_programTexture, "normalMatrix")};
  const GLint lightDirLoc{
      abcg::glGetUniformLocation(m_programTexture, "lightDirWorldSpace")}; 
  const GLint IaLoc{abcg::glGetUniformLocation(m_programTexture, "Ia")};
  const GLint IdLoc{abcg::glGetUniformLocation(m_programTexture, "Id")};
  const GLint IsLoc{abcg::glGetUniformLocation(m_programTexture, "Is")};
  const GLint KaLoc{abcg::glGetUniformLocation(m_programTexture, "Ka")};
  const GLint KdLoc{abcg::glGetUniformLocation(m_programTexture, "Kd")};
  const GLint KsLoc{abcg::glGetUniformLocation(m_programTexture, "Ks")};
  const GLint diffuseTexLoc{abcg::glGetUniformLocation(m_programTexture, "diffuseTex")};
  const GLint normalTexLoc{abcg::glGetUniformLocation(m_programTexture, "normalTex")};
  const GLint mappingModeLoc{
      abcg::glGetUniformLocation(m_programTexture, "mappingMode")};

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_projMatrix[0][0]);

  abcg::glUniform1i(diffuseTexLoc, 0);
  abcg::glUniform1i(normalTexLoc, 1);
  abcg::glUniform1i(mappingModeLoc, m_mappingMode);

  const auto lightDirRotated{m_camera.m_projMatrix * m_lightDir};
  abcg::glUniform4fv(lightDirLoc, 1, &lightDirRotated.x);
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);                         
  
  // Set uniform variables of the current object
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix[0][0]);

  const auto modelViewMatrix{glm::mat3(m_viewMatrix * m_modelMatrix)};
  glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

  
  // bola
  glm::mat4 model{1.0f};
  model = glm::translate(model, glm::vec3(0.0f, 0.2f, 2.0f));  
  model = glm::scale(model, glm::vec3(0.25f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(colorLoc,  1.0f, 0.25f, 0.25f, 1.0f);  

  auto ka = m_modelBola.getKa();
  auto kd = m_modelBola.getKd();
  auto ks = m_modelBola.getKs();

  abcg::glUniform4fv(KaLoc, 1, &ka.x);
  abcg::glUniform4fv(KdLoc, 1, &kd.x);
  abcg::glUniform4fv(KsLoc, 1, &ks.x);
  m_modelBola.render(KaLoc,KdLoc,KsLoc);

  //aviao
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-0.3f, 2.0f, 0.0f));  
  model = glm::scale(model, glm::vec3(0.4f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 0.0f); 

  ka = m_modelAviao.getKa();
  kd = m_modelAviao.getKd();
  ks = m_modelAviao.getKs();

  abcg::glUniform4fv(KaLoc, 1, &ka.x);
  abcg::glUniform4fv(KdLoc, 1, &kd.x);
  abcg::glUniform4fv(KsLoc, 1, &ks.x);
  m_modelAviao.render(KaLoc,KdLoc,KsLoc); 

  //arvore
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-4.0f, 0.5f, 0.0f));  
  model = glm::scale(model, glm::vec3(1.0f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 0.0f); 

  ka = m_modelArvore.getKa();
  kd = m_modelArvore.getKd();
  ks = m_modelArvore.getKs();

  abcg::glUniform4fv(KaLoc, 1, &ka.x);
  abcg::glUniform4fv(KdLoc, 1, &kd.x); 
  abcg::glUniform4fv(KsLoc, 1, &ks.x); 
  m_modelArvore.render(KaLoc,KdLoc,KsLoc); 

  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(4.0f, 0.5f, 0.0f));  
  model = glm::scale(model, glm::vec3(1.0f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 0.0f);  
  m_modelArvore.render(KaLoc,KdLoc,KsLoc); 

  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-4.0f, 0.5f, 2.0f));  
  model = glm::scale(model, glm::vec3(1.0f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 0.0f);  
  m_modelArvore.render(KaLoc,KdLoc,KsLoc); 

  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(4.0f, 0.5f, 2.0f));  
  model = glm::scale(model, glm::vec3(1.0f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 0.0f);  
  m_modelArvore.render(KaLoc,KdLoc,KsLoc); 

  //juiz
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 0.5f, 1.0f));  
  model = glm::scale(model, glm::vec3(0.8f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(colorLoc, 255.0f, 255.0f, 0.0f, 1.0f); 

  
  m_modelJuiz.render(KaLoc,KdLoc,KsLoc); 


  // time azul 
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(1.0f, 0.5f, 1.0f));  
  model = glm::scale(model, glm::vec3(0.6f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(colorLoc, 0.0f, 0.8f, 1.0f, 1.0f);  
  m_modelJogador.render(KaLoc,KdLoc,KsLoc); 
  
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(2.0f, 0.5f, 1.0f));  
  model = glm::scale(model, glm::vec3(0.6f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(colorLoc, 0.0f, 0.8f, 1.0f, 1.0f);   
  m_modelJogador.render(KaLoc,KdLoc,KsLoc); 
  

  // time verde
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-2.0f, 0.5f, 1.0f));  
  model = glm::scale(model, glm::vec3(0.6f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);  
  m_modelJogador.render(KaLoc,KdLoc,KsLoc);  
 
  model = glm::mat4(1.0);  
  model = glm::translate(model, glm::vec3(-1.0f, 0.5f, 1.0f));  
  model = glm::scale(model, glm::vec3(0.6f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);  
  m_modelJogador.render(KaLoc,KdLoc,KsLoc); 

  
  
}

void OpenGLWindow::paintUI() { 
  abcg::OpenGLWindow::paintUI();   

  // Create main window widget
  {
    auto widgetSize{ImVec2(220, 90)};

    if (!m_modelBola.isUVMapped()) {
      // Add extra space for static text
      widgetSize.y += 26;
    }

    ImGui::SetNextWindowPos(ImVec2(m_viewportWidth - widgetSize.x - 5, 5));
    ImGui::SetNextWindowSize(widgetSize);
    auto flags{ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDecoration};
    ImGui::Begin("Widget window", nullptr, flags);
    

    // Slider will be stretched horizontally
    ImGui::PushItemWidth(widgetSize.x - 16);
    
    ImGui::PopItemWidth();

   
    // CW/CCW combo box
    {
      static std::size_t currentIndex{};
      std::vector<std::string> comboItems{"CCW", "CW"};

      ImGui::PushItemWidth(120);
      if (ImGui::BeginCombo("Front face",
                            comboItems.at(currentIndex).c_str())) {
        for (auto index : iter::range(comboItems.size())) {
          const bool isSelected{currentIndex == index};
          if (ImGui::Selectable(comboItems.at(index).c_str(), isSelected))
            currentIndex = index;
          if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      ImGui::PopItemWidth();

      if (currentIndex == 0) {
        abcg::glFrontFace(GL_CCW);
      } else {
        abcg::glFrontFace(GL_CW);
      }
    }        

    if (!m_modelBola.isUVMapped()) {
      ImGui::TextColored(ImVec4(1, 1, 0, 1), "Mesh has no UV coords.");
    }

    // UV mapping box
    {
      std::vector<std::string> comboItems{"Triplanar", "Cylindrical",
                                          "Spherical"};

      if (m_modelBola.isUVMapped()) comboItems.emplace_back("From mesh");

      ImGui::PushItemWidth(120);
      if (ImGui::BeginCombo("UV mapping",
                            comboItems.at(m_mappingMode).c_str())) {
        for (auto index : iter::range(comboItems.size())) {
          const bool isSelected{m_mappingMode == static_cast<int>(index)};
          if (ImGui::Selectable(comboItems.at(index).c_str(), isSelected))
            m_mappingMode = index;
          if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      ImGui::PopItemWidth();
    }

    ImGui::End();
  }  
}

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

/*
  for (const auto& program : m_programs) {
    abcg::glDeleteProgram(program);
  }
*/
  abcg::glDeleteProgram(m_programLookat);  
  abcg::glDeleteProgram(m_programTexture);  
}

void OpenGLWindow::update() {
  const float deltaTime{static_cast<float>(getDeltaTime())};

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
}