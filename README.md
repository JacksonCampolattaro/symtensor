# symtensor
*A templated C++ library for working with small symmetric tensors of arbitrary rank.*

Symtensor is designed for working with tensors which are...
- **Symmetric** — only the upper triangle needs to be stored.
- **Small** — indices have semantic meaning, similar to GLM's `mat3x3` type.
- **High-rank** — not just vectors and matrices, these tensors can have any number of dimensions.

These are unusual needs for a tensor library. [GLM](https://github.com/g-truc/glm), [Eigen](https://github.com/libigl/eigen), and [Fastor](https://github.com/romeric/Fastor) are all fantastic in their own right and each has its own advantages, but none satisfy more than 2 of these requirements.