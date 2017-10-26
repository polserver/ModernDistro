<% use uo; %>
<html>

<head>
<title>POL Server Performance Center</title>
</head>

<%
if (QueryParam( "PriDiv" ))
    polcore().set_priority_divide( CInt(QueryParam("PriDiv")) );
endif
%>

<body>

<p>POL Server Performance Center</p>

<p>Server Vitals:</p>

<table border="1" width="100%">
  <tr>
    <td width="50%">System Load:</td>
    <td width="50%"><%=polcore().sysload%>&nbsp;(<%=polcore().sysload_severity%>)</td>
  </tr>
  <tr>
    <td width="50%">Mobiles:</td>
    <td width="50%"><%=polcore().mobilecount%></td>
  </tr>
  <tr>
    <td width="50%">Toplevel Items:</td>
    <td width="50%"><%=polcore().itemcount%></td>
  </tr>
  <tr>
    <td width="50%">Online Players:</td>
    <td width="50%"><%=EnumerateOnlineCharacters().size()%></td>
  </tr>
  <tr>
    <td width="50%">Events per Minute:</td>
    <td width="50%"><%=polcore().events_per_min%></td>
  </tr>
  <tr>
    <td width="50%">Script Instructions per Minute:</td>
    <td width="50%"><%=polcore().instr_per_min%></td>
  </tr>
  <tr>
    <td width="50%">Bytes Sent:</td>
    <td width="50%"><%=polcore().bytes_sent%></td>
  </tr>
  <tr>
    <td width="50%">Bytes Received:</td>
    <td width="50%"><%=polcore().bytes_received%></td>
  </tr>
  <tr>
    <td width="50%">Priority Divider:</td>
    <td width="50%"><form><input type=text name=PriDiv value="<%=polcore().priority_divide%>"></form></td>
  </tr>
</table>

<ul>
<li><a href="npcgroup.ecl">NPC Grouping</a>
<li><a href="script_profiles.ecl">Script Profiles</a>
<li><a href="running_scripts.ecl">Running Scripts</a>
<li><a href="long_running_scripts.ecl">Long Running Scripts</a>
<li><a href="bandwidth.ecl">Bandwidth Usage</a>
</ul>
</body>
</html>
