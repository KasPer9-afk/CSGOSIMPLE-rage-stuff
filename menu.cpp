#include "Menu.hpp"
#define NOMINMAX
#include <Windows.h>
#include <chrono>
#include <iomanip>

#include "valve_sdk/csgostructs.hpp"
#include "helpers/input.hpp"
#include "options.hpp"
#include "ui.hpp"
#include "config.hpp"
#include <algorithm>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"
#include "imgui/impl/imgui_impl_dx9.h"
#include "imgui/impl/imgui_impl_win32.h"
#include <experimental/filesystem>
#include "render.hpp"
namespace fs = std::experimental::filesystem;

// =========================================================
// 
// These are the tabs on the sidebar
// 
// =========================================================
static char* sidebar_tabs[] = {
    "ESP",
    "AIM",
    "MISC",
    "CONFIG",
    "PLAYER LIST"
};

constexpr static float get_sidebar_item_width() { return 150.0f; }
constexpr static float get_sidebar_item_height() { return  50.0f; }

enum {
	TAB_ESP,
	TAB_AIMBOT,
	TAB_MISC,
	TAB_CONFIG,
    TAB_PLAYERLIST
};

namespace ImGuiEx
{
    inline bool ColorEdit4(const char* label, Color* v, bool show_alpha = true)
    {
        auto clr = ImVec4{
            v->r() / 255.0f,
            v->g() / 255.0f,
            v->b() / 255.0f,
            v->a() / 255.0f
        };

        if(ImGui::ColorEdit4(label, &clr.x, show_alpha)) {
            v->SetColor(clr.x, clr.y, clr.z, clr.w);
            return true;
        }
        return false;
    }
    inline bool ColorEdit3(const char* label, Color* v)
    {
        return ColorEdit4(label, v, false);
    }
}

template<size_t N>
void render_tabs(char* (&names)[N], int& activetab, float w, float h, bool sameline)
{
    bool values[N] = { false };

    values[activetab] = true;

    for(auto i = 0; i < N; ++i) {
        if(ImGui::ToggleButton(names[i], &values[i], ImVec2{ w, h })) {
            activetab = i;
        }
        if(sameline && i < N - 1)
            ImGui::SameLine();
    }
}

ImVec2 get_sidebar_size()
{
    constexpr float padding = 10.0f;
    constexpr auto size_w = padding * 2.0f + get_sidebar_item_width();
    constexpr auto size_h = padding * 2.0f + (sizeof(sidebar_tabs) / sizeof(char*)) * get_sidebar_item_height();

    return ImVec2{ size_w, ImMax(325.0f, size_h) };
}

int get_fps()
{
    using namespace std::chrono;
    static int count = 0;
    static auto last = high_resolution_clock::now();
    auto now = high_resolution_clock::now();
    static int fps = 0;

    count++;

    if(duration_cast<milliseconds>(now - last).count() > 1000) {
        fps = count;
        count = 0;
        last = now;
    }

    return fps;
}
std::string usrname() {

    char buff[MAX_PATH];
    GetEnvironmentVariableA("USERNAME", buff, MAX_PATH);

    return std::string(buff);
}

PlayerList_t PlayerList[65];

