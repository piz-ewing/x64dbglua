#pragma once
#include "sol/sol.hpp"
#include "pluginmain.h"

struct label_info
{
    std::string mod;
    duint rva;
    std::string text;
    bool manual;
};

struct labels
{
    static auto set_name(duint addr, sol::string_view name, std::optional<bool> manual) -> bool
    {
        if (Script::Label::Set(addr, name.data(), manual.value_or(false), false)) {
            GuiUpdateAllViews();
            return true;
        }
        return false;
    }

    static auto get_name(sol::this_state s, duint addr) -> sol::object
    {
        std::string name(MAX_LABEL_SIZE, 0);
        return Script::Label::Get(addr, name.data()) ? sol::object(s, sol::in_place, name) : sol::lua_nil;
    }

    static auto to_label_info(sol::this_state s, Script::Label::LabelInfo &info) -> label_info
    {
        label_info li;
        li.mod = info.mod;
        li.rva = info.rva;
        li.text = info.text;
        li.manual = info.manual;
        return li;
    }

    static inline auto to_info_object(sol::this_state s, Script::Label::LabelInfo &info) -> sol::object
    {
        return sol::object(s, sol::in_place, to_label_info(s, info));
    }

    static auto get_info(sol::this_state s, duint addr) -> sol::object
    {
        Script::Label::LabelInfo info = {};
        return Script::Label::GetInfo((duint)addr, &info) ? to_info_object(s, info) : sol::lua_nil;
    }

    static auto remove(duint addr, std::optional<duint> addrEnd) -> bool
    {
        bool res = false;

        if (addrEnd.has_value()) {
            Script::Label::DeleteRange(addr, addrEnd.value());
            res = true; // Hmm
        }
        else {
            res = Script::Label::Delete(addr);
        }

        if (res) {
            GuiUpdateAllViews();
        }

        return res;
    }

    static auto get_list(sol::this_state s) -> sol::object
    {
        BridgeList<Script::Label::LabelInfo> list;
        std::vector<label_info> result;

        if (Script::Label::GetList(&list)) {
            for (int32_t i = 0; i < list.Count(); i++) {
                result.emplace_back(to_label_info(s, list[i]));
            }
        }

        return sol::object(s, sol::in_place, sol::as_table(result));
    }
};

void bind_labels(sol::state &lua)
{
    lua.new_usertype<labels>("labels",
                             "setName",
                             &labels::set_name,
                             "getName",
                             &labels::get_name,
                             "getInfo",
                             &labels::get_info,
                             "remove",
                             &labels::remove,
                             "getList",
                             &labels::get_list);

    lua.new_usertype<label_info>("labelInfo",
                                 "mod",
                                 &label_info::mod,
                                 "rva",
                                 &label_info::rva,
                                 "text",
                                 &label_info::text,
                                 "manual",
                                 &label_info::manual);
}
