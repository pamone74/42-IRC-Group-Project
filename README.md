## Usage
Basically, you need to run two terminals. One for server and one for client.

For Server, you have makefile at your disposal, just run make, and execute the sever by ./ircserv it will be waiting for client connection

For clien,  compile by c++ Client.cpp -o client the execute ./client: 


Then simple message will be sent to server and will be printed on the terminal.


openn two terminals
terminal 1:
make
./ircserv 2020 123456
--------Server ----------------
Server IP address: 0.0.0.0
Server listening on port 2020
Server 3 Connection Established

terminal 2:
nc localhost 2020

terminal 1:
--------Server ----------------
Server IP address: 0.0.0.0
Server listening on port 2020
Server 3 Connection Established
4 Connected 

terminal 2:
123456
Authentication successful!

terminal 1:
--------Server ----------------
Server IP address: 0.0.0.0
Server listening on port 2020
Server 3 Connection Established
4 Connected 
....Clients Data....123456

Server stored password: '123456'

terminal 2:
nc localhost 2020
123456
Authentication successful!
hello

terminal 1:
--------Server ----------------
Server IP address: 0.0.0.0
Server listening on port 2020
Server 3 Connection Established
4 Connected 
....Clients Data....123456

Server stored password: '123456'
....Clients Data....hello


and so on.....

to exit ctrl+c
