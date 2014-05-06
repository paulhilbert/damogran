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

#ifndef DAMOGRAN_COLORS_HPP_
#define DAMOGRAN_COLORS_HPP_

#include <vector>
#include "common.hpp"

namespace damogran {

template <typename Scalar>
using col_vec3_t = Eigen::Matrix<Scalar, 3, 1>;

template <typename Scalar>
using col_vec4_t = Eigen::Matrix<Scalar, 4, 1>;


template <typename Scalar> class RGB;
template <typename Scalar> class RGBA;
template <typename Scalar> class HSV;
template <typename Scalar> class HSVA;

template <typename Scalar>
class RGB : public col_vec3_t<Scalar> {
	public:
		typedef Scalar scalar_type;

	public:
		RGB();
		RGB(Scalar r, Scalar g, Scalar b);
		RGB(const RGB<Scalar>& rgb);
		RGB(const RGBA<Scalar>& rgba);
		RGB(const HSV<Scalar>& hsv);
		RGB(const HSVA<Scalar>& hsva);
		virtual ~RGB();

		Scalar& r() {
			return (*this)[0];
		}

		const Scalar& r() const {
			return (*this)[0];
		}

		Scalar& g() {
			return (*this)[1];
		}

		const Scalar& g() const {
			return (*this)[1];
		}

		Scalar& b() {
			return (*this)[2];
		}

		const Scalar& b() const {
			return (*this)[2];
		}
};

template <typename Scalar>
class RGBA : public col_vec4_t<Scalar> {
	public:
		typedef Scalar scalar_type;

	public:
		RGBA();
		RGBA(Scalar r, Scalar g, Scalar b, Scalar a = Scalar(1));
		RGBA(const RGB<Scalar>& rgb);
		RGBA(const RGBA<Scalar>& rgba);
		RGBA(const HSV<Scalar>& hsv);
		RGBA(const HSVA<Scalar>& hsva);
		virtual ~RGBA();

		Scalar& r() {
			return (*this)[0];
		}

		const Scalar& r() const {
			return (*this)[0];
		}

		Scalar& g() {
			return (*this)[1];
		}

		const Scalar& g() const {
			return (*this)[1];
		}

		Scalar& b() {
			return (*this)[2];
		}

		const Scalar& b() const {
			return (*this)[2];
		}

		Scalar& a() {
			return (*this)[3];
		}

		const Scalar& a() const {
			return (*this)[3];
		}
};

template <typename Scalar>
class HSV : public col_vec3_t<Scalar> {
	public:
		typedef Scalar scalar_type;

	public:
		HSV();
		HSV(Scalar h, Scalar s, Scalar v);
		HSV(const RGB<Scalar>& rgb);
		HSV(const RGBA<Scalar>& rgba);
		HSV(const HSV<Scalar>& hsv);
		HSV(const HSVA<Scalar>& hsva);
		virtual ~HSV();

		Scalar& h() {
			return (*this)[0];
		}

		const Scalar& h() const {
			return (*this)[0];
		}

		Scalar& s() {
			return (*this)[1];
		}

		const Scalar& s() const {
			return (*this)[1];
		}

		Scalar& v() {
			return (*this)[2];
		}

		const Scalar& v() const {
			return (*this)[2];
		}
};

template <typename Scalar>
class HSVA : public col_vec4_t<Scalar> {
	public:
		typedef Scalar scalar_type;

	public:
		HSVA();
		HSVA(Scalar h, Scalar s, Scalar v, Scalar a = Scalar(1));
		HSVA(const RGB<Scalar>& rgb);
		HSVA(const RGBA<Scalar>& rgba);
		HSVA(const HSV<Scalar>& hsv);
		HSVA(const HSVA<Scalar>& hsva);
		virtual ~HSVA();

		Scalar& h() {
			return (*this)[0];
		}

		const Scalar& h() const {
			return (*this)[0];
		}

		Scalar& s() {
			return (*this)[1];
		}

		const Scalar& s() const {
			return (*this)[1];
		}

		Scalar& v() {
			return (*this)[2];
		}

		const Scalar& v() const {
			return (*this)[2];
		}

		Scalar& a() {
			return (*this)[3];
		}

		const Scalar& a() const {
			return (*this)[3];
		}
};



template <class ColorType>
struct generate {
	typedef ColorType color_type;
	typedef typename ColorType::scalar_type scalar_type;

	static color_type random_hue(scalar_type value = scalar_type(1), scalar_type saturation = scalar_type(1), scalar_type alpha = scalar_type(1));
	static std::vector<color_type> random_hues(uint32_t count, scalar_type value = scalar_type(1), scalar_type saturation = scalar_type(1), scalar_type alpha = scalar_type(1));
	//static std::vector<color_type> uniform(unsigned int count, const Range& hueRange, const Range& satRange, const Range& lightRange, float alpha) {
};

/*
inline std::vector<RGBA> Generation::uniformColorsRGBA(unsigned int count, const Range& hueRange, const Range& satRange, const Range& lightRange, float alpha) {
	float rHue = hueRange.second - hueRange.first;
	float rSat = satRange.second - satRange.first;
	float rLight = lightRange.second - lightRange.first;
	if (rHue < 0.f || rSat < 0.f || rLight < 0.f) {
		throw std::runtime_error("Generation::uniformColorsRGBA: Invalid range input.");
	}
	float sum = rHue/(2.f*M_PI) + rSat + rLight;
	Vector3f w(rHue / (2.f * M_PI * sum), rSat / sum, rLight / sum);
	float eps = Eigen::NumTraits<float>::dummy_precision();
	float prod = (w[0] < eps ? 1.f : w[0]) * (w[1] < eps ? 1.f : w[1]) * (w[2] < eps ? 1.f : w[2]);

	float factor = cbrt(count) / cbrt(prod);
	for (int i=0; i<3; ++i) {
		w[i] = std::max(w[i] * factor, 1.f);
	}
	unsigned int cS = std::floor(w[1]);
	unsigned int cL = std::floor(w[2]);
	unsigned int cH = std::ceil(static_cast<float>(count) / (cS*cL));

	float sH = rHue / (cH > 0.f ? cH : 1.f);
	float sS = rSat / (cS > 1.f ? cS - 1.f : 1.f);
	float sL = rLight / (cL > 1.f ? cL - 1.f : 2.f);
	std::vector<RGBA> result;
	for (unsigned int i=0; i<cH; ++i) {
		float h = hueRange.first + static_cast<float>(i) * sH;
		for (unsigned int j=0; j<cS; ++j) {
			float s = satRange.first + static_cast<float>(j) * sS;
			for (unsigned int k=0; k<cL; ++k) {
				float l = lightRange.first + static_cast<float>(k) * sL;
				HSVA hsva(h, s, l, alpha);
				RGBA rgba = Conversion::hsva2rgba(hsva);
				result.push_back(rgba);
			}
		}
	}
	result.resize(count);
	return result;
}

inline std::vector<RGBA> Generation::shuffledColorsRGBA(unsigned int count, const Range& hueRange, const Range& satRange, const Range& lightRange, float alpha) {
	std::vector<RGBA> result = uniformColorsRGBA(count, hueRange, satRange, lightRange, alpha);
	std::random_shuffle(result.begin(), result.end());
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(result.begin(), result.end(), std::default_random_engine(seed));
	return result;
}
*/

} // damogran

#endif /* DAMOGRAN_COLORS_HPP_ */
