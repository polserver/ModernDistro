Polling a server
----------------
To receive a status reply, a client needs to send:
1. 4 bytes for the seed (anything works)
2. 0xF1, 0x00, 0x04, 0xFF

For an example of how to poll a shard, see example_pollstatus.py

Requirements
------------
This package requires the final release of POL 100.0.0. 
Anything older has a crash bug when calling DisconnectClient()
from within the packet hook.

If "DisconnectClient()" is removed from doServerList.src, this
package can work from POL99 onwards. But note that the client
must then disconnect by itself after receiving the reply.
