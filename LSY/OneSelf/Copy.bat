cd..
cd..

xcopy .\Dx11Engine\Include\*.h .\Engine\Include\ /d /y /s

xcopy .\Dx11Engine\Bin_Debug\*.* .\Engine\Bin_Debug\ /d /y /s /exclude:Out.txt
xcopy .\Dx11Engine\Bin\*.* .\Engine\Bin\ /d /y /s /exclude:Out.txt

xcopy .\Dx11Engine\Bin_Debug\*.* .\Client\Bin_Debug\ /d /y /s /exclude:Out.txt
xcopy .\Dx11Engine\Bin_Debug\Shader\*.* .\Dx11Engine\Bin\Shader\ /d /y /s /exclude:Out.txt
xcopy .\Dx11Engine\Bin\*.* .\Client\Bin\ /d /y /s /exclude:Out.txt