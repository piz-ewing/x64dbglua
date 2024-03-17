#pragma once
#include "sol/sol.hpp"

void bind_constants(sol::state &lua)
{
    lua.set("EFL_C", 1 << 0);
    lua.set("EFL_P", 1 << 2);
    lua.set("EFL_A", 1 << 4);
    lua.set("EFL_Z", 1 << 6);
    lua.set("EFL_S", 1 << 7);
    lua.set("EFL_T", 1 << 8);
    lua.set("EFL_I", 1 << 9);
    lua.set("EFL_D", 1 << 10);
    lua.set("EFL_O", 1 << 11);
}