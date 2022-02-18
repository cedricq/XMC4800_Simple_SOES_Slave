# XMC4800_Simple_SOES_Slave - EtherCAT

![image](https://user-images.githubusercontent.com/32091677/154466757-94ec70eb-4ea5-4070-bedc-38739e0c68ee.png)

# Debugger (Visual Studio Code)

## Requirements
1. Visual Studio Code: [link](https://code.visualstudio.com/)
2. GNU ARM Embedded tool-chain: [link](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
3. J-Link: [link](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack)
4. Visual Studio Code Plugins:
 + C/C++ for Visual Studio Code:[link](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
 + Cortex-Debug:[link](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug)
 + C/C++ Intellisense: [link](https://marketplace.visualstudio.com/items?itemName=austin.code-gnu-global) optional

## Build steps
```
sudo apt install ncurses5
mv vscode .vscode
mkdir build && cd build
cmake ..
make
```

## To run debugger
1. In Visual Studio Code, open this project : ``File`` -> ``Open folder``
2. Plug in the XMC board with USB connection.
2. Press F5 to run debugger
