# Developing-chat-application-introducing-synchronization-in-multithreaded-environment
This code is a messenger between clients, meaning that clients can text each other in a particular room. They can move from room to room and leave.
## Command 
- CREATE_ROOM >roomname<
- JOIN_ROOM >roomname< (Only if this room is created)
- SEND_MESSAGE >roomname< (Only if you are in this room)
- JOIN_ANOTHER_ROOM >roomname< (Only if this room is created)
- LEAVE_ROOM
## BYTES 
You can send a maximum of 4096 bytes, so here is how many bytes each command has, do not exceed the limit, good luck!
| Command to server | Server send answer |Client recv|
| --- | --- |--|
| CREATE_ROOM >roomname<| Sent 25 bytes to client.(Room created successfully)|Sent 13 bytes to server.|
| JOIN_ROOM >roomname< | Received 24 bytes from server.( Joined room successfully) |Sent 11 bytes to server.|
| SEND_MESSAGE >roomname<|Received 21 bytes from server + the length of the text you want to send  (Message from room 2:) |Sent 14 bytes to server + the length of the text you want to send |
| JOIN_ANOTHER_ROOM >roomname< | Received 46 bytes from server.(Left room successfully, Joined room successfully) |Sent 19 bytes to server.|
| LEAVE_ROOM | Received 22 bytes from server.(Left room successfully) |Sent 10 bytes to server.|

## CREATE_ROOM 
### USE CASE DIAGRAM
![image](https://github.com/DenysBzenko/Developing-chat-application-introducing-synchronization-in-multithreaded-environment/assets/119534908/fb081911-ae41-463a-8bc1-998f16a9b4df)\
### IN CODE 
![image](https://github.com/DenysBzenko/Developing-chat-application-introducing-synchronization-in-multithreaded-environment/assets/119534908/c96be1b4-8854-4f23-99ae-d26d008f71b6)
### CONSOLE
![image](https://github.com/DenysBzenko/Developing-chat-application-introducing-synchronization-in-multithreaded-environment/assets/119534908/da3a3b2c-2de6-4626-8020-d5cd1a41044c)


## JOIN_ROOM 
### USE CASE DIAGRAM
![image](https://github.com/DenysBzenko/Developing-chat-application-introducing-synchronization-in-multithreaded-environment/assets/119534908/dbcb42bc-966f-4a9b-a511-ca6c05831d7f)
### IN CODE 
![image](https://github.com/DenysBzenko/Developing-chat-application-introducing-synchronization-in-multithreaded-environment/assets/119534908/013819c8-5fbf-45e1-a35b-e1af3e3b109a)
### CONSOLE
![image](https://github.com/DenysBzenko/Developing-chat-application-introducing-synchronization-in-multithreaded-environment/assets/119534908/321eb9ad-74db-4f17-9c17-5b3a3b020ce8)

## SEND_MESSAGE
### USE CASE DIAGRAM
![image](https://github.com/DenysBzenko/Developing-chat-application-introducing-synchronization-in-multithreaded-environment/assets/119534908/56c862c4-3055-4bac-971e-ed64e546b1d7)
### IN CODE
![image](https://github.com/DenysBzenko/Developing-chat-application-introducing-synchronization-in-multithreaded-environment/assets/119534908/9919d24d-2d61-489c-b9a5-ef153855cd08)
### CONSOLE
![image](https://github.com/DenysBzenko/Developing-chat-application-introducing-synchronization-in-multithreaded-environment/assets/119534908/fe5a8f34-3cca-433d-9611-8109e9938729)

## JOIN_ANOTHER_ROOM 
### USE CASE DIAGRAM
![image](https://github.com/DenysBzenko/Developing-chat-application-introducing-synchronization-in-multithreaded-environment/assets/119534908/5ff5e1ca-85cd-40b6-81a5-efab8835f778)
### IN CODE
![image](https://github.com/DenysBzenko/Developing-chat-application-introducing-synchronization-in-multithreaded-environment/assets/119534908/ad5d7943-dd8e-4519-aea6-b4f65d6493eb)
### CONSOLE
![image](https://github.com/DenysBzenko/Developing-chat-application-introducing-synchronization-in-multithreaded-environment/assets/119534908/48fea1d8-8741-4d6d-8118-05301cf682bf)

## LAVE_ROOM 
### USE CASE DIAGRAM
![image](https://github.com/DenysBzenko/Developing-chat-application-introducing-synchronization-in-multithreaded-environment/assets/119534908/53fb0227-bf42-47b7-96d1-f132c669fd14)
### IN CODE 
![image](https://github.com/DenysBzenko/Developing-chat-application-introducing-synchronization-in-multithreaded-environment/assets/119534908/98420e72-95e3-476c-8b14-5560082e60c5)
### CONSOLE
![image](https://github.com/DenysBzenko/Developing-chat-application-introducing-synchronization-in-multithreaded-environment/assets/119534908/426bd286-4005-4aed-aec4-147d0ce9ca69)




