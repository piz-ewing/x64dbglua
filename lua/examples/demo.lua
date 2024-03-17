local user32 = modules.findByName("user32.dll")
if user32 == nil then
    print("user32 == nil")
    return
end

print("#X", user32.base)

local syms = symbols.getList()
local symUser32 = syms["user32.dll"]
if symUser32 == nil then
    print("symUser32 == nil")
    return
end

local symFunction = symUser32["CreateWindowExW"]
if symFunction == nil then
    print("symFunction == nil")
    return
end

local addr = user32.base + symFunction.rva;
print("#X", addr)

local cmd = "dis " .. string.format("0x%X", addr)
debugger.command(cmd)

if xrefs.analyze(true) then
    local refs = xrefs.get(addr)

    print(#refs)

    for index, value in ipairs(refs) do
        print("#X", "index:", index, "addr", value.addr, "type", value.type)
        bps.add(value.addr);
    end
end
