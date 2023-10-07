# Symmetric Tensors

### Tensors

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

Here, \f$d_{yx} = \frac{\partial v_x}{\partial y}\f$ is the derivative of the \f$x\f$ component of \f$v\f$ with respect to \f$y\f$, meaning the change in \f$v_x\f$ as you move along the \f$y\f$-axis. Each time we take the derivative with respect to a vector in this way, we add a new dimension the size of the vector, increasing the rank of the resulting tensor. The second derivative of our vector with respect to \f$x, y, z\f$ is a rank-3 tensor:

\f[
\frac{\partial}{\partial \langle x, y, z \rangle }
\begin{bmatrix}
d_{xx} & d_{xy} & d_{xz}\\
d_{yx} & d_{yy} & d_{yz}\\
d_{zx} & d_{zy} & d_{zz}
\end{bmatrix}
=
\begin{bmatrix}
    \begin{bmatrix}
    d_{xxx} & d_{xxy} & d_{xxz}\\
    d_{xyx} & d_{xyy} & d_{xyz}\\
    d_{xzx} & d_{xzy} & d_{xzz}
    \end{bmatrix}
    \begin{bmatrix}
    d_{yxx} & d_{yxy} & d_{yxz}\\
    d_{yyx} & d_{yyy} & d_{yyz}\\
    d_{yzx} & d_{yzy} & d_{yzz}
    \end{bmatrix}
    \begin{bmatrix}
    d_{zxx} & d_{zxy} & d_{zxz}\\
    d_{zyx} & d_{zyy} & d_{zyz}\\
    d_{zzx} & d_{zzy} & d_{zzz}
    \end{bmatrix}
\end{bmatrix}
\f]

### Symmetry

The tensors in this library are also *symmetric*. Here, symmetric is defined such that in a rank-2 tensor, \f$q_{xy} = q_{yx}\f$. This means that a quadrupole symmetric tensor can be written without redundancy by only including the upper triangle, as shown here:

\f[
\begin{bmatrix}
t_{xx} & t_{xy} & t_{xz}\\
\cdot  & t_{yy} & t_{yz}\\
\cdot  & \cdot  & t_{zz}
\end{bmatrix}
\f]

Most matrix and tensor libraries treat symmetric matrices as a special case of their regular matrix type. For example, Eigen provides the [`SelfAdjointView`](https://eigen.tuxfamily.org/dox/classEigen_1_1SelfAdjointView.html) for operating on regular matrices as though they are symmetric and Numpy provides methods like [`triu()`](https://numpy.org/doc/stable/reference/generated/numpy.triu.html) which zeroes all but the upper triangle of the matrix. This is a reasonable choice due to the good memory access patterns of operations on rectangular matrices, but for small tensors the advantage becomes less relevant. When storing a large number of symmetric tensors another factor begins to dominate: storage redundancy.

Our \f$3 \times 3 \f$ symmetric tensor is only made 3 values smaller by not storing the lower triangle, but eliminating this redundancy become more important as the rank increases:

\f[
\begin{bmatrix}
    \begin{bmatrix}
    t_{xxx} & t_{xxy} & t_{xxz}\\
    \cdot   & t_{xyy} & t_{xyz}\\
    \cdot   & \cdot   & t_{xzz}
    \end{bmatrix}
    \begin{bmatrix}
    \cdot   & \cdot   & \cdot  \\
    \cdot   & t_{yyy} & t_{yyz}\\
    \cdot   & \cdot   & t_{yzz}
    \end{bmatrix}
    \begin{bmatrix}
    \cdot   & \cdot   & \cdot  \\
    \cdot   & \cdot   & \cdot  \\
    \cdot   & \cdot   & t_{zzz}
    \end{bmatrix}
\end{bmatrix}
\f]

For a \f$3 \times 3 \times 3 \f$ tensor we can store 10 values instead of 27, and for a rank-4 tensor we can store 15 values instead of 81.
