#pragma once

#include <string>

#include "singleton.hpp"
#include "helpers/Utils.hpp"
#include "valve_sdk/math/QAngle.hpp"

class CUserCmd;
class CViewSetup;

class Miscellaneous : public Singleton<Miscellaneous>
{
public:

	void AutoStrafe(CUserCmd* userCMD);
	inline int32_t GetChocked() { return choked; }


	int changes = -1;

     template<class T, class U>
	T clamp(T in, U low, U high);

private:

	int32_t choked = 0;
};
