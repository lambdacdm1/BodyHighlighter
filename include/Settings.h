#pragma once

class Settings : public Singleton<Settings>
{
public:
    static void LoadSettings() noexcept;

    inline static bool debug_logging{};

    inline static std::uint32_t hotkey{};
    inline static double check_dist{};
    inline static bool          use_toggle{};

    inline static std::float_t  r_color{};
    inline static std::float_t  g_color{};
    inline static std::float_t  b_color{};
    inline static std::float_t  alpha{};
};
