# Real-Time Rasterizer

## CPSC387: Computer Graphics

![scene_demo](https://github.com/user-attachments/assets/88b186a5-32e4-462b-bc7a-849b7bd57cba)

---

## Overview

This project is the culminating experience of my computer graphics course at Hiram College.  
It integrates and extends algorithms and techniques implemented throughout the semester into a single, interactive 3D graphics application. The program implements a **real-time rasterization pipeline** using modern OpenGL (Core Profile 3.3).

### Features

* Real-time Phong and model shaders
* Model loading and rendering
* Geometry rendering
* Dynamic lighting toggle and interactive camera controls

---

## Dependencies

| Library / Resource | Source / Documentation |
|---------------------|------------------------|
| **GLFW** | [https://www.glfw.org](https://www.glfw.org) |
| **GLAD** | [https://glad.dav1d.de](https://glad.dav1d.de) |
| **GLM** | [https://github.com/g-truc/glm](https://github.com/g-truc/glm) |
| **Assimp** | [https://github.com/assimp/assimp](https://github.com/assimp/assimp) |
| **CMake** | [https://cmake.org/download/](https://cmake.org/download/) |

---

## Tutorial References

The structure and implementation draw inspiration from the following LearnOpenGL chapters:

| My Implementation | Code Used |
|--------|------------------|
| Basic Lighting | \[https://learnopengl.com/Lighting/Basic-Lighting](https://learnopengl.com/Lighting/Basic-Lighting) |
| Phong Lighting | \[https://learnopengl.com/Lighting/Materials](https://learnopengl.com/Lighting/Materials) |
| Model Loading | \[https://learnopengl.com/Model-Loading/Model](https://learnopengl.com/Model-Loading/Model) |
| Camera Controls | \[https://learnopengl.com/Getting-started/Camera](https://learnopengl.com/Getting-started/Camera) |
| Textures | \[https://learnopengl.com/Getting-started/Textures](https://learnopengl.com/Getting-started/Textures) |

---

## Build Instructions

1. Clone or download this repository
2. Open the Visual Studio solution (`.sln` file)
3. Build the project in **x64 Release** or **Debug** configuration
4. Run the executable to launch the 3D scene

---

## Controls

| Key | Action |
|------|--------|
| `W/A/S/D` | Move camera |
| `Mouse` | Look around |
| `Esc` | Exit application |
| `L` | Toggle light on/off |

---

## Acknowledgments

This project would not have been possible without the following open-source resources and tutorials:

* **LearnOpenGL by Joey de Vries** — [https://learnopengl.com/](https://learnopengl.com/)
* **Song Ho’s OpenGL Tutorials** — [https://www.songho.ca/opengl/](https://www.songho.ca/opengl/)
* **GLFW, GLAD, GLM, and Assimp** open-source communities

