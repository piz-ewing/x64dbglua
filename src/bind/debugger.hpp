#pragma once
#include "sol/sol.hpp"
#include "pluginmain.h"
#include "util/utils.hpp"

#include <format>

struct debugger
{
    static auto setp_over(std::optional<bool> wait_for_pause) -> bool
    {
        bool res = DbgCmdExecDirect("sto");

        if (res && wait_for_pause.value_or(true))
            _plugin_waituntilpaused();

        return res;
    }

    static auto setp_in(std::optional<bool> wait_for_pause) -> bool
    {
        bool res = DbgCmdExecDirect("sti");

        if (res && wait_for_pause.value_or(true))
            _plugin_waituntilpaused();

        return res;
    }

    static auto run(std::optional<bool> wait_for_pause) -> bool
    {
        bool res = DbgCmdExecDirect("run");

        if (res && wait_for_pause.value_or(true))
            _plugin_waituntilpaused();

        return res;
    }

    static auto is_running() -> bool { return DbgIsRunning(); }

    static auto wait() -> bool { return _plugin_waituntilpaused(); }

    static auto switch_thread(duint tid) -> bool
    {
        auto cmd = std::format("switchthread {:X}", tid);
        return DbgCmdExecDirect(cmd.c_str());
    }

    static auto start(sol::table args) -> bool
    {
        auto executable = args["executable"].get<std::string>();
        auto arguments = args["arguments"].get<std::string>();
        auto directory = args["directory"].get<std::string>();

        auto wait_for_pause = args["wait"].get<bool>();

        auto cmd = std::format("InitDebug \"{}\"", executable);

        if (!arguments.empty()) {
            cmd += std::format(",\"{}\"", util::replace_all(arguments, R"(")", R"(\")"));
        }

        if (!directory.empty()) {
            cmd += std::format(",\"{}\"", directory);
        }
        return DbgCmdExecDirect(cmd.c_str());
    }

    static auto stop() -> bool { return DbgCmdExecDirect("stop"); }

    static auto command(std::string_view cmd, std::optional<bool> block) -> bool
    {
        bool res;
        if (block.value_or(true))
            res = DbgCmdExecDirect(cmd.data());
        else
            res = DbgCmdExec(cmd.data());

        return res;
    }
};

void bind_debugger(sol::state &lua)
{
    lua.new_usertype<debugger>("debugger",
                               "stepOver",
                               &debugger::setp_over,
                               "stepIn",
                               &debugger::setp_in,
                               "run",
                               &debugger::run,
                               "isRunning",
                               &debugger::is_running,
                               "wait",
                               &debugger::wait,
                               "switchThread",
                               &debugger::switch_thread,
                               "start",
                               &debugger::start,
                               "stop",
                               &debugger::stop,
                               "command",
                               &debugger::command);
}
