//*********************************************************************************************************************
// Author:	  Karel Hala, hala.karel@gmail.com
// Copyright: (c) 2021 Karel Hala
//*********************************************************************************************************************

/**/#include "InputParams.h"
#include "input/FileReader.h"
#include "trade_book/TradeBook.h"
#include "tax/TaxFifo.h"*/
#include "BitstampTax.h"
//#include <iostream>

wxIMPLEMENT_APP(C_BitstampTax);
/*
int main (int argc, char * argv[])
{
	int retVal (EXIT_SUCCESS);

	C_InputParams params;
	if (params.Read(argc, argv))
	{
		C_FileReader fileReader (params.GetDataFileName());
		if (fileReader.Read())
		{
			C_TradeBook book;
			book.SetTradeItems(std::move(fileReader.MoveTradeItems()));
			std::ostringstream outputString;
			book.PrintData(outputString);
			std::cout << outputString.str();

			C_TaxFifo fifo;
			if (fifo.Process(book))
			{
				std::ostringstream taxString;
				fifo.PrintTax(taxString);
				std::cout << taxString.str();
				std::cout << "\n";
			}
			else
			{
				std::cout << fifo.GetErrorMsg();
				retVal = EXIT_FAILURE;
			}
		}
		else
		{
			std::cout << fileReader.GetErrorMsg();
			retVal = EXIT_FAILURE;
		}
	}
	else
	{
		std::cout << params.GetErrorMsg();
		retVal = EXIT_FAILURE;
	}

    return retVal;
}
*/