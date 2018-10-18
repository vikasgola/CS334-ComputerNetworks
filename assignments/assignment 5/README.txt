## Src
"src" folder contains all required Programs for assignments. 
Files names are according to the question number e.g. program required in question 2 is written in q2.c file.

## Exectuable
All exectuable files of all programs of src folder are in folder "exectuable" with their respective name.

## Tcpdump
contains all tcpdump captured packets in file named respective of question number.


## How to run
q2.c : run "./q2" on terminal and it will ask for your ip address and shows you what has been stored in sockaddr structure.

q3.1.c : run "./q3.1" on terminal and it will tell you which machine your running on Little Endian or Big Endian.

q3.2.c : run "./q3.2" on terminal and it will show you bytes before change byte order and after changing byte order.

q4.c : run "./q4" on terminal and it will print output to terminal.

q5.c : run "./q5 ip ipaddress1 ipaddress2 ipaddress3" or ""./q5 name hostname1 hostname2 hostname3" on terminal and 
        in first case it will give you corresponding hostname of each given ipaddress and
        in second case it will give you corresponding ipaddress of each given hostname.

q6.tcpclient.c : run "./q6.tcpclient 127.0.0.1" on terminal and it will try to connect to tcp daytime server on port 9923
        which is written in q6.tcpserver.c. After connecting it will show you hostname and port number of server with the time requested from server.

q6.tcpserver.c : run "./q6.tcpserver" on terminal and it will start service which provide daytime on port 9923.

q6.udpclient.c : run "./q6.udpclient 127.0.0.1" on terminal and it will try to connect to udp daytime server on port 9923
        which is written in q6.udpserver.c. After succesful connection it shows you "Hello message sent." and recevied time from server.

q6.udpserver.c : run "./q6.udpserver" on terminal and it will start service which provide daytime on port 9923 and after each coming request it will show "Request Received: __" and if
        request has been completed succesfully or time has been sent to client to succesfully it will show "Request completed".

q9.c : run "./q9" on terminal and it will start tcp(concurent) service which provide daytime on port 9923.
