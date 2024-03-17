#pragma once

#include "util/noncopyable.hpp"
#include "sol/sol.hpp"
#include "bind/onebind.hpp"

#include <mutex>
#include <format>

namespace luabind
{
class LuaContext : util::Noncopyable
{
    static constexpr const char *CoreScripts[] = {
        "core\\init.lua",
    };

    struct version
    {
        static auto get() -> std::string { return std::string("0.0.1"); }
    };

    static void lua_print(sol::variadic_args va)
    {
        // TODO: add more options
        const char *integer_fmt = "%llu";

        auto it = va.begin();
        if (it->is<std::string>()) {
            auto s = it->get<std::string>();
            if (s == "#X") {
                integer_fmt = "%llx";
            }
            else {
                dprintf("%s", s.c_str());
            }
            it++;
        }

        for (; it != va.end(); it++) {
            if (it->is<std::string>()) {
                auto s = it->get<std::string>();
                dprintf("%s", s.c_str());
            }
            else if (it->is<lua_Integer>()) {
                dprintf(integer_fmt, it->get<lua_Integer>());
            }
            else if (it->is<lua_Number>()) {
                dprintf("%f", it->get<lua_Number>());
            }
            else if (it->is<bool>()) {
                dprintf("%s", it->get<bool>() ? "true" : "false");
            }
            else if (it->is<sol::table>()) {
                auto table = it->get<sol::table>();
                dprintf("table(%p)", (void *)table.pointer());
            }
            else if (it->is<sol::nil_t>()) {
                dprintf("%s", "<nil>");
            }
            else if (it->is<sol::function>()) {
                auto func = it->get<sol::function>();
                dprintf("function(%p)", func.pointer());
            }

            // format
            if (it != va.begin() && it != va.end()) {
                dprintf("\t");
            }
        }

        dprintf("\n");
    }

    static void lua_include(std::string path)
    {
        std::filesystem::path base_path;
        auto lua = LuaContext::instance();

        if (!lua->path_stack_.empty() && !lua->path_stack_.back().empty()) {
            base_path = lua->path_stack_.back().parent_path();
        }
        else {
            base_path = lua->base_path_;
        }

        lua->run_file(base_path / path);
    }

    static auto lua_exception_handler(lua_State *L,
                                      sol::optional<const std::exception &> maybe_exception,
                                      sol::string_view description) -> int
    {
        // L is the lua state, which you can wrap in a state_view if necessary
        // maybe_exception will contain exception, if it exists
        // description will either be the what() of the exception or a description saying that we hit the general-case catch(...)
        dprintf("An exception occurred in a function, here's what it says ");
        if (maybe_exception) {
            dprintf("(straight from the exception): ");
            const std::exception &ex = *maybe_exception;
            dprintf("%s", ex.what());
        }
        else {
            dprintf("(from the description parameter): ");
            dprintf("%s", description.data());
        }

        dprintf("\n");

        // you must push 1 element onto the stack to be
        // transported through as the error object in Lua
        // note that Lua -- and 99.5% of all Lua users and libraries -- expects a string
        // so we push a single string (in our case, the description of the error)
        return sol::stack::push(L, description);
    }

private:
    LuaContext() { }
    ~LuaContext() { }

public:
    [[nodiscard]] static auto has_inited() -> bool { return instance_ != nullptr; }

    [[nodiscard]] static auto instance() -> LuaContext *
    {
        std::call_once(signleton_flag_, [&]() { instance_ = new LuaContext; });
        return instance_;
    }

    static void destory()
    {
        if (instance_) {
            delete instance_;
            instance_ = nullptr;
        }
    }

public:
    void set_lua_base(std::string &path)
    {
        lua_.set_exception_handler(&lua_exception_handler);

        lua_.open_libraries();
        lua_.set_function("print", &lua_print);
        lua_.set_function("include", &lua_include);
        lua_.new_usertype<version>("version", "get", &version::get);
        lua_.set("IS_DEBUGGING", false);

        base_path_ = path;

        auto gpath = lua_["package"]["path"].get<std::string>();
        auto gcpath = lua_["package"]["cpath"].get<std::string>();

        lua_["package"]["path"] = gpath + ";" + path + "\\libs\\?.lua";
#if _WIN64
        lua_["package"]["cpath"] = gcpath + ";" + path + "\\libs\\dll\\x64\\?.dll";
#else
        lua_["package"]["cpath"] = gcpath + ";" + path + "\\libs\\dll\\x86\\?.dll";
#endif
        gcpath = lua_["package"]["cpath"].get<std::string>();

        lua_onebind(lua_);

        luaopen_x64dbg_bootstrap();

        // test throw
#if 0
        lua_.set_function("will_throw", [] {
            throw std::runtime_error("oh no not an exception!!!");
        });
        lua_.safe_script("will_throw()", &sol::script_pass_on_error);
#endif
    }

    void set_debug_state(bool state) { lua_.set("IS_DEBUGGING", state); }

    void auto_run()
    {
        std::vector<std::filesystem::path> loadQueue;
        auto autorun = base_path_ / "autorun";
        if (!std::filesystem::is_directory(autorun)) {
            dinfo("autorun is not a directory\n");
            return;
        }

        for (auto &v : std::filesystem::directory_iterator(autorun)) {
            auto p = v.path();
            if (std::filesystem::is_regular_file(p)) {
                if (!p.extension().compare(".lua")) {
                    loadQueue.push_back(p);
                }
            }
            else if (std::filesystem::is_directory(p)) {
                loadQueue.push_back(p / "init.lua");
            }
        }

        for (const auto &p : loadQueue) {
            run_file(std::move(p));
        }
    }

    auto run_script(std::string &&script) -> bool
    {
        path_stack_.emplace_back(std::filesystem::path{});
        auto pfr = lua_.safe_script(script, &sol::script_pass_on_error);
        path_stack_.pop_back();

        if (!pfr.valid()) {
            sol::error err = pfr;
            dinfo("%s\n", err.what());
            return false;
        }

        return true;
    }

    auto run_file(const std::filesystem::path &&path) -> bool
    {
        path_stack_.emplace_back(path);
        auto pfr = lua_.safe_script_file(path.string(), &sol::script_pass_on_error);
        path_stack_.pop_back();

        if (!pfr.valid()) {
            sol::error err = pfr;
            dinfo("%s\n", err.what());
            return false;
        }

        return true;
    }

private:
    void luaopen_x64dbg_bootstrap()
    {
        const uint32_t countCoreFiles = _countof(CoreScripts);
        dinfo("Loading %u core scripts\n", countCoreFiles);

        for (uint32_t i = 0; i < countCoreFiles; i++) {
            run_file(base_path_ / CoreScripts[i]);
        }
    }

private:
    sol::state lua_;
    std::filesystem::path base_path_;
    std::vector<std::filesystem::path> path_stack_;

private:
    static LuaContext *instance_;
    static std::once_flag signleton_flag_;
};

LuaContext *LuaContext::instance_ = nullptr;
std::once_flag LuaContext::signleton_flag_;

} // namespace luabind