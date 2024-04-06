# TechnologiesOfNetworkSecurity
## Lab02-TCP Chat Room using RSA
In our last lab(Lab01), I implemented a simple chat room based on TCP protocol, and use **DES encryption** algorithm to encrypt the communication content. I creatively designed it based on **Special Service and Agent Theme**, which I though would be much fun.
This time is a complete upgrade on our labcodes! Last time, the private key of DES encryption is written hard-codedly in our program. If there is a misunderstanding in the key exchange process, the communication content will be in fatal error. And due to the unsecure channel before the DES key exchange, the key may be intercepted by the attacker.
However, in our Lab01, I will use **RSA encryption** algorithm to encrypt the DES key, creating a temporary secure channel to exchange the DES key. Though the attacker can intercept the DES key, he cannot decrypt it without the private key of RSA. What's more, out of the highly cost of RSA encryption, **we only use RSA to encrypt the DES key, and still use DES to encrypt the communication content.**

* Platform: Linux
* Language: C++
* Theme: Special Service
It is welcome and open to all issues around the project
Feel free to contact me on ErwinZhou@outlook.com
*To be continued......*
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
                "${fileDirname}/../bin/${fileBasenameNoExtension}.exe"
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