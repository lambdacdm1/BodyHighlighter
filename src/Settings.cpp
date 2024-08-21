#include "Settings.h"

void Settings::LoadSettings() noexcept
{
    logger::info("Loading settings");

    CSimpleIniA ini;

    ini.SetUnicode();
    ini.LoadFile(R"(.\Data\SKSE\Plugins\BodyHighlighter.ini)");

    debug_logging = ini.GetBoolValue("Log", "Debug");

    if (debug_logging) {
        spdlog::set_level(spdlog::level::debug);
        logger::debug("Debug logging enabled");
    }

    // Load settings

    hotkey     = ini.GetLongValue("General", "uHotkey");
    check_dist = ini.GetDoubleValue("General", "fDetectionDistance");

    use_toggle = ini.GetBoolValue("General", "bUseToggle");

    r_color = ini.GetDoubleValue("Color", "red");
    g_color = ini.GetDoubleValue("Color", "green");
    b_color = ini.GetDoubleValue("Color", "blue");
    alpha   = ini.GetDoubleValue("Color", "alpha");

    logger::info("Loaded settings");
    logger::info("");
}
