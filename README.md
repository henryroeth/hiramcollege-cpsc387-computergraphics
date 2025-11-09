# Real-Time Rasterizer

## CPSC387: Computer Graphics

![scene_demo](https://github.com/user-attachments/assets/88b186a5-32e4-462b-bc7a-849b7bd57cba)

---

## Overview

This project is the culminating experience of my computer graphics course at Hiram College. It integrates and extends algorithms and techniques learned throughout the semester into a single, interactive 3D graphics application. The program implements a real-time rasterization pipeline using modern OpenGL (Core Profile 3.3). ALL of the boilerplate code defining classes, structures, and functions comes directly from the tutorials and resources referenced in this project—untouched. I edited the existing code structure that came directly from the LearnOpenGL tutorial and Song Ho’s OpenGL sphere tutorial. I simply added things like another light source to the Phong refleciton fragment shader source (GLSL), an on/off light toggle input, and other minor variable and structural changes to render all of the componenets into one scene. Otherwise, the entire project followed these workshops almost identically. I was able to explore the graphics concepts I learned throughout the semester and apply them in a simple rendering pipeline.

---

## Features

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
* **Song Ho’s OpenGL Tutorials** — [https://www.songho.ca/opengl/gl_sphere.html](https://www.songho.ca/opengl/gl_sphere.html)
* **GLFW, GLAD, GLM, and Assimp** open-source communities

