3D voxel engine programmed in C++ and GLSL using OpenGL. 
<img width="1917" height="1076" alt="image" src="https://github.com/user-attachments/assets/751c28e9-eafd-40ae-92e6-5090ee18d945" />

# Features
- Procedurally generated terrain using perlin noise.
- Infinite world that gets loaded in as you travel through the world
- Day and night cycle
<img width="1916" height="1076" alt="image" src="https://github.com/user-attachments/assets/2a97925c-aa62-4e11-8943-6632fef7bac7" />

# Graphics Features
- Screen Space ambient occlusion (SSAO)
<img width="1913" height="1079" alt="image" src="https://github.com/user-attachments/assets/af854c18-9b57-4786-aa00-8ef78e6f7ca7" />

- Physics Based Rendering (PBR)
PBR:
<img width="1915" height="1077" alt="image" src="https://github.com/user-attachments/assets/21f50fe6-e27c-46c2-89d3-c7e964a25edc" />
Non-PBR:
<img width="1913" height="1074" alt="image" src="https://github.com/user-attachments/assets/95d10600-cc14-4f1f-adec-759cca85bc5d" />
- Ray casting for shadows using DDA algorithm
<img width="1915" height="1073" alt="image" src="https://github.com/user-attachments/assets/3400da07-d12d-4cbd-b758-9c654590957b" />

# Controls
- WASD to move around
- Hold right mouse button to change camera perspective using mouse
- 1, 2, 3 will change rate of time, i.e. faster day/night cycles
- Space to switch between PBR and non-PBR rendering.

# Additional Dependencies
- glad
- GLFW
- glm
- stb_image
