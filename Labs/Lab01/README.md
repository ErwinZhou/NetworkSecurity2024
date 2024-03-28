# Lab01-TCP Chat Room using DES Encryption
## Important Details
### Task.json
```
{
    "tasks": [
        {
            "type": "cppbuild",
            "label": "C/C++: g++-11 生成活动文件",
            "command": "/usr/bin/g++-11",
            "args": [
                "-fdiagnostics-color=always",
                "-g",
                "${file}",
                "~/lab/labcodes/TechnologiesOfNetworkSecurity/Labs/Lab01/src/utils.cpp",
                "~/lab/labcodes/TechnologiesOfNetworkSecurity/Labs/Lab01/src/DES.cpp",
                "~/lab/labcodes/TechnologiesOfNetworkSecurity/Labs/Lab01/src/agent.cpp",
                "-I",
                "~/lab/labcodes/TechnologiesOfNetworkSecurity/Labs/Lab01/include",
                "-o",
                "${fileDirname}/../bin/${fileBasenameNoExtension}.exe",
                "-lpthread"
            ],
            "options": {
                "cwd": "${fileDirname}"
            },
            "problemMatcher": [
                "$gcc"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "detail": "调试器生成的任务。"
        },
        {
            "type": "cppbuild",
            "label": "C/C++: g++-11 build active file",
            "command": "/usr/bin/g++-11",
            "args": [
                "-fdiagnostics-color=always",
                "-g",
                "${file}",
                "-o",
                "${fileDirname}/${fileBasenameNoExtension}"
            ],
            "options": {
                "cwd": "${fileDirname}"
            },
            "problemMatcher": [
                "$gcc"
            ],
            "group": "build",
            "detail": "compiler: /usr/bin/g++-11"
        }
    ],
    "version": "2.0.0"
}
```
### c_cpp_properties.json
```
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "${workspaceFolder}/**",
                "~/lab/labcodes/TechnologiesOfNetworkSecurity/Labs/Lab01/include",
                "/usr/include"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/clang",
            "cStandard": "c17",
            "cppStandard": "c++14",
            "intelliSenseMode": "linux-clang-x64"
        }
    ],
    "version": 4
}
```