#include "Config.hpp"
#include "Options.hpp"

#include "Helpers/Math.hpp"
#include "Helpers/Utils.hpp"
#include "Helpers/Input.hpp"
#include "Menu.hpp"

void CConfig::SetupValue(int& value, int def, std::string category, std::string name) { value = def; ints.push_back(new ConfigValue< int >(category, name, &value, def)); }
void CConfig::SetupValue(char* value, char* def, std::string category, std::string name) { value = def; chars.push_back(new ConfigValue< char >(category, name, value, *def)); }
void CConfig::SetupValue(float& value, float def, std::string category, std::string name) { value = def; floats.push_back(new ConfigValue< float >(category, name, &value, def)); }
void CConfig::SetupValue(bool& value, bool def, std::string category, std::string name) { value = def; bools.push_back(new ConfigValue< bool >(category, name, &value, def)); }
void CConfig::SetupColor(float value[4], std::string name)
{
    SetupValue(value[0], value[0], ("colors"), name + "_r");
    SetupValue(value[1], value[1], ("colors"), name + "_g");
    SetupValue(value[2], value[2], ("colors"), name + "_b");
    SetupValue(value[3], value[3], ("colors"), name + "_a");
}

void CConfig::SetupVisuals()
{
    //ESP
    SetupValue(g_Options.esp_enabled, false, ("Esp"), ("Enabled"));
    SetupValue(g_Options.esp_enemies_only, false, ("Esp"), ("enemy_only"));
    SetupValue(g_Options.esp_player_boxes, false, ("Esp"), ("boxes"));
    SetupValue(g_Options.esp_player_names, false, ("Esp"), ("names"));
    SetupValue(g_Options.esp_player_health, false, ("Esp"), ("health"));
    SetupValue(g_Options.esp_player_armour, false, ("Esp"), ("armour"));
    SetupValue(g_Options.esp_player_weapons, false, ("Esp"), ("weapons"));
    SetupValue(g_Options.esp_player_snaplines, false, ("Esp"), ("snaplines"));

    //MISC
    SetupValue(g_Options.esp_crosshair, false, ("Esp"), ("crosshair"));
    SetupValue(g_Options.esp_dropped_weapons, false, ("Esp"), ("dropped_weapons"));
    SetupValue(g_Options.esp_defuse_kit, false, ("Esp"), ("defuse_kit"));
    SetupValue(g_Options.esp_planted_c4, false, ("Esp"), ("planted_c4"));
    SetupValue(g_Options.esp_items, false, ("Esp"), ("items"));

    //GLOW
    SetupValue(g_Options.glow_enabled, false, ("Glow"), ("Enabled"));
    SetupValue(g_Options.glow_enemies_only, false, ("Glow"), ("enemy_only"));
    SetupValue(g_Options.glow_players, false, ("Glow"), ("players"));
    SetupValue(g_Options.glow_chickens, false, ("Glow"), ("chickens"));
    SetupValue(g_Options.glow_c4_carrier, false, ("Glow"), ("c4_carrier"));
    SetupValue(g_Options.glow_planted_c4, false, ("Glow"), ("planted_c4"));
    SetupValue(g_Options.glow_defuse_kits, false, ("Glow"), ("defuse_kits"));
    SetupValue(g_Options.glow_weapons, false, ("Glow"), ("weapons"));

    //CHAMS
    SetupValue(g_Options.chams_player_enabled, false, ("Chams"), ("Enabled"));
    SetupValue(g_Options.chams_player_enemies_only, false, ("Chams"), ("enemy_only"));
    SetupValue(g_Options.chams_player_wireframe, false, ("Chams"), ("player_wireframe"));
    SetupValue(g_Options.chams_player_flat, false, ("Chams"), ("player_flat"));
    SetupValue(g_Options.chams_player_ignorez, false, ("Chams"), ("player_ignorez"));
    SetupValue(g_Options.chams_player_glass, false, ("Chams"), ("player_glass"));

    //CHAMS(arms)
    SetupValue(g_Options.chams_arms_enabled, false, ("Chams"), ("Enabled"));
    SetupValue(g_Options.chams_arms_wireframe, false, ("Chams"), ("arms_wireframe"));
    SetupValue(g_Options.chams_arms_flat, false, ("Chams"), ("arms_flat"));
    SetupValue(g_Options.chams_arms_ignorez, false, ("Chams"), ("arms_ignorez"));
    SetupValue(g_Options.chams_arms_glass, false, ("Chams"), ("arms_glass"));
}

