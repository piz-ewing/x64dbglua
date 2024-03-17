#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>

#include <Windows.h>

namespace util
{
[[nodiscard]] auto local_codepage_to_utf16(std::string input) -> std::wstring
{
    size_t need = MultiByteToWideChar(CP_ACP, 0, input.c_str(), input.size(), 0, 0);
    wchar_t *wstr = static_cast<wchar_t *>(malloc(need + sizeof(wchar_t)));
    size_t used = MultiByteToWideChar(CP_ACP, 0, input.c_str(), input.size(), wstr, need);
    std::wstring result(wstr, used);
    free(wstr);
    return result;
}

[[nodiscard]] auto utf16_to_local_codepage(wchar_t *data, size_t len) -> std::string
{
    BOOL ignore;

    size_t need = WideCharToMultiByte(CP_ACP, 0, data, len, 0, 0, "?", &ignore);
    char *cstr = static_cast<char *>(malloc(need + 1));
    size_t used = WideCharToMultiByte(CP_ACP, 0, data, len, cstr, need, "?", &ignore);
    std::string result(cstr, used);
    free(cstr);
    return result;
}

[[nodiscard]] auto utf16_to_utf8(const std::wstring input) -> std::string
{
    BOOL ignore;

    size_t need = WideCharToMultiByte(CP_UTF8, 0, input.c_str(), input.size(), NULL, 0, NULL, NULL);
    char *cstr = static_cast<char *>(malloc(need + 1));
    size_t used = WideCharToMultiByte(CP_UTF8, 0, input.c_str(), input.size(), cstr, need, NULL, NULL);
    std::string result(cstr, used);
    free(cstr);
    return result;
}

[[nodiscard]] auto read_file(std::filesystem::path file_path) -> std::string
{
    try {
        std::ifstream stream;
        stream.open(file_path);
        if (!stream.is_open()) {
            return {};
        }

        stream.seekg(0, std::ios::end);
        size_t size = stream.tellg();
        std::string file_content(size, ' ');
        stream.seekg(0);
        stream.read(&file_content[0], size);
        return file_content;
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    return {};
}

[[nodiscard]] std::string replace_all(std::string str, const std::string &from, const std::string &to)
{
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

[[nodiscard]] inline auto self_module_handle() -> HMODULE
{
    MEMORY_BASIC_INFORMATION mbi;
    return ((::VirtualQuery(self_module_handle, &mbi, sizeof(mbi)) != 0) ? (HMODULE)mbi.AllocationBase
                                                                         : (HMODULE)INVALID_HANDLE_VALUE);
}

[[nodiscard]] inline auto real_module_path(bool main = true) -> std::wstring
{
    static const size_t INITIAL_BUFFER_SIZE = MAX_PATH;
    static const size_t MAX_ITERATIONS = 7;
    std::wstring ret;
    DWORD bufferSize = INITIAL_BUFFER_SIZE;
    auto module_handle = main ? NULL : self_module_handle();
    if (module_handle == (HMODULE)INVALID_HANDLE_VALUE) {
        return {};
    }

    for (size_t iterations = 0; iterations < MAX_ITERATIONS; ++iterations) {
        ret.resize(bufferSize);
        DWORD charsReturned = ::GetModuleFileNameW(module_handle, &ret[0], bufferSize);
        if (charsReturned < ret.length()) {
            ret.resize(charsReturned);
            return ret;
        }
        else {
            bufferSize *= 2;
        }
    }
    return {};
}

[[nodiscard]] static inline auto get_x64dbg_base_path() -> std::string
{
    auto module_filename = real_module_path();
    if (module_filename.empty()) {
        return {};
    }

    return std::filesystem::path(module_filename).parent_path().parent_path().string();
}

[[nodiscard]] static inline auto path_join(const std::string &base, const std::string &sub) -> std::string
{
    return (std::filesystem::path(base) / sub).string();
}

} // namespace util