#ifndef DAMOGRAN_RANGE_H_
#define DAMOGRAN_RANGE_H_

#include <utility>

namespace damogran {

template <class Iter>
class range : public std::pair<Iter, Iter> {
	public:
		range(const std::pair<Iter,Iter>& x) : std::pair<Iter,Iter>(x) {}
		virtual ~range() {}

		Iter begin() const { return this->first;  }
		Iter end()   const { return this->second; }
};


} // damogran

#endif /* DAMOGRAN_RANGE_H_ */