void CConfig::SetupMisc()
{
    SetupValue(g_Options.misc_bhop, false, ("Misc"), ("bhop"));
    SetupValue(g_Options.misc_no_hands, false, ("Misc"), ("no_hands"));
    SetupValue(g_Options.misc_thirdperson, false, ("Misc"), ("thirdperson"));
    SetupValue(g_Options.misc_showranks, false, ("Misc"), ("showranks"));
    SetupValue(g_Options.misc_watermark, false, ("Misc"), ("watermark"));
    SetupValue(g_Options.misc_thirdperson_dist, 100, ("Misc"), ("thirdperson_dist"));
    SetupValue(g_Options.viewmodel_fov, 68, ("Misc"), ("viewmodel_fov"));
    SetupValue(g_Options.mat_ambient_light_r, 0, ("Misc"), ("mat_ambient_light_r"));
    SetupValue(g_Options.mat_ambient_light_g, 0, ("Misc"), ("mat_ambient_light_g"));
    SetupValue(g_Options.mat_ambient_light_b, 0, ("Misc"), ("mat_ambient_light_b"));
}

void CConfig::SetupColors()
{
    // ESP
    SetupColor(g_Options.color_esp_ally_visible, "Esp team Visible color");
    SetupColor(g_Options.color_esp_enemy_visible, "Esp team Visible color");
    SetupColor(g_Options.color_esp_ally_occluded, "Esp team Visible color");
    SetupColor(g_Options.color_esp_enemy_occluded, "Esp team Visible color");

    //MISC
    SetupColor(g_Options.color_esp_crosshair, "Esp team Visible color");
    SetupColor(g_Options.color_esp_weapons, "Esp team Visible color");
    SetupColor(g_Options.color_esp_defuse, "Esp team Visible color");
    SetupColor(g_Options.color_esp_c4, "Esp team Visible color");
    SetupColor(g_Options.color_esp_item, "Esp team Visible color");

    //GLOW
    SetupColor(g_Options.color_glow_ally, "Esp team Visible color");
    SetupColor(g_Options.color_glow_enemy, "Esp team Visible color");
    SetupColor(g_Options.color_glow_chickens, "Esp team Visible color");
    SetupColor(g_Options.color_glow_c4_carrier, "Esp team Visible color");
    SetupColor(g_Options.color_glow_planted_c4, "Esp team Visible color");
    SetupColor(g_Options.color_glow_defuse, "Esp team Visible color");
    SetupColor(g_Options.color_glow_weapons, "Esp team Visible color");

    //CHAMS
    SetupColor(g_Options.color_chams_player_ally_visible, "Esp team Visible color");
    SetupColor(g_Options.color_chams_player_ally_occluded, "Esp team Visible color");
    SetupColor(g_Options.color_chams_player_enemy_visible, "Esp team Visible color");
    SetupColor(g_Options.color_chams_player_enemy_occluded, "Esp team Visible color");
    SetupColor(g_Options.color_chams_arms_visible, "Esp team Visible color");
    SetupColor(g_Options.color_chams_arms_occluded, "Esp team Visible color");
    SetupColor(g_Options.color_watermark, "Esp team Visible color");
}

void CConfig::Setup() {

    CConfig::SetupVisuals();
    CConfig::SetupMisc();
    CConfig::SetupColors();
}

void CConfig::Save(const std::string& name) {
    if (name.empty())
        return;

    CreateDirectoryA(u8"C:\\Explicit\\Configs\\", NULL); 
    std::string file = u8"C:\\Explicit\\Configs\\" + name; 

    for (auto value : ints) {
        WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());
    }

    for (auto value : floats) WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());
    for (auto value : bools) WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void CConfig::Load(const std::string& name) {
    if (name.empty())
        return;

    g_ClientState->ForceFullUpdate();

    CreateDirectoryA(u8"C:\\Explicit\\Configs\\", NULL);
    std::string file = u8"C:\\Explicit\\Configs\\" + name; 

    char value_l[32] = { '\0' };
    for (auto value : ints) {
        GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "0", value_l, 32, file.c_str()); *value->value = atoi(value_l);
    }

    for (auto value : floats) {
        GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "0.0f", value_l, 32, file.c_str()); *value->value = atof(value_l);
    }

    for (auto value : bools) {
        GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "false", value_l, 32, file.c_str()); *value->value = !strcmp(value_l, "true");
    }
}

CConfig* Config = new CConfig();