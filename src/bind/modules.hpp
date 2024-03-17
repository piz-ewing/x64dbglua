#pragma once
#include "sol/sol.hpp"
#include "pluginmain.h"

struct section_info
{
    duint addr;
    duint size;
    std::string name;
};

struct module_info
{
    duint base;
    duint size;
    duint entry;
    std::string name;
    std::string path;
    std::vector<section_info> sections;
};

struct modules
{
    static auto to_module_info(Script::Module::ModuleInfo &info) -> module_info
    {
        module_info mi{};
        mi.base = info.base;
        mi.size = info.size;
        mi.entry = info.entry;
        mi.name = info.name;
        mi.path = info.path;

        BridgeList<Script::Module::ModuleSectionInfo> list;
        if (Script::Module::SectionListFromAddr(info.base, &list)) {
            for (int32_t i = 0; i < list.Count(); i++) {
                const Script::Module::ModuleSectionInfo &info = list[i];
                section_info si{};
                si.addr = info.addr;
                si.size = info.size;
                si.name = info.name;
                mi.sections.emplace_back(si);
            }
        }

        return mi;
    }

    static inline auto to_info_object(sol::this_state s, Script::Module::ModuleInfo &info) -> sol::object
    {
        return sol::object(s, sol::in_place, to_module_info(info));
    }

    static auto get_main(sol::this_state s) -> sol::object
    {
        sol::state_view lua(s);
        Script::Module::ModuleInfo info = {};
        if (!Script::Module::GetMainModuleInfo(&info)) {
            return sol::lua_nil;
        }
        return to_info_object(s, info);
    }

    static auto find_by_address(sol::this_state s, duint addr) -> sol::object
    {
        Script::Module::ModuleInfo info = {};
        return Script::Module::InfoFromAddr(addr, &info) ? to_info_object(s, info) : sol::lua_nil;
    }

    static auto find_by_name(sol::this_state s, sol::string_view name) -> sol::object
    {
        Script::Module::ModuleInfo info = {};
        return Script::Module::InfoFromName(name.data(), &info) ? to_info_object(s, info) : sol::lua_nil;
    }

    static auto get_list(sol::this_state s) -> sol::object
    {
        sol::state_view lua(s);
        BridgeList<Script::Module::ModuleInfo> list;
        auto modules = lua.create_table();

        // FIXME: always return false
        bool res = Script::Module::GetList(&list);
        if (res) {
            for (int32_t i = 0; i < list.Count(); i++) {
                modules[list[i].name] = to_module_info(list[i]);
            }
        }
        return modules;
    }
};

void bind_modules(sol::state &lua)
{
    lua.new_usertype<modules>("modules",
                              "getMain",
                              &modules::get_main,
                              "findByAddress",
                              &modules::find_by_address,
                              "findByName",
                              &modules::find_by_name,
                              "getList",
                              &modules::get_list);

    lua.new_usertype<module_info>("moduleInfo",
                                  "base",
                                  &module_info::base,
                                  "size",
                                  &module_info::size,
                                  "entry",
                                  &module_info::entry,
                                  "name",
                                  &module_info::name,
                                  "path",
                                  &module_info::path,
                                  "sections",
                                  &module_info::sections);
    lua.new_usertype<section_info>(
        "sectionInfo", "addr", &section_info::addr, "size", &section_info::size, "name", &section_info::name);
}
