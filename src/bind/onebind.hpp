#pragma once
#include "sol/sol.hpp"
#include "constants.hpp"
#include "registers.hpp"
#include "assembler.hpp"
#include "bps.hpp"
#include "debugger.hpp"
#include "xrefs.hpp"
#include "utils.hpp"
#include "labels.hpp"
#include "memory.hpp"
#include "modules.hpp"
#include "symbols.hpp"

void lua_onebind(sol::state &lua)
{
    bind_constants(lua);
    bind_registers(lua);
    bind_assembler(lua);
    bind_bps(lua);
    bind_debugger(lua);
    bind_xrefs(lua);
    bind_utils(lua);
    bind_labels(lua);
    bind_memory(lua);
    bind_modules(lua);
    bind_symbols(lua);
}
