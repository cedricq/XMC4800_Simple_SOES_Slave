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
Add the following to your `~/.bashrc` so that the build process can find the necessary binaries. 
```
export PATH="PATH_TO_GCC_ARM/gcc-arm-none-eabi-10.3-2021.10/bin:$PATH"
source ~/.bashrc
```
```
sudo apt install ncurses5
mv vscode .vscode
mkdir build && cd build
cmake ..
make
```

## To run debugger - VS Code
1. In Visual Studio Code, open this project : ``File`` -> ``Open folder``
2. Plug in the XMC board with USB connection.
2. Press F5 to run debugger


## To build and run debugger - Eclipse

1. Install Eclipse Embedded CDT
https://projects.eclipse.org/projects/iot.embed-cdt

2. Install Jlink, GCC ARM, OpenOCD

2. Setup in the project options the MCU tools:

![image](https://user-images.githubusercontent.com/32091677/155189890-06e1cdce-28ce-4441-87dc-e178081a358d.png)

![image](https://user-images.githubusercontent.com/32091677/187872006-0ac2433f-b50c-43ef-8760-5caf0cf25b45.png)

![image](https://user-images.githubusercontent.com/32091677/187872082-cb9a6672-e042-40a9-b243-00427a708e31.png)

![image](https://user-images.githubusercontent.com/32091677/187872181-fb334a51-271e-40f2-99a9-ea66ce09dd4b.png)


3. Setup in the project options the build options:

![image](https://user-images.githubusercontent.com/32091677/187872553-90631c6d-809b-452d-a086-5248c7b851b4.png)

![image](https://user-images.githubusercontent.com/32091677/187872851-fc34c623-b7f5-4292-9b10-504c66052c14.png)

![image](https://user-images.githubusercontent.com/32091677/187873001-935776b4-76d0-4cf2-8fd5-91b2e4283ab0.png)


4. Setup the debug configuration:
![image](https://user-images.githubusercontent.com/32091677/155190736-10b3bfe0-a981-49ec-9660-5dd344130051.png)

![image](https://user-images.githubusercontent.com/32091677/155190837-181957dd-401f-4bb9-9d04-e77b9a64a7ea.png)

![image](https://user-images.githubusercontent.com/32091677/155190922-31d1abca-399b-42bc-a744-02f223c308c6.png)

