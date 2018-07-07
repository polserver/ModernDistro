/* $Id: header.inc 1179 2008-02-06 05:42:30Z AustinHeilman $
 *
 */

use http;

function DoHeader(title:="")
	WriteHTML("<HEAD>");
	WriteHTML("<TITLE>"+title+"</TITLE>");
	WriteHTML("<LINK REL='stylesheet' TYPE='text/css' HREF='/stylesheets/main.css'>");
	WriteHTML("</HEAD>");

	WriteHTML("<BODY>");
	return 1;
endfunction

function DoFooter()
	WriteHTML("<DIV ID='footer'>");
	WriteHTML("<P>Copyright &copy; 2006-2008 - POL Development Team.</P>");
	WriteHTML("</DIV>");
	WriteHTML("</DIV>");
	WriteHTML("</BODY>");
	WriteHTML("</HTML>");

	return 1;
endfunction

function TableHeader(name:="")
	WriteHtmlRaw("<TABLE WIDTH='90%' CELLSPACING='1' CELLPADDING='2'>"
		+"<TR>"
		+"<TH VALIGN='TOP' ALIGN='LEFT' WIDTH='100%'><STRONG>"+name+"</STRONG></TH>"
		+"<TH WIDTH='100' VALIGN='TOP' ALIGN='CENTER' NOWRAP><A HREF='index.ecl'><FONT SIZE=1>ServMgmt Home</FONT></A></TH>"
		+"</TR>"
		+"</TABLE>");

	return 1;
endfunction
