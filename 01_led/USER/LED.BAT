SET PATH=D:\Applications\30_keil\ARM\ARMCC\Bin;C:\Program Files (x86)\Common Files\Oracle\Java\javapath;D:\WorkSpace\softwares\vmware\bin\;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;D:\Applications\19_git\Git\cmd;D:\WorkSpace\softwares\mingw\x86_64-8.1.0-release-win32-seh-rt_v6-rev0\mingw64\bin;C:\Program Files\Bandizip\;C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\Program Files\NVIDIA Corporation\NVIDIA NvDLISR;C:\Users\Manfred\AppData\Local\Microsoft\WindowsApps;D:\WorkSpace\softwares\vscode\Microsoft VS Code\bin;D:\WorkSpace\softwares\mingw\x86_64-8.1.0-release-win32-seh-rt_v6-rev0\mingw64\bin;
SET ARMCC5_ASMOPT=--diag_suppress=9931
SET ARMCC5_CCOPT=--diag_suppress=9931
SET ARMCC5_LINKOPT=--diag_suppress=9931
SET CPU_TYPE=STM32F407VE
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=LED
SET CPU_CLOCK=0x017D7840
"D:\Applications\30_keil\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\main.__i"
"D:\Applications\30_keil\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f4xx_it.__i"
"D:\Applications\30_keil\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\system_stm32f4xx.__i"
"D:\Applications\30_keil\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\led.__i"
"D:\Applications\30_keil\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\delay.__i"
"D:\Applications\30_keil\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\sys.__i"
"D:\Applications\30_keil\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\usart.__i"
"D:\Applications\30_keil\ARM\ARMCC\Bin\ArmAsm" --Via "..\obj\startup_stm32f40_41xxx._ia"
"D:\Applications\30_keil\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\misc.__i"
"D:\Applications\30_keil\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f4xx_gpio.__i"
"D:\Applications\30_keil\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f4xx_rcc.__i"
"D:\Applications\30_keil\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f4xx_syscfg.__i"
"D:\Applications\30_keil\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f4xx_usart.__i"
"D:\Applications\30_keil\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\croutine.__i"
"D:\Applications\30_keil\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\event_groups.__i"
"D:\Applications\30_keil\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\list.__i"
"D:\Applications\30_keil\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\queue.__i"
"D:\Applications\30_keil\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stream_buffer.__i"
"D:\Applications\30_keil\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\tasks.__i"
"D:\Applications\30_keil\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\timers.__i"
"D:\Applications\30_keil\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\heap_4.__i"
"D:\Applications\30_keil\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\port.__i"
"D:\Applications\30_keil\ARM\ARMCC\Bin\ArmLink" --Via "..\OBJ\LED.lnp"
"D:\Applications\30_keil\ARM\ARMCC\Bin\fromelf.exe" "..\OBJ\LED.axf" --i32combined --output "..\OBJ\LED.hex"
