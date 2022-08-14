#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include "valve_sdk/Misc/Color.hpp"
#include <map>

#define A( s ) #s
#define OPTION(type, var, val) type var = val
extern std::map<int, const char*> K_weapon_names;

class Options
{
public:
	    //
        // AIM
        //
	    OPTION(bool, aim_enabled, false);
	    OPTION(bool, aim_friendly_fire, false);
	    OPTION(bool, aim_auto_shoot, false);
	    OPTION(bool, aim_silent_mode, false);
	    OPTION(int, silentaimtype, 0);
	    OPTION(bool, auto_duck, false);
	    OPTION(bool, auto_wall, false);
	    OPTION(float, auto_wall_mindamange, 20.0f);
	    OPTION(bool, hit_chance, false);
	    OPTION(bool, multipointer, false);
	    OPTION(bool, fovfix, false);
	    OPTION(bool, attarget, false);
	    OPTION(bool, thpaa, false);
	    OPTION(bool, vecvelocityprediction, false);
	    OPTION(float, hit_chanceamount, false);
	    OPTION(bool, auto_scope, false);
	    OPTION(float, aim_fov, 180.f);
	    OPTION(int, aay, 0);
	    OPTION(int, aax, 0);
	    OPTION(int, aafake, 0);
		OPTION(bool, auto_stop, false);	
	    //
        //PlayerList
        //
	    OPTION(bool, entity_friend, false);
     	OPTION(int, selected_player_index, 0);
		// 
		// ESP
		// 
		OPTION(bool, esp_enabled, false);
		OPTION(bool, esp_enemies_only, false);
		OPTION(bool, esp_player_boxes, false);
		OPTION(bool, esp_player_names, false);
		OPTION(bool, esp_player_health, false);
		OPTION(bool, esp_player_armour, false);
		OPTION(bool, esp_player_weapons, false);
		OPTION(bool, esp_player_snaplines, false);
		OPTION(bool, esp_crosshair, false);
		OPTION(bool, esp_dropped_weapons, false);
		OPTION(bool, esp_defuse_kit, false);
		OPTION(bool, esp_planted_c4, false);
		OPTION(bool, esp_items, false);
		OPTION(bool, esp_grenade_prediction, false);

		// 
		// GLOW
		// 
		OPTION(bool, glow_enabled, false);
		OPTION(bool, glow_enemies_only, false);
		OPTION(bool, glow_players, false);
		OPTION(bool, glow_chickens, false);
		OPTION(bool, glow_c4_carrier, false);
		OPTION(bool, glow_planted_c4, false);
		OPTION(bool, glow_defuse_kits, false);
		OPTION(bool, glow_weapons, false);

		//
		// CHAMS
		//
		OPTION(bool, chams_player_enabled, false);
		OPTION(bool, chams_player_enemies_only, false);
		OPTION(bool, chams_player_wireframe, false);
		OPTION(bool, chams_player_flat, false);
		OPTION(bool, chams_player_ignorez, false);
		OPTION(bool, chams_player_glass, false);
		OPTION(bool, chams_arms_enabled, false);
		OPTION(bool, chams_arms_wireframe, false);
		OPTION(bool, chams_arms_flat, false);
		OPTION(bool, chams_arms_ignorez, false);
		OPTION(bool, chams_arms_glass, false);

		//
		// MISC
		//
		OPTION(bool, misc_bhop, false);
		OPTION(bool, misc_autostrafe, false);
		OPTION(bool, misc_no_hands, false);
		OPTION(bool, misc_thirdperson, false);
		OPTION(bool, misc_showranks, false);
		OPTION(bool, misc_watermark, false);
		OPTION(float, misc_thirdperson_dist, 50.f);
		OPTION(int, viewmodel_fov, 68);
		OPTION(float, mat_ambient_light_r, 0.0f);
		OPTION(float, mat_ambient_light_g, 0.0f);
		OPTION(float, mat_ambient_light_b, 0.0f);

		// 
		// COLORS
		// 
		float color_esp_ally_visible[4] = {1.0f, 1.0f, 1.0f, 1.0f};
		float color_esp_enemy_visible[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float color_esp_ally_occluded[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float color_esp_enemy_occluded[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float color_esp_crosshair[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float color_esp_weapons[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float color_esp_defuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float color_esp_c4[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float color_esp_item[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

		float color_glow_ally[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float color_glow_enemy[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float color_glow_chickens[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float color_glow_c4_carrier[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float color_glow_planted_c4[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float color_glow_defuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float color_glow_weapons[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

		float color_chams_player_ally_visible[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float color_chams_player_ally_occluded[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float color_chams_player_enemy_visible[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float color_chams_player_enemy_occluded[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float color_chams_arms_visible[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float color_chams_arms_occluded[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float color_watermark[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

		float color_grenade_prediction[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
};

extern Options g_Options;
extern bool g_Unload;
