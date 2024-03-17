#include "plugin.h"

#include "util/utils.hpp"
#include "luabind.hpp"

enum
{
    MENU_LOAD_LUA_FILE,
    MENU_RELOAD,
};

static bool cbLuaCommand(int argc, char *argv[])
{
    const char *luaCode = argv[0] + 4;

    dinfo("> %s\n", luaCode);

    return luabind::LuaContext::instance()->run_script(luaCode);
}

static duint exprZero(int argc, duint *argv, void *userdata) { return 0; }

PLUG_EXPORT void CBINITDEBUG(CBTYPE cbType, PLUG_CB_INITDEBUG *info)
{
    dinfo("Debugging of %s started!\n", info->szFileName);
    luabind::LuaContext::instance()->set_debug_state(true);
}

PLUG_EXPORT void CBSTOPDEBUG(CBTYPE cbType, PLUG_CB_STOPDEBUG *info)
{
    dputs("Debugging stopped!");
    luabind::LuaContext::instance()->set_debug_state(false);
}

PLUG_EXPORT void CBEXCEPTION(CBTYPE cbType, PLUG_CB_EXCEPTION *info)
{
    dinfo("ExceptionRecord.ExceptionCode: %08X\n", info->Exception->ExceptionRecord.ExceptionCode);
}

PLUG_EXPORT void CBPAUSEDEBUG(CBTYPE cbType, PLUG_CB_PAUSEDEBUG *info) { }

PLUG_EXPORT void CBRESUMEDEBUG(CBTYPE cbType, PLUG_CB_RESUMEDEBUG *info) { }

PLUG_EXPORT void CBSTEPPED(CBTYPE cbType, PLUG_CB_STEPPED *info) { }

PLUG_EXPORT void CBDEBUGEVENT(CBTYPE cbType, PLUG_CB_DEBUGEVENT *info)
{
    if (info->DebugEvent->dwDebugEventCode == EXCEPTION_DEBUG_EVENT) {
        dinfo("DebugEvent->EXCEPTION_DEBUG_EVENT->%.8X\n", info->DebugEvent->u.Exception.ExceptionRecord.ExceptionCode);
    }
}

void plugin_menu_load_lua_file(std::string &last_loaded)
{
    OPENFILENAMEW ofn = {};   // common dialog box structure
    wchar_t szFile[260] = {}; // if using TCHAR macros

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"Lua\0*.lua\0All\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn) == TRUE) {
        last_loaded = util::utf16_to_utf8(ofn.lpstrFile);
        luabind::LuaContext::instance()->run_file(last_loaded);
    }
}

void plugin_menu_reload(std::string &last_loaded)
{
    if (!last_loaded.empty()) {
        dinfo("Reload file %s\n", last_loaded.c_str());
        luabind::LuaContext::instance()->run_file(last_loaded);
    }
    else {
        dinfo("Reload empty\n");
    }
}

PLUG_EXPORT void CBMENUENTRY(CBTYPE cbType, PLUG_CB_MENUENTRY *info)
{
    static std::string last_load;
    switch (info->hEntry) {
    case MENU_LOAD_LUA_FILE:
        plugin_menu_load_lua_file(last_load);
        break;
    case MENU_RELOAD:
        plugin_menu_reload(last_load);
        break;
    default:
        break;
    }
}

bool cbLuaExecuteScript(const char *text)
{
    dinfo("> %s\n", text);
    return luabind::LuaContext::instance()->run_script(text);
}

void cbLuaScriptComplete(const char *text, char **entries, int *entryCount)
{
    if (entryCount)
        *entryCount = 0;
}

bool pluginScriptInit()
{
    SCRIPTTYPEINFO sti = {};
    sti.execute = cbLuaExecuteScript;
    sti.id = 'L' + 'u' + 'a' + '5' + '4';
    strcpy_s(sti.name, "lua54");

    GuiRegisterScriptLanguage(&sti);

    return true;
}

#if BIN
int main()
{
    if (luabind::LuaContext::has_inited()) {
        return false;
    }

    auto basePath = std::filesystem::current_path().string();
    dinfo("Base Path: %s\n", basePath.c_str());

    auto luaBasePath = util::path_join(basePath, "lua");
    dinfo("Lua Base Path: %s\n", luaBasePath.c_str());

    auto lua_ctx = luabind::LuaContext::instance();
    lua_ctx->set_lua_base(luaBasePath);
    dinfo("Lua context created\n");

    lua_ctx->auto_run();
    dinfo("Lua context auto_run\n");

    for (;;) {
        std::string script;

        std::cout << "lua>";
        std::getline(std::cin, script);
        if (!script.compare(":q"))
            break;
        lua_ctx->run_script(std::move(script));
    }
}
#endif

//Initialize your plugin data here.
bool pluginInit(PLUG_INITSTRUCT *initStruct)
{
    _plugin_registercommand(pluginHandle, "lua", cbLuaCommand, false);

    if (luabind::LuaContext::has_inited()) {
        return false;
    }

    auto basePath = util::get_x64dbg_base_path();
    dinfo("Base Path: %s\n", basePath.c_str());

    auto luaBasePath = util::path_join(basePath, "lua");
    dinfo("Lua Base Path: %s\n", luaBasePath.c_str());

    auto lua_ctx = luabind::LuaContext::instance();
    lua_ctx->set_lua_base(luaBasePath);
    dinfo("Lua context created\n");

    lua_ctx->auto_run();
    dinfo("Lua context auto_run\n");

    pluginScriptInit();

    return true; //Return false to cancel loading the plugin.
}

//Deinitialize your plugin data here.
void pluginStop() { luabind::LuaContext::destory(); }

//Do GUI/Menu related things here.
void pluginSetup()
{
    _plugin_menuaddentry(hMenu, MENU_LOAD_LUA_FILE, "&Load Lua File");
    _plugin_menuaddentry(hMenu, MENU_RELOAD, "&Reload");
}
