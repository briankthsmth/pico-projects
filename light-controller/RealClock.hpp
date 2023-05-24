// Create by Brian Smith 4/27/2023

#ifndef RealClock_hpp
#define RealClock_hpp

#include "boards/sparkfun_promicro.h"
#include "pico/util/datetime.h"

// Uses a RV8O33 on the I2C bus at address 0x32 for date time operations.
namespace Peripheral {

	class RealClock {
	public:
		RealClock();
		~RealClock() = default;

		void set_datetime(const datetime_t);
		datetime_t get_datetime();
		
	private:
		RealClock(const RealClock&) = delete;
		RealClock& operator= (const RealClock&) = delete;

		struct Register {
			static const uint8_t seconds; 
		};
	}; // class RealClock
 
}; // namespace Peripheral

#endif // RealClock_hpp