#ifndef DAMOGRAN_DISCRETIZED_ARRAY_HPP_
#define DAMOGRAN_DISCRETIZED_ARRAY_HPP_

#include <vector>
#include <memory>
#include <functional>
#include <Eigen/Dense>

#ifdef USE_CEREAL
#include "cereal.hpp"
#endif // USE_CEREAL

namespace damogran {


template <int D, typename T>
class discretized_array {
	public:
		typedef std::shared_ptr<discretized_array> ptr;
		typedef std::weak_ptr<discretized_array> wptr;
		typedef std::shared_ptr<const discretized_array> const_ptr;
		typedef std::weak_ptr<const discretized_array> const_wptr;
		typedef T value_type;
		typedef Eigen::Matrix<float, D, 1> vec_float;
		typedef Eigen::Matrix<int, D, 1> vec_int;
		typedef vec_float float_key_type;
		typedef vec_int key_type;

#ifdef USE_CEREAL
		friend class cereal::access;
#endif // USE_CEREAL

	public:
		discretized_array(const vec_int& bin_counts, const vec_float& lower_bounds, const vec_float& upper_bounds, const value_type& init_value = value_type());
		virtual ~discretized_array();

		const value_type& operator[](const float_key_type& float_key) const;
		value_type& operator[](const float_key_type& float_key);
		const value_type& operator[](const key_type& key) const;
		value_type& operator[](const key_type& key);

		key_type discretize(const float_key_type& desc) const;
		float_key_type interpolate(const key_type& desc, float interpolation_param = 0.f) const;

		template <typename Func>
		void forall_keys(Func&& func);

		template <typename Func>
		void forall_keys(Func&& func) const;

	protected:
		int index_of(const key_type& key) const;
		key_type key_of(int index) const;

#ifdef USE_CEREAL
		discretized_array();
		template <class Archive>
		void serialize(Archive& ar);
#endif // USE_CEREAL

	protected:
		vec_int   bin_counts_;
		vec_float lower_bounds_;
		vec_float upper_bounds_;
		value_type init_value_;
		vec_float deltas_;
		vec_int   offsets_;
		int       size_;
		std::vector<value_type> data_;
};

#include "discretized_array.ipp"

} // damogran

#endif /* DAMOGRAN_DISCRETIZED_ARRAY_HPP_ */