void RenderEspTab()
{
    static char* esp_tab_names[] = { "ESP", "GLOW", "CHAMS" };
    static int   active_esp_tab = 0;

    bool placeholder_true = true;

    auto& style = ImGui::GetStyle();
    float group_w = ImGui::GetCurrentWindow()->Size.x - style.WindowPadding.x * 2;
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    {
        render_tabs(esp_tab_names, active_esp_tab, group_w / _countof(esp_tab_names), 25.0f, true);
    }
    ImGui::PopStyleVar();
    ImGui::BeginGroupBox("##body_content");
    {
        if(active_esp_tab == 0) {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
            ImGui::Columns(3, nullptr, false);
            ImGui::SetColumnOffset(1, group_w / 3.0f);
            ImGui::SetColumnOffset(2, 2 * group_w / 3.0f);
            ImGui::SetColumnOffset(3, group_w);

            ImGui::Checkbox("Enabled", &g_Options.esp_enabled);
            ImGui::Checkbox("Team check", &g_Options.esp_enemies_only);
            ImGui::Checkbox("Boxes", &g_Options.esp_player_boxes);
            ImGui::Checkbox("Names", &g_Options.esp_player_names);
            ImGui::Checkbox("Health", &g_Options.esp_player_health);
            ImGui::Checkbox("Armour", &g_Options.esp_player_armour);
            ImGui::Checkbox("Weapon", &g_Options.esp_player_weapons);
            ImGui::Checkbox("Snaplines", &g_Options.esp_player_snaplines);

            ImGui::NextColumn();

            ImGui::Checkbox("Crosshair", &g_Options.esp_crosshair);
            ImGui::Checkbox("Dropped Weapons", &g_Options.esp_dropped_weapons);
            ImGui::Checkbox("Defuse Kit", &g_Options.esp_defuse_kit);
            ImGui::Checkbox("Planted C4", &g_Options.esp_planted_c4);
			ImGui::Checkbox("Item Esp", &g_Options.esp_items);
            ImGui::Checkbox("Grenade prediction", &g_Options.esp_grenade_prediction);
            ImGui::SameLine();
            if (ImGui::Button("+##gr_pred"))
                ImGui::OpenPopup("##flaggr_pred");

            if (ImGui::BeginPopup("##flaggr_pred")) {
                ImGui::PushItemWidth(160.f);
                ImGui::ColorEdit3("Grenade prediction", g_Options.color_grenade_prediction);
                ImGui::PopItemWidth();
                ImGui::EndPopup();
            }

            ImGui::NextColumn();

            ImGui::PushItemWidth(100);
            ImGui::ColorEdit3("Allies Visible", g_Options.color_esp_ally_visible);
            ImGui::ColorEdit3("Enemies Visible", g_Options.color_esp_enemy_visible);
            ImGui::ColorEdit3("Allies Occluded", g_Options.color_esp_ally_occluded);
            ImGui::ColorEdit3("Enemies Occluded", g_Options.color_esp_enemy_occluded);
            ImGui::ColorEdit3("Crosshair", g_Options.color_esp_crosshair);
            ImGui::ColorEdit3("Dropped Weapons", g_Options.color_esp_weapons);
            ImGui::ColorEdit3("Defuse Kit", g_Options.color_esp_defuse);
            ImGui::ColorEdit3("Planted C4", g_Options.color_esp_c4);
			ImGui::ColorEdit3("Item Esp", g_Options.color_esp_item);
            ImGui::PopItemWidth();

            ImGui::Columns(1, nullptr, false);
            ImGui::PopStyleVar();
        } else if(active_esp_tab == 1) {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
            ImGui::Columns(3, nullptr, false);
            ImGui::SetColumnOffset(1, group_w / 3.0f);
            ImGui::SetColumnOffset(2, 2 * group_w / 3.0f);
            ImGui::SetColumnOffset(3, group_w);

            ImGui::Checkbox("Enabled", &g_Options.glow_enabled);
            ImGui::Checkbox("Team check", &g_Options.glow_enemies_only);
            ImGui::Checkbox("Players", &g_Options.glow_players);
            ImGui::Checkbox("Chickens", &g_Options.glow_chickens);
            ImGui::Checkbox("C4 Carrier", &g_Options.glow_c4_carrier);
            ImGui::Checkbox("Planted C4", &g_Options.glow_planted_c4);
            ImGui::Checkbox("Defuse Kits", &g_Options.glow_defuse_kits);
            ImGui::Checkbox("Weapons", &g_Options.glow_weapons);

            ImGui::NextColumn();

            ImGui::PushItemWidth(100);
            ImGui::ColorEdit3("Ally", g_Options.color_glow_ally);
            ImGui::ColorEdit3("Enemy", g_Options.color_glow_enemy);
            ImGui::ColorEdit3("Chickens", g_Options.color_glow_chickens);
            ImGui::ColorEdit3("C4 Carrier", g_Options.color_glow_c4_carrier);
            ImGui::ColorEdit3("Planted C4", g_Options.color_glow_planted_c4);
            ImGui::ColorEdit3("Defuse Kits", g_Options.color_glow_defuse);
            ImGui::ColorEdit3("Weapons", g_Options.color_glow_weapons);
            ImGui::PopItemWidth();

            ImGui::NextColumn();

            ImGui::Columns(1, nullptr, false);
            ImGui::PopStyleVar();
        } else if(active_esp_tab == 2) {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
            ImGui::Columns(3, nullptr, false);
            ImGui::SetColumnOffset(1, group_w / 3.0f);
            ImGui::SetColumnOffset(2, 2 * group_w / 2.9f);
            ImGui::SetColumnOffset(3, group_w);

            ImGui::BeginGroupBox("Players");
            {
                ImGui::Checkbox("Enabled", &g_Options.chams_player_enabled); ImGui::SameLine();
                ImGui::Checkbox("Enemy Only", &g_Options.chams_player_enemies_only);
                ImGui::Checkbox("Wireframe", &g_Options.chams_player_wireframe);
                ImGui::Checkbox("Flat", &g_Options.chams_player_flat);
                ImGui::Checkbox("Ignore-Z", &g_Options.chams_player_ignorez); ImGui::SameLine();
                ImGui::Checkbox("Glass", &g_Options.chams_player_glass);
                ImGui::PushItemWidth(110);
                ImGui::ColorEdit4("Ally (Visible)", g_Options.color_chams_player_ally_visible);
                ImGui::ColorEdit4("Ally (Occluded)", g_Options.color_chams_player_ally_occluded);
                ImGui::ColorEdit4("Enemy (Visible)", g_Options.color_chams_player_enemy_visible);
                ImGui::ColorEdit4("Enemy (Occluded)", g_Options.color_chams_player_enemy_occluded);
                ImGui::PopItemWidth();
            }
            ImGui::EndGroupBox();

            ImGui::NextColumn();

            ImGui::BeginGroupBox("Arms");
            {
                ImGui::Checkbox("Enabled", &g_Options.chams_arms_enabled);
                ImGui::Checkbox("Wireframe", &g_Options.chams_arms_wireframe);
                ImGui::Checkbox("Flat", &g_Options.chams_arms_flat);
                ImGui::Checkbox("Ignore-Z", &g_Options.chams_arms_ignorez);
                ImGui::Checkbox("Glass", &g_Options.chams_arms_glass);
                ImGui::PushItemWidth(110);
                ImGui::ColorEdit4("Color (Visible)", g_Options.color_chams_arms_visible);
                ImGui::ColorEdit4("Color (Occluded)", g_Options.color_chams_arms_occluded);
                ImGui::PopItemWidth();
            }
            ImGui::EndGroupBox();

            ImGui::Columns(1, nullptr, false);
            ImGui::PopStyleVar();
        }
    }
    ImGui::EndGroupBox();
}

