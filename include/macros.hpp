#ifndef DAMOGRAN_MACROS_HPP_
#define DAMOGRAN_MACROS_HPP_


#define COMMON_NUMERIC_TYPES \
	X(float) \
	X(double) \
	X(short) \
	X(unsigned short) \
	X(int) \
	X(unsigned int) \
	X(char) \
	X(unsigned char)

#define INTEGRAL_NUMERIC_TYPES \
	X(short) \
	X(int) \
	X(unsigned int) \
	X(char) \
	X(unsigned char)

#define REAL_NUMERIC_TYPES \
	X(float) \
	X(double)

#define DURATION_TYPES \
	X(std::chrono::hours) \
	X(std::chrono::minutes) \
	X(std::chrono::seconds) \
	X(std::chrono::milliseconds) \
	X(std::chrono::microseconds) \
	X(std::chrono::nanoseconds)


#endif /* MACROS_HPP_ */
