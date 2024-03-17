xmake f -m debug -a x64
xmake -y
Copy-Item -Force build\windows\x64\debug\x64dbglua.dll x64dbg-plugin-path