if not "%1" == "" goto RETAIL

:DEBUG
nmake CVL=/co CVC="-Zi -Od" -f mm.mak
goto TheEnd

:RETAIL
nmake CVL= CVC=-Ox -f mm.mak

:TheEnd
