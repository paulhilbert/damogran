#include <profiling.hpp>


namespace damogran {


profiling::profiling() {
}

#ifdef PROFILING
void profiling::start(std::string name) {
	if (profiles_.find(name) != profiles_.end()) {
		profiles_[name].start = clock_t::now();
		profiles_[name].finished = false;
	} else {
		TimeInfo t;
		t.start = clock_t::now();
		t.passed = duration_t(0);
		t.finished = false;
		profiles_[name] = t;
	}
}

void profiling::end(std::string name) {
	if (!profiles_.count(name)) return;

	profiles_[name].passed += clock_t::now() - profiles_[name].start;
	profiles_[name].finished = true;
}

void profiling::profile(std::string name) {
	if (profiles_.count(name) && !profiles_[name].finished) {
		end(name);
	} else {
		start(name);
	}
}

template <class DurationType>
void profiling::summarize(std::function<void (std::string)> logFunction) {
	if (!profiles_.size()) return;
	std::function<void (std::string)> log = logFunction ? std::move(logFunction) : [&] (std::string msg) { std::cout << msg << "\n"; };

	for (auto it = profiles_.begin(); it != profiles_.end(); ++it) {
		if (!(*it).second.finished) {
			log("Unfinished profile block: " + (*it).first);
			end((*it).first);
		}
	}

	// print empty lines before summary
	log("");
	log("profiling summary");

	typedef typename DurationType::rep Rep;
	Rep overall(0);
	for (auto it = profiles_.begin(); it != profiles_.end(); ++it) {
		Rep duration = std::chrono::duration_cast<DurationType>(it->second.passed).count();
		overall += duration;
		log(format<DurationType>(it->first, duration));
		//double fraction = (double)(duration.total_microseconds()) / profileSpan.get().length().total_microseconds();
	}
	log(format<DurationType>("Summed: ", overall));
	log("Finished summary");
	profiles_.clear();
}

template <class DurationType>
typename DurationType::rep profiling::duration(std::string profile) const {
	auto findIt = profiles_.find(profile);
	if (findIt == profiles_.end()) {
		return typename DurationType::rep(0);
	}
	return std::chrono::duration_cast<DurationType>(findIt->second.passed).count();
}

#else

void profiling::start(std::string) {
}

void profiling::end(std::string) {
}

void profiling::profile(std::string) {
}

template <class DurationType>
void profiling::summarize(std::function<void (std::string)>) {
}

template <class DurationType>
typename DurationType::rep profiling::duration(std::string profile) const {
	return typename DurationType::rep(0);
}

#endif // PROFILING


template <class DurationType>
std::string profiling::format(std::string name, typename DurationType::rep duration) {
	char output[255];
	std::string format = "  %-25s%11d"+unit<DurationType>();//   %13.3f%%";
	sprintf(output, format.c_str(), name.c_str(), duration); //, fraction*100.0);
	std::string result(output);
	return result;
}

template <>
std::string profiling::unit<std::chrono::hours>() {
	return "h";
}

template <>
std::string profiling::unit<std::chrono::minutes>() {
	return "m";
}

template <>
std::string profiling::unit<std::chrono::seconds>() {
	return "s";
}

template <>
std::string profiling::unit<std::chrono::milliseconds>() {
	return "ms";
}

template <>
std::string profiling::unit<std::chrono::microseconds>() {
	return "us";
}

template <>
std::string profiling::unit<std::chrono::nanoseconds>() {
	return "ns";
}


} // damogran


#define X(type) \
	template void      damogran::profiling::summarize<type>(std::function<void (std::string)>) const; \
	template type::rep damogran::profiling::duration(std::string) const;
#undef X
