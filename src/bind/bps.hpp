#pragma once
#include "sol/sol.hpp"
#include "pluginmain.h"

struct bps
{
    static auto add(duint addr) -> bool { return Script::Debug::SetBreakpoint(addr); }
    static auto remove(duint addr) -> bool { return Script::Debug::DeleteBreakpoint(addr); }
    static auto disable(duint addr) -> bool { return Script::Debug::DisableBreakpoint(addr); }
};

void bind_bps(sol::state &lua)
{
    lua.new_usertype<bps>("bps", "add", &bps::add, "remove", &bps::remove, "disable", &bps::disable);
}
