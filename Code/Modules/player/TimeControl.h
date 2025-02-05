#ifndef MUTALISK__TIME_CONTROL_H_
#define MUTALISK__TIME_CONTROL_H_

#include "cfg.h"

namespace mutalisk
{
	class TimeControl
	{
	public:
		TimeControl();

		float update(float dt);
		void restart(int complete);
		void scroll(float dt);
		void pause(int flag = true);
		
		void resetKeys();
		void from(float key);
		void to(float key);

		float time() const { return static_cast<float>(currentTime); }
		int isPaused() const { return paused; }

		float getDiscontinuity();

	private:
		float currentTime;
		float timeKey[2];
		bool paused;
		float discontinuity;
	};
} // namespace mutalisk

#endif // MUTALISK__TIME_CONTROL_H_
