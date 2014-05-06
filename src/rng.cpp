#include <rng.hpp>
#include <macros.hpp>

#include <chrono> // for time seeding

namespace damogran {


template <class T>
T rng::uniform_01() {
	auto gen = uniform_01_gen<T>();
	return gen();
}

template <class T>
T rng::uniform_ab(T a, T b) {
	auto gen = uniform_ab_gen<T>(a, b);
	return gen();
}

template <class T>
T rng::geometric(float p) {
	auto gen = geometric_gen<T>(p);
	return gen();
}

template <class T>
T rng::normal(T mean, T std_dev) {
	auto gen = normal_gen<T>(mean, std_dev);
	return gen();
}

template <class T>
typename rng::traits_t<T>::generator_t rng::uniform_01_gen(bool deterministic) {
	auto gen = generator(deterministic);
	typename traits_t<T>::dist_01_t dist(T(0), T(1));
	return std::bind(dist, gen);
}

template <class T>
typename rng::traits_t<T>::generator_t rng::uniform_ab_gen(T a, T b, bool deterministic) {
	// uniform_real gives [a,b) and uniform_int [a,b];
	// upper_bound(b, ...) is b-1 for is_floating_point<T> == false_type
	auto gen = generator(deterministic);
	typename traits_t<T>::dist_ab_t dist( a, traits_t<T>::upper_bound(b, std::is_floating_point<T>()) );
	return std::bind(dist, gen);
}

template <class T>
typename rng::traits_t<T>::generator_t rng::geometric_gen(float p, bool deterministic) {
	auto gen = generator(deterministic);
	typename traits_t<T>::dist_geom_t dist(p);
	return std::bind(dist, gen);
}

template <class T>
typename rng::traits_t<T>::generator_t rng::normal_gen(T mean, T std_dev, bool deterministic) {
	auto gen = generator(deterministic);
	typename traits_t<T>::dist_normal_t dist(mean, std_dev);
	return std::bind(dist, gen);
}

rng::internal_generator_t rng::generator(bool deterministic) {
	internal_generator_t gen;
	if (!deterministic) {
		unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
		gen.seed(seed);
	}
	return gen;
}


// instantiate for numeric types
#define X(type) \
	template type rng::uniform_ab<type>(type a, type b); \
	template typename rng::traits_t<type>::generator_t rng::uniform_ab_gen<type>(type a, type b, bool deterministic);
COMMON_NUMERIC_TYPES
#undef X

// instantiate for integral types
#define X(type) \
	template type rng::geometric<type>(float p); \
	template typename rng::traits_t<type>::generator_t rng::geometric_gen<type>(float p, bool deterministic);
INTEGRAL_NUMERIC_TYPES
#undef X

// instantiate for floating point types
#define X(type) \
	template type rng::uniform_01<type>(); \
	template type rng::normal<type>(type mean, type std_dev); \
	template typename rng::traits_t<type>::generator_t rng::uniform_01_gen<type>(bool deterministic); \
	template typename rng::traits_t<type>::generator_t rng::normal_gen<type>(type mean, type std_dev, bool deterministic);
REAL_NUMERIC_TYPES
#undef X


} // damogran
