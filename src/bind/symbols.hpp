#pragma once
#include "sol/sol.hpp"
#include "pluginmain.h"

struct symbol_info
{
    std::string mod;
    duint rva;
    std::string name;
    bool manual;
    std::string type;
};

struct symbols
{
    static auto get_list(sol::this_state s) -> sol::object
    {
        sol::state_view lua(s);
        BridgeList<Script::Symbol::SymbolInfo> list;

        auto syms = lua.create_table();
        constexpr const char *symbol_type_str[] = {"function", "import", "export"};

        auto res = Script::Symbol::GetList(&list);
        if (res) {
            for (int32_t i = 0; i < list.Count(); i++) {
                auto info = list[i];
                syms[sol::create_if_nil][info.mod][info.name] =
                    symbol_info{info.mod, info.rva, info.name, info.manual, symbol_type_str[info.type]};
            }
        }
        return syms;
    }
};

void bind_symbols(sol::state &lua)
{
    lua.new_usertype<symbols>("symbols", "getList", &symbols::get_list);
    lua.new_usertype<symbol_info>("symbolInfo",
                                  "mod",
                                  &symbol_info::mod,
                                  "rva",
                                  &symbol_info::rva,
                                  "name",
                                  &symbol_info::name,
                                  "manual",
                                  &symbol_info::manual,
                                  "type",
                                  &symbol_info::type);
}
