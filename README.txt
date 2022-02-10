# BitstampTax

	The application computes annual profit/loss based on Bitstamp's transaction
history. The FIFO method is used.


Build steps:
	- Windows
		build wxWidgets somewhere (e.g. d:/sdks/wxWidgets_315)
			open wxWidgets/build/msw/wx_vc16.sln
			use batch build and select all configurations. Build...
			add sytem variable WXWIN" -> e.g. "d:/sdks/wxWidgets_315"
		create VS solution in BitstampTax/_build directory 
			mkdir _build
			cd _build
			cmake .. -G "Visual Studio 16 2019"
		
