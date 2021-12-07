#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <vector>

#include "abcg.hpp"
#include "camera.hpp"
#include "ground.hpp"
#include "model.hpp"


class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& ev) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:  
  GLuint m_program;
  //GLuint m_programTexture{};
  //GLuint m_programBlinnPhong{};
  //GLuint m_programPhong{};
  //GLuint m_programGouraud{};
  //GLuint m_programNormal{};
  //GLuint m_programDepth{};


  // Shaders
  std::vector<const char*> m_shaderNames{
      "normalmapping", "texture", "blinnphong", "phong",
      "gouraud",       "normal",  "depth"};
  std::vector<GLuint> m_programs;
  int m_currentProgramIndex{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  Model m_modelBola;
  Model m_modelJogador;
  Model m_modelAviao;
  Model m_modelJuiz;
  Model m_modelArvore;

  int m_trianglesToDraw{};

  Camera m_camera;
  float m_dollySpeed{0.0f};
  float m_truckSpeed{0.0f};
  float m_panSpeed{0.0f};

  glm::mat4 m_modelMatrix{1.0f};
  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};

  Ground m_ground;

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices; 

  SDL_AudioDeviceID m_deviceId;
  Uint8 *m_wavBuffer;    

  // Mapping mode
  // 0: triplanar; 1: cylindrical; 2: spherical; 3: from mesh
  int m_mappingMode{};

  // Light and material properties
  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka{};
  glm::vec4 m_Kd{};
  glm::vec4 m_Ks{};
  //float m_shininess{};


  void loadModelFromFile(std::string_view path);  
  void update();
  void initializeSound(std::string path);
};

#endif