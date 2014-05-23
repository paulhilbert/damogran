#ifndef DAMOGRAN_MATH_HPP_
#define DAMOGRAN_MATH_HPP_

#include "common.hpp"

namespace damogran {

template <typename T>
void clamp(T& value, const T& lower = T(0), const T& upper = T(1));

template <typename T>
T clamped(const T& value, const T& lower = T(0), const T& upper = T(1));

template <typename Scalar, int Rows, int Cols>
void clamp(Eigen::Matrix<Scalar, Rows, Cols>& value, const Scalar& lower = Scalar(0), const Scalar& upper = Scalar(1));

template <typename Scalar, int Rows, int Cols>
Eigen::Matrix<Scalar, Rows, Cols> clamped(const Eigen::Matrix<Scalar, Rows, Cols>& value, const Scalar& lower = Scalar(0), const Scalar& upper = Scalar(1));


#include "math.ipp"

} // damogran

#endif /* DAMOGRAN_MATH_HPP_ */
