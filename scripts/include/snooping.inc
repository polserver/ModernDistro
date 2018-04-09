include ":attributes:attributes";

function snoop(who, victim)
  victim := victim; // stops "var not used" ecompile warning
  SendSysMessage(who, "snooping not implemented yet");
  return;
endfunction