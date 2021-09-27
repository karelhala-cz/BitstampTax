# BitstampTax

Build steps:
	- Windows
		build wxWidgets library
			open 3rd/wxWidgets/build/msw/wx_vc16.sln
				build both Release and Debug configuration (no DLL)
		create VS solution in BitstampTax/_build directory 
			mkdir _build
			cd _build
			cmake .. -G "Visual Studio 16 2019"
		
