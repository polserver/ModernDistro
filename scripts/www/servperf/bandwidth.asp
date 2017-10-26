<%
use os;
use uo;
%>

<HTML>
<BODY>
<% 
  include header; 
  PrintHeader( "Bandwidth Usage" );
%>

<table border=1 cellspacing=0 cellpadding=5>
  <tr bgcolor=#99CCFF>
    <td>Bytes Sent</td>
    <td>bps out</td>
    <td>Received</td>
    <td>bps in</td>
  </tr>

<% foreach elem in (GetGlobalProperty( "#:perfmon:bandwidth" )) %>
  <tr>
    <td><%=elem[2]%></td>
    <td><%=elem[2]/6%></td>
    <td><%=elem[1]%></td>
    <td><%=elem[1]/6%></td>
  </tr>
<% endforeach %>
</table>

</body>
</html>
