/////////////////////////////////////////////////////////////////////
//
// Program Name:
//
// Author:
//
// Date Created:
//
// Description:
//
// Parameters:
//
// Return Value:
//
/////////////////////////////////////////////////////////////////////

function HTTPSecurity()

	if (QueryIP() == "127.0.0.1")
		return 1;
	else
		return 0;
	endif

endfunction

function notAuthorizedMsg()

	//<!DOCTYPE html>
	WriteHTML("<html>");
	  WriteHTML("<head>");
		WriteHTML("<meta http-equiv='content-type' content='text/html; charset=UTF-8'>");
		WriteHTML("<title></title>");
	  WriteHTML("</head>");
	  WriteHTML("<body>");
		WriteHTML("<h1 style='   text-align: center;'><br>");
		WriteHTML("</h1>");
		WriteHTML("<h1 style='   text-align: center;'><br>");
		WriteHTML("</h1>");
		WriteHTML("<h1 style='   text-align: center;'><br>");
		WriteHTML("</h1>");
		WriteHTML("<h1 style='   text-align: center;'><br>");
		WriteHTML("</h1>");
		WriteHTML("<h1 style='   text-align: center;'>Oops! You seem to be lost.</h1>");
		WriteHTML("<p><br>");
		WriteHTML("</p>");
		WriteHTML("<h1 style=' text-align: center;'>You are not authorized to view this page.</h1>");
	  WriteHTML("</body>");
	WriteHTML("</html>");

endfunction

