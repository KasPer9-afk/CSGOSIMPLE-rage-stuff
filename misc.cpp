#include "misc.hpp"

#include "valve_sdk/csgostructs.hpp"
#include "options.hpp"
#include "helpers/Math.hpp"

#include "features/aimbot.hpp"
#include "features/prediction.hpp"

#include <chrono>

template<class T, class U>
T Clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}

void Miscellaneous::AutoStrafe(CUserCmd* userCMD)
{
	if (g_LocalPlayer->m_nMoveType() == MOVETYPE_NOCLIP || g_LocalPlayer->m_nMoveType() == MOVETYPE_LADDER || !g_LocalPlayer->IsAlive()) return;

	// If we're not jumping or want to manually move out of the way/jump over an obstacle don't strafe.
	if (!g_InputSystem->IsButtonDown(ButtonCode_t::KEY_SPACE) ||
		g_InputSystem->IsButtonDown(ButtonCode_t::KEY_A) ||
		g_InputSystem->IsButtonDown(ButtonCode_t::KEY_D) ||
		g_InputSystem->IsButtonDown(ButtonCode_t::KEY_S) ||
		g_InputSystem->IsButtonDown(ButtonCode_t::KEY_W))
		return;

	if (!(g_LocalPlayer->m_fFlags() & FL_ONGROUND)) {
		if (userCMD->mousedx > 1 || userCMD->mousedx < -1) {
			userCMD->sidemove = clamp(userCMD->mousedx < 0.f ? -400.f : 400.f, -400, 400);
		}
		else {
			if (g_LocalPlayer->m_vecVelocity().Length2D() == 0 || g_LocalPlayer->m_vecVelocity().Length2D() == NAN || g_LocalPlayer->m_vecVelocity().Length2D() == INFINITE)
			{
				userCMD->forwardmove = 400;
				return;
			}
			userCMD->forwardmove = clamp(5850.f / g_LocalPlayer->m_vecVelocity().Length2D(), -400, 400);
			if (userCMD->forwardmove < -400 || userCMD->forwardmove > 400)
				userCMD->forwardmove = 0;
			userCMD->sidemove = clamp((userCMD->command_number % 2) == 0 ? -400.f : 400.f, -400, 400);
			if (userCMD->sidemove < -400 || userCMD->sidemove > 400)
				userCMD->sidemove = 0;
		}
	}
}
template<class T, class U>
T Miscellaneous::clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}