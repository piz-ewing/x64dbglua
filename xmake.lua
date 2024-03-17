add_rules("mode.debug", "mode.minsizerel")

set_policy("build.ccache", false)

set_languages("c17", "cxx20")
set_exceptions("cxx")
set_runtimes(is_mode("debug") and "MTd" or "MT")

if is_mode("bin") then
    add_defines("DEBUG")
    set_symbols("debug")
    set_optimize("none")
end

target("liblua")
    set_kind("static")
    add_includedirs("3rd/lua")

    add_defines("MAKE_LIB")

    add_files("3rd/lua/onelua.c")

target("luasocket-core")
    set_kind("shared")

    add_deps("liblua")
    add_includedirs("3rd/Lua")

    add_defines("WIN32", "NDEBUG", "_WINDOWS", "_USRDLL", "_CRT_SECURE_NO_WARNINGS", "_WINDLL", "LUASOCKET_")
    add_includedirs("3rd/luasocket")

    local src = "3rd/luasocket/src/"

    add_files(src.."luasocket.c")
    add_files(src.."timeout.c")
    add_files(src.."buffer.c")
    add_files(src.."io.c")
    add_files(src.."auxiliar.c")
    add_files(src.."compat.c")
    add_files(src.."options.c")
    add_files(src.."inet.c")
    add_files(src.."except.c")
    add_files(src.."select.c")
    add_files(src.."tcp.c")
    add_files(src.."udp.c")

    if is_os("windows") then
        add_files(src.."wsocket.c")
        add_syslinks("ws2_32")
    end

target("luasocket-mime")
    set_kind("shared")

    add_deps("liblua")
    add_includedirs("3rd/Lua")


    add_defines("WIN32", "NDEBUG", "_WINDOWS", "_USRDLL", "_CRT_SECURE_NO_WARNINGS", "_WINDLL", "LUASOCKET_")
    add_includedirs("3rd/luasocket")
    add_files(src.."mime.c")
    add_files(src.."compat.c")

-- don't make it
-- target("lua")
--     set_kind("binary")

--     add_deps("liblua")
--     add_includedirs("3rd/Lua")

--     add_files("3rd/lua/lua.c")

target("x64dbglua")
    if is_mode("bin") then
        set_kind("binary")
        add_defines("BIN=1")
    else
        set_kind("shared")
    end

    add_deps("liblua")

    add_includedirs("3rd/Lua")
    add_includedirs("3rd/sol2/include")

    add_includedirs("3rd")
    add_linkdirs("3rd")

    add_syslinks("user32")
    add_syslinks("Comdlg32")

    add_defines("UNICODE", "_UNICODE")
    add_defines("SOL_ALL_SAFETIES_ON=1")

    -- add files
    add_files("src/*.cpp")