void RenderMiscTab()
{
    bool placeholder_true = true;

    auto& style = ImGui::GetStyle();
    float group_w = ImGui::GetCurrentWindow()->Size.x - style.WindowPadding.x * 2;

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::ToggleButton("MISC", &placeholder_true, ImVec2{ group_w, 25.0f });
    ImGui::PopStyleVar();

    ImGui::BeginGroupBox("##body_content");
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
        ImGui::Columns(3, nullptr, false);
        ImGui::SetColumnOffset(1, group_w / 3.0f);
        ImGui::SetColumnOffset(2, 2 * group_w / 3.0f);
        ImGui::SetColumnOffset(3, group_w);

        ImGui::Checkbox("Bunny hop", &g_Options.misc_bhop);
        ImGui::Checkbox("Auto-Strafe##Movement", &g_Options.misc_autostrafe);
		ImGui::Checkbox("Third Person", &g_Options.misc_thirdperson);
		if(g_Options.misc_thirdperson)
			ImGui::SliderFloat("Distance", &g_Options.misc_thirdperson_dist, 0.f, 150.f);
        ImGui::Checkbox("No hands", &g_Options.misc_no_hands);
		ImGui::Checkbox("Rank reveal", &g_Options.misc_showranks);
		ImGui::Checkbox("Watermark##hc", &g_Options.misc_watermark);
        ImGui::SameLine();
        if (ImGui::Button("+##waremark"))
            ImGui::OpenPopup("##waterm");

        if (ImGui::BeginPopup("##waterm_col")) {
            ImGui::PushItemWidth(160.f);
            ImGui::ColorEdit3("Watermark", g_Options.color_watermark);
            ImGui::PopItemWidth();
            ImGui::EndPopup();
        }
        //ImGui::PushItemWidth(-1.0f);
		ImGui::NextColumn();
        ImGui::SliderInt("viewmodel_fov:", &g_Options.viewmodel_fov, 68, 120);
		ImGui::Text("Postprocessing:");
        ImGui::SliderFloat("Red", &g_Options.mat_ambient_light_r, 0, 1);
        ImGui::SliderFloat("Green", &g_Options.mat_ambient_light_g, 0, 1);
        ImGui::SliderFloat("Blue", &g_Options.mat_ambient_light_b, 0, 1);
        //ImGui::PopItemWidth();

        ImGui::Columns(1, nullptr, false);
        ImGui::PopStyleVar();
    }
    ImGui::EndGroupBox();
}
void RenderPlayerListTab()
{
    auto& style = ImGui::GetStyle();
    float group_w = ImGui::GetCurrentWindow()->Size.x - style.WindowPadding.x * 2;

    bool placeholder_true = true;

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::ToggleButton("PlayerList", &placeholder_true, ImVec2{ group_w, 25.0f });
    ImGui::PopStyleVar();


    //ImGui::PopStyleVar();

    ImGui::BeginGroupBox("##body_content");
    {
        char* playersNicknames[15];
        int playerIndxes[15];
        auto pLocal = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer()));
        int i = 0;

        if (g_EngineClient->IsInGame())
        {
            for (INT ax = 1; ax < g_EngineClient->GetMaxClients(); ++ax)
            {
                if (PlayerList[ax].pPlayer != NULL)
                {
                    if (PlayerList[ax].pPlayer->m_iTeamNum() != pLocal->m_iTeamNum())
                    {
                        player_info_t info = PlayerList[ax].pPlayer->GetPlayerInfo();
                        playersNicknames[i] = new char[(sizeof(info.szName) / sizeof(char)) + 1];
                        strcpy(playersNicknames[i], info.szName);
                        playerIndxes[i] = ax;
                        i++;
                    }

                }
            }

            if (PlayerList[playerIndxes[g_Options.selected_player_index]].pPlayer != NULL && playersNicknames[0] != nullptr)
            {
                ImGui::Checkbox("Friend", &PlayerList[playerIndxes[g_Options.selected_player_index]].bFriend);
                ImGui::Checkbox("High Priority", &PlayerList[playerIndxes[g_Options.selected_player_index]].bHighPriority);
                ImGui::Checkbox("Baim", &PlayerList[playerIndxes[g_Options.selected_player_index]].bBaim);
                const char* listbox_items32[] = { "None", "Down", "UP" };
                ImGui::Combo("Fix X ", &PlayerList[playerIndxes[g_Options.selected_player_index]].bFixX, listbox_items32, IM_ARRAYSIZE(listbox_items32));
                ImGui::Combo("Player ", &g_Options.selected_player_index, playersNicknames, i);
            }

        }
        else
        {
            ImGui::Text("Not Ingame.");
        }
        ImGui::EndGroupBox();
    }
}
void RenderAimTab()
{
    static char* aim_tab_names[] = { "Aimbot", "Anti-Aim" };
    static int   active_aim_tab = 0;

    bool placeholder_true = true;

    auto& style = ImGui::GetStyle();
    float group_w = ImGui::GetCurrentWindow()->Size.x - style.WindowPadding.x * 2;
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    {
        render_tabs(aim_tab_names, active_aim_tab, group_w / _countof(aim_tab_names), 25.0f, true);
    }
    ImGui::PopStyleVar();
    ImGui::BeginGroupBox("##body_content");
    {
        if (active_aim_tab == 0) {
            //start
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
            ImGui::Columns(3, nullptr, false);
            ImGui::SetColumnOffset(1, group_w / 3.0f);
            ImGui::SetColumnOffset(2, 2 * group_w / 3.0f);
            ImGui::SetColumnOffset(3, group_w);

            ImGui::BeginGroupBox("AimBot");
            {
                ImGui::Checkbox("Enabled", &g_Options.aim_enabled);
                ImGui::Checkbox("Auto Shoot", &g_Options.aim_auto_shoot);
                ImGui::SliderFloat("FOV", &g_Options.aim_fov, 1.f, 180.f);
                ImGui::Checkbox("Auto Crouch", &g_Options.auto_duck);
                ImGui::Checkbox("Auto Stop", &g_Options.auto_stop);
                ImGui::Checkbox("Auto Wall", &g_Options.auto_wall);
                ImGui::SliderFloat("Min Damage", &g_Options.auto_wall_mindamange, 1.0f, 100.0f);
            }
            ImGui::EndGroupBox();

            ImGui::NextColumn();

            ImGui::BeginGroupBox("Aimbot2");
            {
          //      ImGui::Checkbox("Silent", &g_Options.aim_silent_mode);
                ImGui::Checkbox("Friendly Fire", &g_Options.aim_friendly_fire);
                ImGui::Checkbox("Auto Scope", &g_Options.auto_scope);
                ImGui::Checkbox("Hitchance", &g_Options.hit_chance);
                ImGui::SliderFloat("Minimal Hitchance", &g_Options.hit_chanceamount, 1.f, 100.f);



                ImGui::NextColumn();
            }
            ImGui::EndGroupBox();

            ImGui::NextColumn();


            ImGui::BeginGroupBox("Predictions");
            {
                ImGui::Checkbox("Velocity-Prediction", &g_Options.vecvelocityprediction);
                ImGui::Checkbox("Multipoint", &g_Options.multipointer);
        //        ImGui::Checkbox("FoV Prediction", &g_Options.fovfix);

                ImGui::NextColumn();
            }
            ImGui::EndGroupBox();



            ImGui::Columns(1, nullptr, false);
            ImGui::PopStyleVar();
        }
        else if (active_aim_tab == 1) {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
            ImGui::Columns(3, nullptr, false);
            ImGui::SetColumnOffset(1, group_w / 3.0f);
            ImGui::SetColumnOffset(2, 2 * group_w / 3.0f);
            ImGui::SetColumnOffset(3, group_w);

       /*     ImGui::BeginGroupBox("Resolvers");
            {
                const char* listbox_items322[] = { "None", "AW", "Floted", "AW Floated", "Floated2" };
                ImGui::Combo("Pitch", &g_Options.resolverx, listbox_items322, IM_ARRAYSIZE(listbox_items322));
                const char* listbox_items32[] = { "None", "LBY Delta", "LBY Target", "LBY Target Delta", "Brute", "Brute2", "LBY Forces" };
                ImGui::Combo("Yaw", &g_Options.resolvery, listbox_items32, IM_ARRAYSIZE(listbox_items32));
            } */
            ImGui::EndGroupBox();

            ImGui::NextColumn();

            ImGui::BeginGroupBox("AntiAim");
            {
                const char* combobox_items[] = { "None", "Down", "Up", "Fake Jitter Down", "Fake Up", "Fake Zero", "Fake Down" };

                ImGui::PushItemWidth(-1);
                ImGui::Combo("Pitch", &g_Options.aax, combobox_items, IM_ARRAYSIZE(combobox_items));
                ImGui::PopItemWidth();

                const char* combobox_items2[] = { "None", "Fake Static", "Fake SideWays", "Fake Spin", "Fake Drop Inverse", "Fake Left Spin", "Backwards", "Backwards Dance", "Fake Backward Jitter", "SideWays Drop", "LBY", "Fake LBY", "Fake LBY2", "Fake LBY3", "Fake LBY4", "Fake LBY5", "Fake Static LBY", "Fake Zero", "Dynamic Tank" };

                ImGui::PushItemWidth(-1);
                ImGui::Combo("Yaw", &g_Options.aay, combobox_items2, IM_ARRAYSIZE(combobox_items2));
                ImGui::PopItemWidth();
                ImGui::Checkbox("At Targets", &g_Options.attarget);
                ImGui::Checkbox("Third Person AA", &g_Options.thpaa);

                ImGui::NextColumn();
            }
            ImGui::EndGroupBox();



            ImGui::Columns(1, nullptr, false);
            ImGui::PopStyleVar();
        }
    }
    ImGui::EndGroupBox();
}


