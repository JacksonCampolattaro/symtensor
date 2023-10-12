<picture>
    <source srcset="doc/images/symtensor-dark.svg"  media="(prefers-color-scheme: dark)">
    <img src="doc/images/symtensor.svg" alt="symtensor logo">
</picture>

![unit tests](https://img.shields.io/github/actions/workflow/status/JacksonCampolattaro/symtensor/unit-tests-multi-platform.yml?logo=github&label=Unit%20tests) 
![MIT license](https://img.shields.io/badge/license-MIT-A51931) 
![C++20](https://img.shields.io/badge/standard-C%2B%2B20-blue?logo=cplusplus)

Symtensor is designed for working with tensors which are...
- **Symmetric** — only the upper triangle needs to be stored.
- **Small** — indices have semantic meaning, similar to GLM's `mat3x3` type.
- **High-rank** — not just vectors and matrices, these tensors can have any number of dimensions.

These are unusual needs for a tensor library. [GLM](https://github.com/g-truc/glm), [Eigen](https://github.com/libigl/eigen), and [Fastor](https://github.com/romeric/Fastor) each have their own advantages, but none satisfy more than 2 of these requirements.
