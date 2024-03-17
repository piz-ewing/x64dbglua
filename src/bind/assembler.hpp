#pragma once
#include "sol/sol.hpp"
#include "pluginmain.h"

struct assembler
{
    static auto encode(sol::this_state s, duint addr, std::string instr) -> sol::variadic_results
    {
        sol::variadic_results values;

        std::vector<uint8_t> dest(16);
        int size = 0;
        char error[MAX_ERROR_SIZE];

        if (Script::Assembler::AssembleEx(addr, dest.data(), &size, instr.c_str(), error)) {
            values.push_back(
                sol::object(s, sol::in_place, sol::string_view(reinterpret_cast<const char *>(dest.data()), size)));
            values.push_back(sol::lua_nil);
        }
        else {
            values.push_back(sol::lua_nil);
            values.push_back(sol::object(s, sol::in_place, error));
        }

        return values;
    }
};

void bind_assembler(sol::state &lua) { lua.new_usertype<assembler>("assembler", "encode", &assembler::encode); }
