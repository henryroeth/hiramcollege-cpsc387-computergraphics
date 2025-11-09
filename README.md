# Real-Time Rasterizer

## CPSC387: Computer Graphics

This project is the culminating experience of my computer graphics course at Hiram College.  
It integrates and extends algorithms and techniques implemented throughout the semester into a single, interactive 3D graphics application.

!\[scene\_demo](https://github.com/user-attachments/assets/d4cc1d68-6518-477f-98c9-4bc56f0e44c7)

---

## Overview

This program implements a **real-time rasterization pipeline** using modern OpenGL (Core Profile 3.3).  
It features object rendering with **Phong illumination**, **texture mapping**, and **model loading via Assimp**.

### Features

* Real-time Phong shading with adjustable light parameters
* Model loading and rendering using the [Assimp](https://github.com/assimp/assimp) library
* Textured and procedurally generated geometry (e.g., cube, sphere, plane)
* Dynamic lighting toggle and interactive camera controls
* Clean, modular shader and object management system

---

## Code Structure

| Folder / File | Description |
|----------------|-------------|
| `main.cpp` | Program entry point. Handles OpenGL initialization, main render loop, and user input. |
| `shader\_s.h` | Wrapper class for compiling, linking, and using GLSL shaders. |
| `camera.h` | FPS-style camera implementation for interactive navigation. |
| `model.h` / `mesh.h` | Object loading and drawing via Assimp. |
| `sphere.h` | Sphere generation based on parametric geometry (adapted from Song Ho). |
| `resources/` | Contains textures, models, and shader source files. |
| `lib/` | External libraries (GLFW, GLAD, GLM, Assimp). |

---

## Dependencies

This project uses the following libraries and tutorials:

| Library / Resource | Source / Documentation |
|---------------------|------------------------|
| \*\*GLFW\*\* | \[https://www.glfw.org/](https://www.glfw.org/) |
| \*\*GLAD\*\* | \[https://glad.dav1d.de/](https://glad.dav1d.de/) |
| \*\*GLM\*\* | \[https://github.com/g-truc/glm](https://github.com/g-truc/glm) |
| \*\*Assimp\*\* | \[https://github.com/assimp/assimp](https://github.com/assimp/assimp) |
| \*\*Song Ho’s Sphere Geometry\*\* | \[https://www.songho.ca/opengl/gl\_sphere.html](https://www.songho.ca/opengl/gl\_sphere.html) |
| \*\*LearnOpenGL Tutorials\*\* | \[https://learnopengl.com/](https://learnopengl.com/) (used extensively for structure, shading, and rendering examples) |

---

## Key Tutorial References

The structure and implementation draw inspiration from the following LearnOpenGL chapters:

| Topic | LearnOpenGL Link |
|--------|------------------|
| Basic Lighting | \[https://learnopengl.com/Lighting/Basic-Lighting](https://learnopengl.com/Lighting/Basic-Lighting) |
| Phong Lighting | \[https://learnopengl.com/Lighting/Materials](https://learnopengl.com/Lighting/Materials) |
| Model Loading | \[https://learnopengl.com/Model-Loading/Model](https://learnopengl.com/Model-Loading/Model) |
| Camera Controls | \[https://learnopengl.com/Getting-started/Camera](https://learnopengl.com/Getting-started/Camera) |
| Textures | \[https://learnopengl.com/Getting-started/Textures](https://learnopengl.com/Getting-started/Textures) |

---

## Build Instructions

1. Clone or download this repository.
2. Make sure you have the required libraries located under `lib/`:

   * `glfw-3.4.bin.WIN64`
   * `glad`
   * `glm`
   * `assimp`

3. Open the Visual Studio solution (`.sln` file).
4. Build the project in **x64 Release** or **Debug** configuration.
5. Run the executable to launch the 3D scene.

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

---

## License

This project is for educational purposes only.  
External libraries and referenced tutorial code retain their respective licenses.