void RenderConfigTab()
{
    static std::vector<std::string> configs;

    static auto load_configs = []() {
        std::vector<std::string> items = {};

        std::string path("C:\\Explicit\\Configs"); // Тут изменяем ( \\ после текста нинада)
        if (!fs::is_directory(path))
            fs::create_directories(path);

        for (auto& p : fs::directory_iterator(path))
            items.push_back(p.path().string().substr(path.length() + 1));

        return items;
    };

    static auto is_configs_loaded = false;
    if (!is_configs_loaded) {
        is_configs_loaded = true;
        configs = load_configs();
    }

    static std::string current_config;

    static char config_name[32];

    ImGui::InputText("##config_name", config_name, sizeof(config_name));
    ImGui::SameLine();
    if (ImGui::Button("Create")) {
        current_config = std::string(config_name);

        Config->Save(current_config + ".ini");
        is_configs_loaded = false;
        memset(config_name, 0, 32);
    }

    ImGui::ListBoxHeader("##configs");
    {
        for (auto& config : configs) {
            if (ImGui::Selectable(config.c_str(), config == current_config)) {
                current_config = config;
            }
        }
    }
    ImGui::ListBoxFooter();

    if (!current_config.empty()) {

        if (ImGui::Button("Load"))
            Config->Load(current_config);
        ImGui::SameLine();

        if (ImGui::Button("Save"))
            Config->Save(current_config);
        ImGui::SameLine();

        if (ImGui::Button("Delete") && fs::remove("C:\\Explicit\\Configs\\" + current_config)) { //Тут также изменяем на ваше, \\ обязательно
            current_config.clear();
            is_configs_loaded = false;
        }
        ImGui::SameLine();
    }
    if (ImGui::Button("Refresh"))
        is_configs_loaded = false;
}

