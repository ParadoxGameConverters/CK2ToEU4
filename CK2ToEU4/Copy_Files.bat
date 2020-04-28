echo on
rem Copy converter data files
mkdir "..\Release\Configuration"
copy "Data_Files\fronter-configuration.txt" "..\Release\Configuration\fronter-configuration.txt"
copy "Data_Files\fronter-options.txt" "..\Release\Configuration\fronter-options.txt"
copy "Data_Files\ReadMe.txt" "..\Release\readme.txt"

copy "Data_Files\log.txt" "..\Release\CK2ToEU4\log.txt"
copy "Data_Files\ReadMe.txt" "..\Release\CK2ToEU4\readme.txt"
copy "Data_Files\FAQ.txt" "..\Release\CK2ToEU4\FAQ.txt"
copy "Data_Files\license.txt" "..\Release\CK2ToEU4\license.txt"
copy "Data_Files\after_converting.txt" "..\Release\CK2ToEU4\after_converting.txt"

rem Create Configurables
del "..\Release\CK2ToEU4\configurables" /Q
rmdir "..\Release\CK2ToEU4\configurables" /S /Q
xcopy "Data_Files\configurables" "..\Release\CK2ToEU4\configurables" /Y /E /I

rem Create Blank Mod
del "..\Release\CK2ToEU4\blankMod" /Q
rmdir "..\Release\CK2ToEU4\blankMod" /S /Q
xcopy "Data_Files\blankMod" "..\Release\CK2ToEU4\blankMod" /Y /E /I

rem Copy Flags
rem del "..\Release\CK2ToEU4\flags" /Q
rem rmdir "..\Release\CK2ToEU4\flags" /S /Q
rem xcopy "Data_Files\flags" "..\Release\CK2ToEU4\flags" /Y /E /I
