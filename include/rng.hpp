#ifndef DAMOGRAN_RNG_H_
#define DAMOGRAN_RNG_H_

#include <random>
#include <memory>
#include <functional>
#include <type_traits>


namespace damogran {

class rng {
	public:
		typedef std::mt19937 internal_generator_t;

		template <class T>
		struct traits_t {
			// uniform_01
			typedef std::uniform_real_distribution<T> dist_01_t;
			typedef typename std::conditional< std::is_floating_point<T>::value, std::uniform_real_distribution<T>, std::uniform_int_distribution<T> >::type dist_ab_t;
			typedef std::geometric_distribution<T> dist_geom_t;
			typedef std::normal_distribution<T> dist_normal_t;

			static T upper_bound(T original, std::true_type is_float) { return original; }
			static T upper_bound(T original, std::false_type is_float) { return original-1; }

			typedef std::function<T ()> generator_t;
		};

	public:
		template <class T>
		static T uniform_01();

		template <class T>
		static T uniform_ab(T a, T b);

		template <class T>
		static T geometric(float p);

		template <class T>
		static T normal(T mean = 0.f, T std_dev = 1.f);

		template <class T>
		static typename traits_t<T>::generator_t uniform_01_gen(bool deterministic = false);

		template <class T>
		static typename traits_t<T>::generator_t uniform_ab_gen(T a, T b, bool deterministic = false);

		template <class T>
		static typename traits_t<T>::generator_t geometric_gen(float p, bool deterministic = false);

		template <class T>
		static typename traits_t<T>::generator_t normal_gen(T mean = 0.f, T std_dev = 1.f, bool deterministic = false);

	protected:
		static internal_generator_t generator(bool deterministic);
};


} // damogran


#endif /* DAMOGRAN_RNG_H_ */
