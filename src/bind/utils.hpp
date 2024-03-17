#pragma once
#include "sol/sol.hpp"
#include "pluginmain.h"

struct utils
{
    static auto alert(sol::string_view str, sol::string_view title) -> int
    {
        return MessageBoxA(nullptr, str.data(), title.data(), MB_OK | MB_ICONWARNING);
    }

    static auto question(sol::string_view str, sol::string_view title) -> bool
    {
        return MessageBoxA(nullptr, str.data(), title.data(), MB_YESNO | MB_ICONWARNING) == IDYES;
    }
};

void bind_utils(sol::state &lua)
{
    lua.new_usertype<utils>("utils", "alert", &utils::alert, "question", &utils::question);
}
