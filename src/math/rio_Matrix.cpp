#include <math/rio_Matrix.h>

namespace rio {

template <>
const Matrix34<f32> Matrix34<f32>::zero{
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0
};

template <>
const Matrix34<f32> Matrix34<f32>::ident{
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0
};

template <>
const Matrix44<f32> Matrix44<f32>::zero{
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0
};

template <>
const Matrix44<f32> Matrix44<f32>::ident{
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

}
