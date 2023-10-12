@mainpage symtensor
@image html images/symtensor.svg

@m_div{m-text-center}
*A templated C++ library for working with small symmetric tensors of arbitrary rank.*
@m_enddiv


@m_div{m-text-center}
![unit tests](https://img.shields.io/github/actions/workflow/status/JacksonCampolattaro/symtensor/unit-tests-multi-platform.yml?logo=github&label=Unit%20tests)
![MIT license](https://img.shields.io/badge/license-MIT-A51931)
![C++20](https://img.shields.io/badge/standard-C%2B%2B20-blue?logo=cplusplus)
@m_enddiv


[//]: # (![unit tests]&#40;https://img.shields.io/github/actions/workflow/status/JacksonCampolattaro/symtensor/unit-tests-multi-platform.yml?logo=github&label=Unit%20tests&#41; [![MIT license]&#40;https://img.shields.io/badge/license-MIT-purple&#41;]&#40;https://github.com/JacksonCampolattaro/symtensor/blob/main/LICENSE&#41; ![C++20]&#40;https://img.shields.io/badge/standard-C%2B%2B20-blue?logo=cplusplus&#41;)

Symtensor is designed for working with tensors which are...
- **Symmetric** — only the upper triangle needs to be stored.
- **Small** — indices have semantic meaning, similar to GLM's `mat3x3` type.
- **High-rank** — not just vectors and matrices, these tensors can have any number of dimensions.

These are unusual needs for a tensor library. [GLM](https://github.com/g-truc/glm), [Eigen](https://github.com/libigl/eigen), and [Fastor](https://github.com/romeric/Fastor) each have their own advantages, but none satisfy more than 2 of these requirements.
