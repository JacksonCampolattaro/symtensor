# Symmetric Tensors
#### What is a symmetric tensor?

A tensor is a generalization of vectors and matrices, which supports an arbitrary number of dimensions. The number of dimensions in a tensor is known as its "Rank". A vector is a tensor of rank 1, and may look like this: \f$ \langle v_0, v_1, v_2 \rangle \f$. A matrix of size \f$n \times m\f$ is a tensor of rank 2, and may look like this: 

\f[
\begin{bmatrix}
m_{00} & m_{01} & m_{02}\\
m_{10} & m_{11} & m_{12}\\
m_{20} & m_{21} & m_{22}
\end{bmatrix}
\f]

This library is meant for working with small tensors, where the indices have semantic meaning. For example, a vector is a natural representation for a cartesian coordinate: \f$ \langle p_x, p_y, p_z \rangle \f$. Higher order tensors can have semantic meaning as well, though less intuitive. For example, the derivative of a vector with respect to another vector produces the following:

\f[
\frac{\partial}{\partial \langle x, y, z \rangle }
\langle v_x, v_y, v_z \rangle
=
\begin{bmatrix}
\frac{\partial v_x}{\partial x} & \frac{\partial v_y}{\partial x} & \frac{\partial v_z}{\partial x}\\
\frac{\partial v_x}{\partial y} & \frac{\partial v_y}{\partial y} & \frac{\partial v_z}{\partial y}\\
\frac{\partial v_x}{\partial z} & \frac{\partial v_y}{\partial z} & \frac{\partial v_z}{\partial z}\\
\end{bmatrix}
=
\begin{bmatrix}
d_{xx} & d_{xy} & d_{xz}\\
d_{yx} & d_{yy} & d_{yz}\\
d_{zx} & d_{zy} & d_{zz}
\end{bmatrix}
\f]

The tensors in this library are also *symmetric*. Here, symmetric is defined such that in a rank-2 tensor, \f$q_{xy} = q_{yx}\f$. This means that a quadrupole symmetric tensor can be written without redundancy by only including the upper triangle, as shown here:

\f[
\begin{bmatrix}
q_{xx} & q_{xy} & q_{xz}\\
\cdot  & q_{yy} & q_{yz}\\
\cdot  & \cdot  & q_{zz}
\end{bmatrix}
\f]
