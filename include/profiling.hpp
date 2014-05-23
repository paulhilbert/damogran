#ifndef DAMOGRAN_PROFILING_H
#define DAMOGRAN_PROFILING_H

#include <string>
#include <map>
#include <chrono>

#include "common.hpp"


namespace damogran {

class profiling {
	public:
		/** Shared pointer to this class */
		typedef std::shared_ptr<profiling>        ptr;
		/** Weak pointer to this class */
		typedef std::weak_ptr<profiling>          wptr;
		/** Shared const pointer to this class */
		typedef std::shared_ptr<const profiling>  const_ptr;
		/** Weak const pointer to this class */
		typedef std::weak_ptr<const profiling>    const_wptr;

	protected:
		typedef std::chrono::steady_clock       clock_t;
		typedef std::chrono::time_point<clock_t>  time_point_t;
		typedef clock_t::duration                 duration_t;
		struct TimeInfo {
			time_point_t start;
			duration_t  passed;
			bool      finished;
		};

	public:
		/** 
		 *  Constructor 
		 */
		profiling();

		/**
		 *  Start/restarts new profile with given name.
		 *
		 *  This member function creates and starts a new time measurement
		 *  record with the given name. This name should be unique for the
		 *  measurement and is used to reference (e.g. when stopping) the
		 *  profile later. It is also used for printing the profile summary
		 *  using summarize().
		 *
		 *  This function may also restarts previously stopped profiles
		 *  and any measured time is added to the entire measurement of
		 *  the profile.
		 *
		 *  @param name Unique name of the profile to be started.
		 */
		void start(std::string name);

		/**
		 *  Ends measurement of given profile.
		 *
		 *  When called, this member function records the time passed
		 *  since the last call to start(std::string name).
		 *
		 *  @param name Unique name of the profile used for the start call.
		 */
		void end(std::string name);

		/**
		 *  Starts/stops profiles depending on their current status.
		 *
		 *  This function is a convenience wrapper. On first call it starts
		 *  a profile with the given name. On second call it stops it, then
		 *  restarts it and so on...
		 *
		 *  @param name Unique name of profile to start/stop.
		 */
		void profile(std::string name);

		/**
		 *  Print summary of all measured profiles.
		 *
		 *  Depending on the measurement needs a suitable unit of measurement
		 *  should be used for the template parameter. Implemented are:
		 *
		 *  - std::chrono::hours
		 *  - std::chrono::minutes
		 *  - std::chrono::seconds
		 *  - std::chrono::milliseconds
		 *  - std::chrono::microseconds
		 *  - std::chrono::nanoseconds
		 *
		 *  The printed unit names and values are changed accordingly.
		 *
		 *  @tparam DurationType Unit of measurement to use for the printed duration.
		 *  @param logFunction If given, this callback function is used to print the summary, otherwise std::cout is used.
		 */
		template <class DurationType>
		void summarize(std::function<void (std::string)> logFunction = nullptr);

		template <class DurationType>
		typename DurationType::rep duration(std::string profile) const;

	protected:
		template <class DurationType>
		std::string format(std::string name, typename DurationType::rep duration);

		template <class DurationType>
		std::string unit();

	protected:
		std::map< std::string, TimeInfo >  profiles_;
};


} // damogran

#endif // DAMOGRAN_PROFILING_H
