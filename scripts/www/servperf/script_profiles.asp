<%
use uo;
use os;
%>

<html>
<head>
<title>Script Profiles</title>
</head>

<body>

<% 
  include header; 
  PrintHeader( "Script Profiles" );
%>

<% 
  
  if (QueryParam( "Action" ) == "Clear Counters") 
      polcore().clear_script_profile_counters();
	    // even better would be to redirect, but we can't.
%>
<p><center><strong><a href="script_profiles.ecl">Reload this page without clearing</a></strong></center></p>
<%
  endif 
%>
<div align=center>
<table border=1 cellspacing=0 cellpadding=5>
  <tr bgcolor=#99CCFF>
    <td>Script Name</td>
    <td>Cycles</td>
    <td>Invocations</td>
    <td>Instr/Invoc</td>
	<td>%</td>
  </tr>

<%
  foreach script in (polcore().script_profiles)
%>
  <tr>
    <td><%=script.name%></td>
	<td><%=script.instr%></td>
    <td><%=script.invocations%></td>
	<td><%=script.instr_per_invoc%></td>
	<td><%=script.instr_percent%></td>
  </tr>
<%
    sleepms(1);
  endforeach
%>

</table>
</div>

<center><form><input type=submit name=Action Value="Clear Counters"></form></center>
</BODY>
</HTML>
