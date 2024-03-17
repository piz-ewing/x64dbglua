#pragma once
#include "sol/sol.hpp"
#include "pluginmain.h"

struct xref_info
{
    duint addr;
    std::string type;
};

struct xrefs
{
    static auto analyze(std::optional<bool> block) -> bool
    {
        bool res;
        if (block.value_or(true))
            res = DbgCmdExecDirect("analx");
        else
            res = DbgCmdExec("analx");

        return res;
    }

    static auto get(sol::this_state s, duint addr) -> sol::object
    {
        std::vector<xref_info> result;

        constexpr const char *xref_type_str[] = {"none", "data", "jmp", "call"};

        XREF_INFO info{};
        if (DbgXrefGet(addr, &info)) {
            for (duint i = 0; i < info.refcount; i++) {
                result.emplace_back(xref_info{info.references[i].addr, xref_type_str[info.references[i].type]});
            }
        }
        return sol::object(s, sol::in_place, sol::as_table(result));
    }

    static auto add(duint addr, duint from) -> bool { return DbgXrefAdd(addr, from); }

    static auto remove(duint addr) -> bool { return DbgXrefDelAll(addr); }
};

void bind_xrefs(sol::state &lua)
{
    lua.new_usertype<xrefs>(
        "xrefs", "analyze", &xrefs::analyze, "get", &xrefs::get, "add", &xrefs::add, "remove", &xrefs::remove);

    lua.new_usertype<xref_info>("xrefInfo", "addr", &xref_info::addr, "type", &xref_info::type);
}
