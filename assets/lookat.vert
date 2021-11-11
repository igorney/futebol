#version 410

layout(location = 0) in vec3 inPosition;
layout(location = 0) in vec3 inPosition1;

uniform vec4 color;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform mat4 modelMatrix1;
uniform mat4 viewMatrix1;
uniform mat4 projMatrix1;

out vec4 fragColor;

void main() {
  vec4 posEyeSpace = viewMatrix * modelMatrix * vec4(inPosition, 1);
  vec4 posEyeSpace1 = viewMatrix1 * modelMatrix1 * vec4(inPosition1, 1);

  float i = 1.0 - (-posEyeSpace.z / 5.0);
  fragColor = vec4(i, i, i, 1) * color;

  gl_Position = projMatrix * posEyeSpace;
}