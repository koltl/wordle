################################################################
# Wordle project                                               #
#Lead by Kolt Litrenta                                         #
#Developed by Kolt Litrenta, Hamad Ahmed, and Boyd Lenich      #
################################################################

-This project is a simple client server based implementation of the popular browser game wordle,
-it is implemented with a tcp server connection, and the gameplay is run on the client, while the server
-sends the word for the user to guess, if the user wants to play again they have the options to.

*COMPILING*
SERVER: To compile the server invoke the make server_run command
CLIENT: To compile the server invoke the make client_run command
ALL:  This compiles both the client and server at the same time, this is usefull when running both on the same system
  for debugging purposes, or if you don't have a friend to run the server.

*RUNNING*
SERVER: ./server <Optional port number>
CLIENT: ./client <IP or server the client is run on> <Optional port number>
  NOTE: The port number must be between 1024 and 65535

*LIBRARY*
The library we used is a static library, the application is fairly simple and does not 
  have any library functions requiring one another, as well having  the library code be
  compiled with the client server code does not add any time to it.
  
*PROTOCOL*
All data sent between the client and server is using a tcp connection, and is predetermined by the 
  programs, meaning a strong protocol did not need to be developed. The one we used adds a non-letter
  character to the end of every message sent, and the recieving function looks for that character and
  only uses what was before said character. The character in question being "<".
    
*ISSUES*
  

*TEAM REMARKS*
  
BOYD: I feel like our strongest point was being communicative, we would share our code consistently, and show eachother
  how it would run to get feedback and help  to better the project as a whole. Multiple times we would hit walls and stream
  our code over discord to find issues and come up with solutions for the final project. 
  
  
KOLT:
  
  
HAMAD:
  


