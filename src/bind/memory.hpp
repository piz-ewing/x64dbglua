#pragma once
#include "sol/sol.hpp"
#include "pluginmain.h"

struct memory
{
    static auto read(sol::this_state s, duint addr, duint len) -> sol::variadic_results
    {
        sol::variadic_results result;

        std::vector<uint8_t> buf(len);

        duint read = 0;
        if (!Script::Memory::Read(addr, buf.data(), len, &read)) {
            result.push_back(sol::lua_nil);
            result.push_back(0);
        }
        else {
            result.push_back(
                sol::object(s, sol::in_place, sol::string_view(reinterpret_cast<const char *>(buf.data()), read)));
            result.push_back(sol::object(s, sol::in_place, read));
        }

        return result;
    }

    static auto write(duint addr, sol::string_view buf) -> duint
    {
        duint written = 0;
        return Script::Memory::Write(addr, buf.data(), buf.size(), &written) ? written : 0;
    }
};

template <typename T> struct memory_access
{
    static auto read(sol::this_state s, memory_access<T> &ref, duint addr) -> sol::object
    {
        T res = 0;
        duint read = 0;
        return Script::Memory::Read(addr, &res, sizeof(T), &read) || read != sizeof(T)
                   ? sol::object(s, sol::in_place, res)
                   : sol::lua_nil;
    }

    static void write(sol::this_state s, memory_access<T> &ref, duint addr, duint val)
    {
        T wv = static_cast<T>(val);
        duint written = 0;
        if (!Script::Memory::Write(addr, &wv, sizeof(T), &written) || written != sizeof(T)) {
            // HMM
        }

        GuiUpdateMemoryView();
        GuiUpdateDisassemblyView();
    }
};

// FIXME: Requires verification
void bind_memory(sol::state &lua)
{
    lua.new_usertype<memory>("memory", "read", &memory::read, "write", &memory::write);

    // Meta
    lua.new_usertype<memory_access<uint64_t>>("qword_ptr",
                                              sol::meta_function::index,
                                              &memory_access<uint64_t>::read,
                                              sol::meta_function::new_index,
                                              &memory_access<uint64_t>::write);

    lua.new_usertype<memory_access<uint32_t>>("dword_ptr",
                                              sol::meta_function::index,
                                              &memory_access<uint32_t>::read,
                                              sol::meta_function::new_index,
                                              &memory_access<uint32_t>::write);

    lua.new_usertype<memory_access<uint16_t>>("word_ptr",
                                              sol::meta_function::index,
                                              &memory_access<uint16_t>::read,
                                              sol::meta_function::new_index,
                                              &memory_access<uint16_t>::write);

    lua.new_usertype<memory_access<uint8_t>>("byte_ptr",
                                             sol::meta_function::index,
                                             &memory_access<uint8_t>::read,
                                             sol::meta_function::new_index,
                                             &memory_access<uint8_t>::write);
}
