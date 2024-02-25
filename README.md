# Developing-chat-application-introducing-synchronization-in-multithreaded-environment
This code is a messenger between clients, meaning that clients can text each other in a particular room. They can move from room to room and leave.
## Command 
- CREATE_ROOM >roomname<
- JOIN_ROOM >roomname< (Only if this room is created)
- SEND_MESSAGE >roomname< (Only if you are in this room)
- JOIN_ANOTHER_ROOM >roomname< (Only if this room is created)
- LEAVE_ROOM
## BYTES 
| Command to server | Server send answer |Client recv|
| --- | --- |--|
| CREATE_ROOM >roomname<| Sent 25 bytes to client.(Room created successfully)|Sent 13 bytes to server.|
| JOIN_ROOM >roomname< | Received 24 bytes from server.( Joined room successfully) |Sent 11 bytes to server.|
| SEND_MESSAGE >roomname<|Received 21 bytes from server + the length of the text you want to send  (Message from room 2:) |Sent 14 bytes to server + the length of the text you want to send |
| JOIN_ANOTHER_ROOM >roomname< | Received 46 bytes from server.(Left room successfully, Joined room successfully) |Sent 19 bytes to server.|
| LEAVE_ROOM | Received 22 bytes from server.(Left room successfully) |Sent 10 bytes to server.|
