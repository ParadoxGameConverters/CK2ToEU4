echo on
rem Copy converter data files
copy "Data_Files\log.txt" "..\Release\CK2ToEU4\"
copy "Data_Files\configuration-example.txt" "..\Release\CK2ToEU4\"

mkdir "..\Release\Configuration"
copy "Data_Files\fronter-configuration.txt" "..\Release\Configuration\"
copy "Data_Files\fronter-options.txt" "..\Release\Configuration\"
copy "Data_Files\*.yml" "..\Release\Configuration\"

mkdir "..\Release\Docs"
copy "Data_Files\ReadMe.txt" "..\Release\Docs\"
copy "Data_Files\FAQ.txt" "..\Release\Docs\"
copy "Data_Files\license.txt" "..\Release\Docs\"
copy "Data_Files\after_converting.txt" "..\Release\Docs\"

rem Create Configurables
del "..\Release\CK2ToEU4\configurables" /Q
rmdir "..\Release\CK2ToEU4\configurables" /S /Q
xcopy "Data_Files\configurables" "..\Release\CK2ToEU4\configurables" /Y /E /I

rem Create Blank Mod
del "..\Release\CK2ToEU4\blankMod" /Q
rmdir "..\Release\CK2ToEU4\blankMod" /S /Q
xcopy "Data_Files\blankMod" "..\Release\CK2ToEU4\blankMod" /Y /E /I
