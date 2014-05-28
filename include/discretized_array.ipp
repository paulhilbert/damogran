template <int D, typename T>
inline discretized_array<D,T>::discretized_array(const vec_int& bin_counts, const vec_float& lower_bounds, const vec_float& upper_bounds, const value_type& init_value) : bin_counts_(bin_counts), lower_bounds_(lower_bounds), upper_bounds_(upper_bounds), init_value_(init_value) {
	if ((bin_counts.array() <= vec_int::Zero().array()).any()) throw std::runtime_error("Could not construct discretized_array: Bin counts must be positive (>0).");
	deltas_ = (upper_bounds - lower_bounds).cwiseQuotient(bin_counts.template cast<float>());
	offsets_ = vec_int::Constant(1);
	for (int i = 2; i <= D; ++i) {
		offsets_[D-i] = bin_counts[D-i+1] * offsets_[D-i+1];
	}
	size_ = bin_counts.prod();
	//for (int i = 0; i < D; ++i) {
		//size_ *= bin_counts[i];
	//}
	data_.resize(size_, init_value);
}

template <int D, typename T>
inline discretized_array<D,T>::~discretized_array() {
}

template <int D, typename T>
inline const typename discretized_array<D,T>::value_type& discretized_array<D,T>::operator[](const float_key_type& float_key) const {
	return this->operator[](discretize(float_key));
}

template <int D, typename T>
inline typename discretized_array<D,T>::value_type& discretized_array<D,T>::operator[](const float_key_type& float_key) {
	typename discretized_array<D,T>::value_type& result = this->operator[](discretize(float_key));
	return result;
}

template <int D, typename T>
inline const typename discretized_array<D,T>::value_type& discretized_array<D,T>::operator[](const key_type& key) const {
	return data_[index_of(key)];
}

template <int D, typename T>
inline typename discretized_array<D,T>::value_type& discretized_array<D,T>::operator[](const key_type& key) {
	return data_[index_of(key)];
}

template <int D, typename T>
inline typename discretized_array<D,T>::key_type discretized_array<D,T>::discretize(const float_key_type& float_key) const {
	return (float_key - lower_bounds_).cwiseQuotient(deltas_).template cast<int>().cwiseMin(bin_counts_ - vec_int::Constant(1));
}

template <int D, typename T>
template <typename Func>
inline void discretized_array<D,T>::forall_keys(Func&& func) {
	for (int i = 0; i < size_; ++i) {
		func(key_of(i));
	}
}

template <int D, typename T>
template <typename Func>
inline void discretized_array<D,T>::forall_keys(Func&& func) const {
	for (int i = 0; i < size_; ++i) {
		func(key_of(i));
	}
}

template <int D, typename T>
inline int discretized_array<D,T>::index_of(const key_type& key) const {
	return key.dot(offsets_);
}

#ifdef USE_CEREAL
template <int D, typename T>
discretized_array<D,T>::discretized_array() {
}
template <int D, typename T>
template <class Archive>
void discretized_array<D,T>::serialize(Archive& ar) {
	ar(cereal::make_nvp("bin_counts", bin_counts_),
		cereal::make_nvp("lower_bounds", lower_bounds_),
		cereal::make_nvp("upper_bounds", upper_bounds_),
		cereal::make_nvp("init_value", init_value_),
		cereal::make_nvp("deltas", deltas_),
		cereal::make_nvp("offsets", offsets_),
		cereal::make_nvp("size", size_),
		cereal::make_nvp("data", data_));
}
#endif // USE_CEREAL

template <int D, typename T>
inline typename discretized_array<D,T>::key_type discretized_array<D,T>::key_of(int index) const {
	key_type key;
	for (int i=0; i<D; ++i) {
		key[i] = index / offsets_[i];
		index %= offsets_[i];
	}
	return key;
}
