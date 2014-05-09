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
	typedef std::pair<scalar_type, scalar_type> range_type;

	static color_type random_hue(scalar_type value = scalar_type(1), scalar_type saturation = scalar_type(1), scalar_type alpha = scalar_type(1));
	static std::vector<color_type> random_hues(uint32_t count, scalar_type value = scalar_type(1), scalar_type saturation = scalar_type(1), scalar_type alpha = scalar_type(1));
	static std::vector<color_type> uniform(uint32_t count, const range_type& hue_range, const range_type& value_range, const range_type& sat_range, scalar_type alpha = scalar_type(1));
	static std::vector<color_type> shuffled_uniform(uint32_t count, const range_type& hue_range, const range_type& value_range, const range_type& sat_range, scalar_type alpha = scalar_type(1));
};


} // damogran

#endif /* DAMOGRAN_COLORS_HPP_ */
