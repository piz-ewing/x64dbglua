---@meta _

---
--- current debug state
---
---@global
IS_DEBUGGING = false

---
--- include lua file
---
---@param filename? string
function include(filename) end

---@class version
version = {}

---@return string
function version.get() end

---@class register
register = {}

---@param reg string
---@return integer?
function register.read(reg) end

---@param reg string
---@param val integer
---@return boolean
function register.write(reg, val) end

---@class assembler
assembler = {}

---@param addr  integer
---@param instr string
---@return string?, string?
function assembler.encode(addr, instr) end

---@class modules
modules = {}

---@class sectionInfo
---@field addr          integer
---@field size          integer
---@field name          string

---@class moduleInfo
---@field base          integer
---@field size          integer
---@field entry         integer
---@field name          string
---@field path          string
---@field sections      sectionInfo

---@return moduleInfo?
function modules.getMain() end

---@param addr integer
---@return moduleInfo?
function modules.findByAddress(addr) end

---@param name string
---@return moduleInfo?
function modules.findByName(name) end

---@return table {moduleName = sectionInfo}
function modules.getList() end

---@class symbolInfo
---@field mod           string
---@field rva           integer
---@field name          string
---@field manual        boolean
---@field type          string

---@class symbols
symbols = {}

---@return table {moduleName = table{ functionName = symbolInfo}}
function symbols.getList() end

--@class bps
bps = {}

---@param addr integer
---@return boolean
function bps.add(addr) end

---@param addr integer
---@return boolean
function bps.remove(addr) end

---@param addr integer
---@return boolean
function bps.disable(addr) end

---@class labelInfo
---@field mod           string
---@field rva           integer
---@field text          string
---@field manual        boolean

--@class labels
labels = {}

---@param addr integer
---@param name string
---@param manual? boolean
---@return boolean
function labels.setName(addr, name, manual) end

---@param addr integer
---@return string?
function labels.getName(addr) end

---@param name string
---@return moduleInfo?
function labels.getInfo(name) end

---@param addr integer
---@param addrEnd? integer
---@return boolean
function labels.remove(addr, addrEnd) end

---@return labelInfo[]
function labels.getList() end

---@meta
-- Provides memory access operations for a 64-bit unsigned integer (qword) type.
-- @class qword_ptr
-- @field __index function: Reads data from memory.
-- @field __newindex function: Writes data to memory.

---@meta
-- Provides memory access operations for a 32-bit unsigned integer (dword) type.
-- @class dword_ptr
-- @field __index function: Reads data from memory.
-- @field __newindex function: Writes data to memory.

---@meta
-- Provides memory access operations for a 16-bit unsigned integer (word) type.
-- @class word_ptr
-- @field __index function: Reads data from memory.
-- @field __newindex function: Writes data to memory.

---@meta
-- Provides memory access operations for an 8-bit unsigned integer (byte) type.
-- @class byte_ptr
-- @field __index function: Reads data from memory.
-- @field __newindex function: Writes data to memory.

---@class memory
memory = {}

---@param addr string
---@param len  integer
---@return string?, integer
function memory.read(addr, len) end

---@param addr string
---@param buf  string
---@return integer
function memory.write(addr, buf) end

---@class debugger
debugger = {}

---@param wait? boolean
---@return boolean
function debugger.stepOver(wait) end

---@param wait? boolean
---@return boolean
function debugger.stepIn(wait) end

---@param wait? boolean
---@return boolean
function debugger.run(wait) end

---@return boolean
function debugger.isRunning() end

---@return boolean
function debugger.wait() end

---@param tid integer
---@return boolean
function debugger.switchThread(tid) end

---@param args table {executable, arguments, directory}
---@return boolean
function debugger.start(args) end

---@return boolean
function debugger.stop() end

---@param cmd string
---@param block? boolean
---@return boolean
function debugger.command(cmd, block) end

---@class xrefInfo
---@field addr      integer
---@field type      string

---@class xrefs
xrefs = {}

---@param block? boolean
---@return boolean
function xrefs.analyze(block) end

---@param addr integer
---@return xrefInfo[]
function xrefs.get(addr) end

---@param addr integer
---@param from integer
---@return boolean
function xrefs.add(addr, from) end

---@param addr integer
---@return boolean
function xrefs.remove(addr) end

---@class utils
utils = {}

---@param str string
---@param title string
---@return integer
function utils.alert(str, title) end

---@param str string
---@param title string
---@return boolean
function utils.question(str, title) end
