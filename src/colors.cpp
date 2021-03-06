/*
 * damogran - c++ opengl wrapper library
 *
 * Written in 2014 by Richard Vock
 *
 * To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide.
 * This software is distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication along with this software.
 * If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
 *
 */

#include <colors.hpp>

#include <rng.hpp>

#include <chrono>

namespace damogran {


template <typename Scalar> struct color_limits_t;

template <>
struct color_limits_t<float> {
	static constexpr float lower = 0.f;
	static constexpr float upper = 1.f;
};

template <>
struct color_limits_t<double> {
	static constexpr double lower = 0.0;
	static constexpr double upper = 1.0;
};

template <>
struct color_limits_t<short> {
	static constexpr short lower = 0;
	static constexpr short upper = 65535;
	static constexpr float rnd_upper = 1.f;
};

template <>
struct color_limits_t<int> {
	static constexpr int lower = 0;
	static constexpr int upper = 65535;
};

template <>
struct color_limits_t<unsigned int> {
	static constexpr unsigned int lower = 0;
	static constexpr unsigned int upper = 65535;
};

template <>
struct color_limits_t<unsigned char> {
	static constexpr unsigned char lower = 0;
	static constexpr unsigned char upper = 255;
};

template <>
struct color_limits_t<char> {
	static constexpr char lower = 0;
	static constexpr char upper = 127;
};

template <typename Scalar>
RGB<Scalar>::RGB() : col_vec3_t<Scalar>() {
}

template <typename Scalar>
RGB<Scalar>::RGB(Scalar r, Scalar g, Scalar b) : col_vec3_t<Scalar>(r,g,b) {
}

template <typename Scalar>
RGB<Scalar>::RGB(const RGB<Scalar>& rgb) : col_vec3_t<Scalar>(rgb) {
}

template <typename Scalar>
RGB<Scalar>::RGB(const RGBA<Scalar>& rgba) : RGB<Scalar>(rgba.r(), rgba.g(), rgba.b()) {
}

template <typename Scalar>
RGB<Scalar>::RGB(const HSV<Scalar>& hsv) {
	float lower = static_cast<float>(color_limits_t<Scalar>::lower);
	float range = static_cast<float>(color_limits_t<Scalar>::upper - color_limits_t<Scalar>::lower);

	Eigen::Vector3f vec = hsv.head(3).template cast<float>();
  	vec -= Eigen::Vector3f::Constant(lower);
  	vec /= static_cast<float>(range);
	float a = vec[0] / (static_cast<float>(M_PI) / 3.f);
	float c = std::floor(a);
	float f = a - c;
	float p = vec[2] * (1-vec[1]);
	float q = vec[2] * (1-vec[1]*f);
	float t = vec[2] * (1-vec[1]*(1-f));

	float r, g, b;

	if (c == 1) {
		r = q; g = vec[2]; b = p;
	} else if (c == 2) {
		r = p; g = vec[2]; b = t;
	} else if (c == 3) {
		r = p; g = q; b = vec[2];
	} else if (c == 4) {
		r = t; g = p; b = vec[2];
	} else if (c == 5) {
		r = vec[2]; g = p; b = q;
	} else {
		r = vec[2]; g = t; b = p;
	}

	Eigen::Vector3f rgb(r, g, b);
	rgb = rgb * range + Eigen::Vector3f::Constant(lower);
	(*this) = RGB<Scalar>(Scalar(rgb[0]), Scalar(rgb[1]), Scalar(rgb[2]));
}

template <typename Scalar>
RGB<Scalar>::RGB(const HSVA<Scalar>& hsva) : RGB<Scalar>(HSV<Scalar>(hsva)) {
}

template <typename Scalar>
RGB<Scalar>::~RGB() {
}

template <typename Scalar>
RGBA<Scalar>::RGBA() : col_vec4_t<Scalar>() {
}

template <typename Scalar>
RGBA<Scalar>::RGBA(Scalar r, Scalar g, Scalar b, Scalar a) : col_vec4_t<Scalar>(r, g, b, a) {
}

template <typename Scalar>
RGBA<Scalar>::RGBA(const RGB<Scalar>& rgb) {
	(*this).head(3) = rgb;
	(*this)[3] = Scalar(1);
}

template <typename Scalar>
RGBA<Scalar>::RGBA(const RGBA<Scalar>& rgba) : col_vec4_t<Scalar>(rgba) {
}

template <typename Scalar>
RGBA<Scalar>::RGBA(const HSV<Scalar>& hsv) : RGBA<Scalar>(RGB<Scalar>(hsv)) {
}

template <typename Scalar>
RGBA<Scalar>::RGBA(const HSVA<Scalar>& hsva) : RGBA<Scalar>(HSV<Scalar>(hsva)) {
	(*this)[3] = hsva.a();
}

template <typename Scalar>
RGBA<Scalar>::~RGBA() {
}

template <typename Scalar>
HSV<Scalar>::HSV() : col_vec3_t<Scalar>() {
}

template <typename Scalar>
HSV<Scalar>::HSV(Scalar h, Scalar s, Scalar v) : col_vec3_t<Scalar>(h, s, v) {
}

template <typename Scalar>
HSV<Scalar>::HSV(const RGB<Scalar>& rgb) {
	Eigen::Vector3f vec = rgb.head(3).template cast<float>();
	float lower = static_cast<float>(color_limits_t<Scalar>::lower);
	float range = static_cast<float>(color_limits_t<Scalar>::upper - color_limits_t<Scalar>::lower);
	vec = (vec - Eigen::Vector3f::Constant(lower)) / range;
	Scalar min = std::min(std::min(vec[0], vec[1]), vec[2]);
	Scalar max = std::max(std::max(vec[0], vec[1]), vec[2]);

	float h;
	if (min == max) {
		h = 0.f;
	} else if (max == vec[0]) {
		h = static_cast<float>(M_PI) / 3.f * ((vec[1]-vec[2]) / (max-min));
	} else if (max == vec[1]) {
		h = static_cast<float>(M_PI) / 3.f * (2.f + (vec[2]-vec[0]) / (max-min));
	} else {
		h = static_cast<float>(M_PI) / 3.f * (4.f + (vec[0]-vec[1]) / (max-min));
	}
	if (h < 0.0) {
		h += static_cast<float>(M_PI) * 2.f;
	}

	float s = max == 0.f ? 0.f : (max-min) / max;
	float v = max;

	vec = Eigen::Vector3f(h, s, v) * range + Eigen::Vector3f::Constant(lower);
	(*this) = HSV(Scalar(vec[0]), Scalar(vec[1]), Scalar(vec[2]));
}

template <typename Scalar>
HSV<Scalar>::HSV(const RGBA<Scalar>& rgba) : HSV<Scalar>(RGB<Scalar>(rgba)) {
}

template <typename Scalar>
HSV<Scalar>::HSV(const HSV<Scalar>& hsv) : col_vec3_t<Scalar>(hsv) {
}

template <typename Scalar>
HSV<Scalar>::HSV(const HSVA<Scalar>& hsva) : col_vec3_t<Scalar>(hsva.head(3)) {
}

template <typename Scalar>
HSV<Scalar>::~HSV() {
}

template <typename Scalar>
HSVA<Scalar>::HSVA() : col_vec4_t<Scalar>() {
}

template <typename Scalar>
HSVA<Scalar>::HSVA(Scalar h, Scalar s, Scalar v, Scalar a) : col_vec4_t<Scalar>(h, s, v, a) {
}

template <typename Scalar>
HSVA<Scalar>::HSVA(const RGB<Scalar>& rgb) : HSVA<Scalar>(HSV<Scalar>(rgb)) {
	(*this)[3] = Scalar(1);
}

template <typename Scalar>
HSVA<Scalar>::HSVA(const RGBA<Scalar>& rgba) : HSVA<Scalar>(HSV<Scalar>(rgba)) {
	(*this)[3] = rgba.a();
}

template <typename Scalar>
HSVA<Scalar>::HSVA(const HSV<Scalar>& hsv) {
	(*this).head(3) = hsv;
	(*this)[3] = Scalar(1);
}

template <typename Scalar>
HSVA<Scalar>::HSVA(const HSVA<Scalar>& hsva) : col_vec4_t<Scalar>(hsva) {
}

template <typename Scalar>
HSVA<Scalar>::~HSVA() {
}

template <class ColorType>
typename generate<ColorType>::color_type generate<ColorType>::random_hue(scalar_type value, scalar_type saturation, scalar_type alpha) {
	auto lower = color_limits_t<scalar_type>::lower;
	auto upper = color_limits_t<scalar_type>::upper;
	return color_type(HSVA<scalar_type>(rng::uniform_ab<scalar_type>(lower, upper), value, saturation, alpha));
}

template <class ColorType>
std::vector<typename generate<ColorType>::color_type> generate<ColorType>::random_hues(uint32_t count, scalar_type value, scalar_type saturation, scalar_type alpha) {
	auto lower = color_limits_t<scalar_type>::lower;
	auto upper = color_limits_t<scalar_type>::upper;
	auto gen = rng::uniform_ab_gen<scalar_type>(lower, upper);
	std::vector<color_type> result(count);
	for (uint32_t i = 0; i < count; ++i) {
		result[i] = color_type(HSVA<scalar_type>(gen(), value, saturation, alpha));
	}
}

template <class ColorType>
std::vector<typename generate<ColorType>::color_type> generate<ColorType>::uniform(uint32_t count, const range_type& hue_range, const range_type& value_range, const range_type& sat_range, scalar_type alpha) {
	auto lower = color_limits_t<scalar_type>::lower;
	auto upper = color_limits_t<scalar_type>::upper;
	float range = static_cast<float>(upper) - static_cast<float>(lower);


	float r_hue = 2.f * M_PI * static_cast<float>(hue_range.second - hue_range.first) / range;
	float r_sat = static_cast<float>(sat_range.second - sat_range.first) / range;
	float r_light = static_cast<float>(value_range.second - value_range.first) / range;
	float l_hue = 2.f * M_PI * static_cast<float>(hue_range.first - lower) / range;
	float l_sat = static_cast<float>(sat_range.first - lower) / range;
	float l_light = static_cast<float>(value_range.first - lower) / range;
	if (r_hue < 0.f || r_sat < 0.f || r_light < 0.f) {
		throw std::runtime_error("generate::uniform: Invalid range input.");
	}
	float sum = r_hue/(2.f*M_PI) + r_sat + r_light;
	Eigen::Vector3f w(r_hue / (2.f * M_PI * sum), r_sat / sum, r_light / sum);
	float eps = Eigen::NumTraits<float>::dummy_precision();
	float prod = (w[0] < eps ? 1.f : w[0]) * (w[1] < eps ? 1.f : w[1]) * (w[2] < eps ? 1.f : w[2]);

	float factor = cbrt(count) / cbrt(prod);
	for (int i=0; i<3; ++i) {
		w[i] = std::max(w[i] * factor, 1.f);
	}
	unsigned int c_s = std::floor(w[1]);
	unsigned int c_l = std::floor(w[2]);
	unsigned int c_h = std::ceil(static_cast<float>(count) / (c_s*c_l));

	float s_h = r_hue / (c_h > 0.f ? c_h : 1.f);
	float s_s = r_sat / (c_s > 1.f ? c_s - 1.f : 1.f);
	float s_l = r_light / (c_l > 1.f ? c_l - 1.f : 2.f);
	std::vector<color_type> result;
	for (unsigned int i=0; i<c_h; ++i) {
		float h = l_hue + static_cast<float>(i) * s_h;
		for (unsigned int j=0; j<c_s; ++j) {
			float s = l_sat + static_cast<float>(j) * s_s;
			for (unsigned int k=0; k<c_l; ++k) {
				float l = l_light + static_cast<float>(k) * s_l;
				result.push_back(color_type(HSVA<scalar_type>(h, s, l, alpha)));
			}
		}
	}
	result.resize(count);
	return result;
}

template <class ColorType>
std::vector<typename generate<ColorType>::color_type> generate<ColorType>::shuffled_uniform(uint32_t count, const range_type& hue_range, const range_type& value_range, const range_type& sat_range, scalar_type alpha) {
	auto colors = uniform(count, hue_range, value_range, sat_range, alpha);
	uint32_t seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(colors.begin(), colors.end(), std::default_random_engine(seed));
	return colors;
}


#define TYPE_LIST \
	X(float) \
	X(double) \
	X(short) \
	X(int) \
	X(unsigned int) \
	X(char) \
	X(unsigned char)

// instantiate color types
#define X(type) \
	template class RGB<type>; \
	template class RGBA<type>; \
	template class HSV<type>; \
	template class HSVA<type>;
TYPE_LIST
#undef X

// instantiate generator functions
#define X(type) \
	template RGB<type> generate<RGB<type>>::random_hue(type value, type saturation, type alpha); \
	template std::vector<RGB<type>> generate<RGB<type>>::random_hues(uint32_t count, type value, type saturation, type alpha); \
	template std::vector<RGB<type>> generate<RGB<type>>::uniform(uint32_t count, const range_type& hue_range, const range_type& value_range, const range_type& sat_range, scalar_type alpha); \
	template std::vector<RGB<type>> generate<RGB<type>>::shuffled_uniform(uint32_t count, const range_type& hue_range, const range_type& value_range, const range_type& sat_range, scalar_type alpha); \
	template RGBA<type> generate<RGBA<type>>::random_hue(type value, type saturation, type alpha); \
	template std::vector<RGBA<type>> generate<RGBA<type>>::random_hues(uint32_t count, type value, type saturation, type alpha); \
	template std::vector<RGBA<type>> generate<RGBA<type>>::uniform(uint32_t count, const range_type& hue_range, const range_type& value_range, const range_type& sat_range, scalar_type alpha); \
	template std::vector<RGBA<type>> generate<RGBA<type>>::shuffled_uniform(uint32_t count, const range_type& hue_range, const range_type& value_range, const range_type& sat_range, scalar_type alpha); \
	template HSV<type> generate<HSV<type>>::random_hue(type value, type saturation, type alpha); \
	template std::vector<HSV<type>> generate<HSV<type>>::random_hues(uint32_t count, type value, type saturation, type alpha); \
	template std::vector<HSV<type>> generate<HSV<type>>::uniform(uint32_t count, const range_type& hue_range, const range_type& value_range, const range_type& sat_range, scalar_type alpha); \
	template std::vector<HSV<type>> generate<HSV<type>>::shuffled_uniform(uint32_t count, const range_type& hue_range, const range_type& value_range, const range_type& sat_range, scalar_type alpha); \
	template HSVA<type> generate<HSVA<type>>::random_hue(type value, type saturation, type alpha); \
	template std::vector<HSVA<type>> generate<HSVA<type>>::random_hues(uint32_t count, type value, type saturation, type alpha); \
	template std::vector<HSVA<type>> generate<HSVA<type>>::uniform(uint32_t count, const range_type& hue_range, const range_type& value_range, const range_type& sat_range, scalar_type alpha); \
	template std::vector<HSVA<type>> generate<HSVA<type>>::shuffled_uniform(uint32_t count, const range_type& hue_range, const range_type& value_range, const range_type& sat_range, scalar_type alpha);
TYPE_LIST
#undef X


} // damogran