void Menu::Initialize()
{
	CreateStyle();

    _visible = true;
}

void Menu::Shutdown()
{
    ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Menu::OnDeviceLost()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
}

void Menu::OnDeviceReset()
{
    ImGui_ImplDX9_CreateDeviceObjects();
}

void Menu::Render()
{
	ImGui::GetIO().MouseDrawCursor = _visible;

    if(!_visible)
        return;

    const auto sidebar_size = get_sidebar_size();
    static int active_sidebar_tab = 0;

    //ImGui::PushStyle(_style);

    ImGui::SetNextWindowPos(ImVec2{ 0, 0 }, ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2{ 1000, 400 }, ImGuiSetCond_Once);

	if (ImGui::Begin("CSGOSimple",
		&_visible,
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar)) {

		//auto& style = ImGui::GetStyle();
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));
        {
            ImGui::BeginGroupBox("##sidebar", sidebar_size);
            {
				//ImGui::GetCurrentWindow()->Flags &= ~ImGuiWindowFlags_ShowBorders;

                render_tabs(sidebar_tabs, active_sidebar_tab, get_sidebar_item_width(), get_sidebar_item_height(), false);
            }
            ImGui::EndGroupBox();
        }
        ImGui::PopStyleVar();
        ImGui::SameLine();

        // Make the body the same vertical size as the sidebar
        // except for the width, which we will set to auto
        auto size = ImVec2{ 0.0f, sidebar_size.y };

		ImGui::BeginGroupBox("##body", size);
        if(active_sidebar_tab == TAB_ESP) {
            RenderEspTab();
        } else if(active_sidebar_tab == TAB_AIMBOT) {
            RenderAimTab();
        } else if(active_sidebar_tab == TAB_MISC) {
            RenderMiscTab();
        } else if(active_sidebar_tab == TAB_CONFIG) {
            RenderConfigTab();
        }
        else if (active_sidebar_tab == TAB_PLAYERLIST) {
            RenderPlayerListTab();
        }
        ImGui::EndGroupBox();

        ImGui::TextColored(ImVec4{ 0.0f, 0.5f, 0.0f, 1.0f }, "FPS: %03d", get_fps());
        ImGui::SameLine(ImGui::GetWindowWidth() - 150 - ImGui::GetStyle().WindowPadding.x);
        if(ImGui::Button("Unload", ImVec2{ 150, 25 })) {
            g_Unload = true;
        }
        ImGui::End();
    }
}

void Menu::Toggle()
{
    _visible = !_visible;
}

void Menu::CreateStyle()
{
	ImGui::StyleColorsDark();
	ImGui::SetColorEditOptions(ImGuiColorEditFlags_HEX);
	_style.FrameRounding = 0.f;
	_style.WindowRounding = 0.f;
	_style.ChildRounding = 0.f;
	_style.Colors[ImGuiCol_Button] = ImVec4(0.260f, 0.590f, 0.980f, 0.670f);
	_style.Colors[ImGuiCol_Header] = ImVec4(0.260f, 0.590f, 0.980f, 0.670f);
	_style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.260f, 0.590f, 0.980f, 1.000f);
	//_style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.000f, 0.545f, 1.000f, 1.000f);
	//_style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.060f, 0.416f, 0.980f, 1.000f);
	_style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.30f, 1.0f);
	_style.Colors[ImGuiCol_WindowBg] = ImVec4(0.000f, 0.009f, 0.120f, 0.940f);
	_style.Colors[ImGuiCol_PopupBg] = ImVec4(0.076f, 0.143f, 0.209f, 1.000f);
	ImGui::GetStyle() = _style;
}

