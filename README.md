# BMX
## About
**Block Mixture Language** is *lite* data storage language, which provides a easy but flexiable way for you to organize data.

This repo is a **C++** implementation of **BMX** praser. And, welcome everybody to do works on binding **BMX** to other languages.

## Example
**BMX** was originally created to allow me writting vertex shader and fragment shader in one `.shader` file. Here is an example of using **BMX** for shader usage.

- basic_shader.shader

```ini
@about: basic-shader
@author: AnpyD

[ vertex ]
#version 460
layout (location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos, 1.0);
}

[ fragment ]
#version 460
out vec4 FragColor;

void main() {
    FragColor = vec4(0.5, 0.3, 0.2, 1.0);
}
```

- main.cpp
```cpp
#include <bmx/bmx.h>
#include <fstream>

int main() {
    std::fstream shader_file("basic_shader.shader");
    BMX::Data shader_data = BMX::loads(shader_file);
    shader_file.close();

    std::string vertex_code = shader_data.blocks["vertex"];
    std::string frag_code = shader_data.blocks["fragment"];

    return 0;
}
```

## Document
Read [Document](https://github.com/AnpyDX/BMX/blob/main/document.md) for more details and guides.

## Thanks
**Ascii-icon** of **BMX** was created by [network-science](http://www.network-science.de/ascii/).

## License
Licensed under the MIT license, see `LICENSE` for details.
