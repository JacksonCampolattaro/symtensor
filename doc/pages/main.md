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

Symtensor is designed for working with tensors which are **Symmetric**, **Small** and **High-rank**, and for the composition of those tensors into Multipoles.

These are unusual needs for a tensor library. The design comes from symtensor's original purpose as part of an implementation of the Fast Multipole Method for an [_n_-body gravity simulation](https://github.com/JacksonCampolattaro/n-body). Popular tensor libraries like [GLM](https://github.com/g-truc/glm), [Eigen](https://github.com/libigl/eigen), and [Fastor](https://github.com/romeric/Fastor) are ruled out for this purpose because they don't support condensed storage of symmetric tensors.

There are a handful of projects which aim to solve the same problems as this one, including [Tensor](https://github.com/thenumbernine/Tensor), [GADGET-4's symmetric tensor library](https://github.com/weiguangcui/Gadget4/blob/Gadget4-Simba/src/data/symtensors.h), and [fmmgen](https://github.com/rpep/fmmgen). Tensor is feature-rich, but has a broader maths focus and doesn't provide benchmarks. GADGET-4's library has been proven to perform well for _n_-body simulations, but is ultimately limited by its handwritten operators -- it can't be used for high-order multipoles. Fmmgen uses a script to generate efficient multipole and symmetric tensor code which makes it very flexible, but the code it produces is based on C-style arrays and not intended to be human-readable.

The goal of this project is to produce a library which combines the modern sensibilities of _Tensor_ with the performance of _GADGET-4_'s library and the flexibility of _fmmgen_.

Eventually, I intend to incorporate symtensor into a larger _n_-body solver built from the ground up. To this end, [Christoph Peters](https://momentsingraphics.de/About.html) has been a big help by generating derivatives of Newton's gravity equation up to arbitrarily high orders.
