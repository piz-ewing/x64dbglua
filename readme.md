<p align="center"><img src ="images/header.png" /></p>

[![Contributions Welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/piz-ewing/x64dbglua/issues)
[![GitHub release](https://img.shields.io/github/release/piz-ewing/x64dbglua.svg?label=latest+release&maxAge=60)](https://github.com/piz-ewing/x64dbglua/releases/latest)
[![Github Releases](https://img.shields.io/github/downloads/piz-ewing/x64dbglua/latest/total.svg?label=latest+downloads)](https://github.com/piz-ewing/x64dbglua/releases/latest)
[![Github All Releases](https://img.shields.io/github/downloads/piz-ewing/x64dbglua/total.svg?label=total+downloads)](https://github.com/piz-ewing/x64dbglua/releases)
[![GitHub stars](https://img.shields.io/github/stars/piz-ewing/x64dbglua.svg?style=flat&label=stars)](https://github.com/piz-ewing/x64dbglua/stargazers)
[![license](https://img.shields.io/badge/license-MIT-yellow.svg?style=flat)](https://github.com/piz-ewing/x64dbglua/blob/main/LICENSE)
[![Language](https://img.shields.io/badge/language-cpp-brightgreen)]()

`x64dbglua` is a plugin for `x64dbg` that provides `Lua` support. The plugin is  based on [`x64dbgPlaytime`](https://github.com/ZehMatt/x64dbgPlaytime) by `ZehMatt` and includes additional ideas and features. Currently, the main goal of `x64dbglua` is to complete automated binding, allowing users to access all functionalities of the `x64dbg` plugin SDK in `Lua`. Please note that the project is still in its early stages, and there may be many issues that need to be addressed and improved.


**Table Of Contents**

- [Features](#features)
- [Download](#download)
- [Installation](#installation)
- [Configuration](#configuration)
- [Usage](#usage)
  - [plugin guid](#plugin-guid)
  - [lua guide](#lua-guide)
- [Development](#development)
    - [Step 1 - Install Visual Studio C++ Build tools](#step-1---install-visual-studio-c-build-tools)
    - [Step 2 - Install Visual Studio and choose workload](#step-2---install-visual-studio-and-choose-workload)
    - [Step 3 - Install xmake](#step-3---install-xmake)
    - [Step 4 - Install Git for Windows](#step-4---install-git-for-windows)
    - [Step 5 - Clone this project](#step-5---clone-this-project)
    - [Step 6 - Build this project](#step-6---build-this-project)
    - [Other](#other)
- [To-Do Long-Term](#to-do-long-term)
- [Acknowledgments](#acknowledgments)
- [Contributing](#contributing)

## Features
Some of the key features and improvements comprise:

- Enhanced adherence to C++ style
- Improved readability and ease of debugging, along with enhanced extensibility
- Addition of more modules
- Integration with sol2
- Using xmake
- Automatic binding (support planned for the future)

## Download
Download [HERE](https://github.com/piz-ewing/x64dbglua/releases/latest) the latest release of the binaries

## Installation
1. Download the latest version of [x64dbg](https://github.com/x64dbg/x64dbg/releases/latest)
2. Extract `release.zip` to the `release` directory of `x64dbg`.
3. The `Lua` scripts are located in the `release` directory.

Tips: `release` is the directory where `x96dbg.exe` is located.

## Configuration
This will be provided in future versions.

## Usage
### plugin guid
1. Enter lua print("hello") in the command line.
2. Navigate to `plugins -> x64dbglua -> Load Lua File`.
3. Navigate to `plugins -> x64dbglua -> Reload File`.

### lua guide

- Adds `lua/libs` directory to the `package.path`.
- Adds `lua/libs/dll/[x86|x64]` directory to the `package.cpath`.
- Examples can be found in `lua/examples`.
- Reference is available in `lua/libs/buildin.lua`.
- Recommend using `stuartwang.luapanda` extension in `VSCode` as the lua debugger.

## Development
#### Step 1 - Install Visual Studio C++ Build tools

If you have installed Visual Studio, go to the next step.

Click the following link to go to the [Visual Studio C++ Build tools page](https://visualstudio.microsoft.com/zh-hans/visual-cpp-build-tools/).

#### Step 2 - Install Visual Studio and choose workload

At least, you should choose **Desktop development with C++** and **Universal Windows Platform development**, If you don't know what the other options do, leave them as default.

Please wait for the installation process to complete before proceeding to the next step.

#### Step 3 - Install xmake

Click the following link to go to the [xmake release page](https://github.com/xmake-io/xmake/releases), you should download the file which named **xmake-master.win64.exe**

#### Step 4 - Install Git for Windows

Click the following link to go to the [Git download page](https://git-scm.com/download/win)

#### Step 5 - Clone this project
```
git clone https://github.com/piz-ewing/x64dbglua.git
cd x64dbglua
git submodule update --init --recursive
```

#### Step 6 - Build this project
Now just run ```xmake -y```, xmake should do it all for you!

#### Other

- [tboox.xmake-vscode](https://xmake.io/mirror/plugin/more_plugins.html)
- gen vs project,  `xmake project -k vsxmake -m "bin,debug,minsizerel"`

## To-Do Long-Term
- Automatic binding
- Advanced interface
- Unit test

## Acknowledgments
- [x64dbgPlaytime](https://github.com/ZehMatt/x64dbgPlaytime)
- [sol2](https://github.com/ThePhD/sol2)
- [LuaPanda](https://github.com/Tencent/LuaPanda)
- [x64dbg](https://github.com/x64dbg/x64dbg)
- [lua](https://github.com/lua/lua)
- [luasocket](https://github.com/lunarmodules/luasocket)

## Contributing
All contributions are welcome!

Please use the [issue tracker](https://github.com/piz-ewing/x64dbglua/issues) to report any bugs or file feature requests.
