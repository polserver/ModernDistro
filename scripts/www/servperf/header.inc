function PrintHeader( name )
  WriteHtmlRaw( "<table border=0 width=\"100%\""
      +"<tr>"
      +"<td valign=\"top\" width=\"100%\"><strong><big>" + name + "</big></strong></td>"
      +"<td width=\"50%\" valign=\"top\" align=\"right\" nowrap><a href=\"index.ecl\"><small>Performance<br>"
      +"Home</small></a></td>"
      +"</tr>"
      +"</table>" );
endfunction