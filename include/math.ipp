template <typename T>
inline void clamp(T& value, const T& lower, const T& upper) {
	if (value < lower) value = lower;
	if (value > upper) value = upper;
}

template <typename T>
inline T clamped(const T& value, const T& lower, const T& upper) {
	T result(value);
	clamp(result, lower, upper);
	return result;
}

template <typename Scalar, int Rows, int Cols>
inline void clamp(Eigen::Matrix<Scalar, Rows, Cols>& value, const Scalar& lower, const Scalar& upper) {
	value = value.cwiseMin(upper);
	value = value.cwiseMax(lower);
}

template <typename Scalar, int Rows, int Cols>
inline Eigen::Matrix<Scalar, Rows, Cols> clamped(const Eigen::Matrix<Scalar, Rows, Cols>& value, const Scalar& lower, const Scalar& upper) {
	Eigen::Matrix<Scalar, Rows, Cols> result(value);
	clamp(result, lower, upper);
	return result;
}
