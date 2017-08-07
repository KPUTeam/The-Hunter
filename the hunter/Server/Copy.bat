xcopy	.\Engine\Bin_Debug\*.*	.\INGAMESERVER\Bin_Debug /d /y /s /exclude:Out.txt
xcopy	.\Engine\Bin\*.*  .*INGAMESERVER\Bin\ /d /y /s /exclude:Out.txt

xcopy	.\FBXSDK\lib\x86\debug\*.lib	.\INGAMESERVER\Bin_Debug\ /d /y /s /exclude:Out.txt
xcopy	.\FBXSDK\lib\x86\debug\*.dll	.\INGAMESERVER\Bin_Debug\ /d /y /s /exclude:Out.txt

xcopy	.\FBXSDK\lib\x86\release\.lib	.\INGAMESERVER\Bin\ /d /y /s /exclude:Out.txt	
xcopy	.\FBXSDK\lib\x86\release\.dll	.\INGAMESERVER\Bin\ /d /y /s /exclude:Out.txt