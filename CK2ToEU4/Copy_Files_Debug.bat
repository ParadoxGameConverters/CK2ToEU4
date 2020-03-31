echo on
rem Copy converter data files
copy "Data_Files\configuration.txt" "..\Debug\CK2ToEU4\configuration.txt"
copy "Data_Files\ReadMe.txt" "..\Debug\CK2ToEU4\readme.txt"
copy "Data_Files\ReadMe.txt" "..\Debug\readme.txt"
copy "Data_Files\FAQ.txt" "..\Debug\CK2ToEU4\FAQ.txt"
copy "Data_Files\CK2ToEU4DefaultConfiguration.xml" "..\Debug\Configuration\CK2ToEU4DefaultConfiguration.xml"
copy "Data_Files\SupportedConvertersDefault.xml" "..\Debug\Configuration\SupportedConvertersDefault.xml"
copy "Data_Files\license.txt" "..\Debug\CK2ToEU4\license.txt"
copy "Data_Files\after_converting.txt" "..\Debug\CK2ToEU4\after_converting.txt"

rem Create Configurables
del "..\Debug\CK2ToEU4\configurables" /Q
rmdir "..\Debug\CK2ToEU4\configurables" /S /Q
xcopy "Data_Files\configurables" "..\Debug\CK2ToEU4\configurables" /Y /E /I

rem Create Blank Mod
rem del "..\Debug\CK2ToEU4\blankMod" /Q
rem rmdir "..\Debug\CK2ToEU4\blankMod" /S /Q
rem xcopy "Data_Files\blankMod" "..\Debug\CK2ToEU4\blankMod" /Y /E /I

rem Copy Flags
rem del "..\Debug\CK2ToEU4\flags" /Q
rem rmdir "..\Debug\CK2ToEU4\flags" /S /Q
rem xcopy "Data_Files\flags" "..\Debug\CK2ToEU4\flags" /Y /E /I
