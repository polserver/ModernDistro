// None of these need to be changed unless the UO client is reprogrammed to use different packet structure...
// So these will never need to be changed.

CONST MSGTYPE_PARTY := 0xBF;
CONST SUBCMD_PARTY := 6;

CONST OFFSET_PARTY_MSGLEN := 1;
CONST OFFSET_PARTY_SUBCMD := 3;
CONST OFFSET_PARTY_SUBSUBCMD := 5;