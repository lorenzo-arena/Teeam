# __Teeam__

Teeam is a software to make project planning easier and faster.
You can organize everything you are working at using Gannt diagrams.

## To compile

 - project needs KD Chart libraries, you can find at [KDChart Homepage](https://www.kdab.com/development-resources/qt-tools/kd-chart/)
 - it is also based on Qt (tested in Windows 10, MinGW toolchain)
 - compile everything in 32bit configuration

## To deploy

You need to create a folder with this files:

 - Teeam.exe
 - kdchart2.dll
 - libEGL.dll
 - libgcc_s_dw2-1.dll
 - libstdc++-6.dll
 - libwinpthread-1.dll
 - Qt5Core.dll
 - Qt5Gui.dll
 - Qt5PrintSupport.dll
 - Qt5Svg.dll
 - Qt5Widgets.dll
 - Qt5Xml.dll
 - you also need to create a folder named "platforms" with the file qwindows.dll
 
 Or you can download the bin for windows from the latest release.
