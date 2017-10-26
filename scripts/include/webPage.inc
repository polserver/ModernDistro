//Shinni's web page functions

Function OpenWebSite(who, url)
  var packet:="A5"+WordPacket(Len(url)+4);
  ForEach ascii_code in CAscZ(url)
    packet:=packet+BytePacket(ascii_code);
  EndForEach
  SendPacket(who, packet+"00");
EndFunction

Function BytePacket(byte)
  var paket:=Hex(byte);
  paket:="00"+paket[3, Len(paket)-2];
  Return (paket[Len(paket)-1, 2]);
EndFunction

Function WordPacket(word)
  var paket:=Hex(word);
  paket:="0000"+paket[3, Len(paket)-2];
  Return (paket[Len(paket)-3, 4]);
EndFunction

Function DoubleWordPacket(doubleword)
  var paket:=Hex(doubleword);
  paket:="00000000"+paket[3, Len(paket)-2];
  Return (paket[Len(paket)-7, 8]);
EndFunction