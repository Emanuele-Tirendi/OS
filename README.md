# Multithreaded Webserver

This is a webserver that stores a `html` page, supports several concurrent client connections and allows the clients to access and modify that `html` page.

## Building and Running

### General

#### System requirements:
* This software was developed and tested on **Ubuntu 22.04.4 LTS** on **Oracle VM VirtualBox Manager**.
* For installing the software you need **git**.
* For building and running the software you need **gcc** where version **gcc \(Ubuntu 11.4.0-1ubuntu1~22.04\) 11.4.0** and **python3** where version **Python 3.10.12** was used for development and testing.
* If you connect clients and servers on different devices, use Hamachi for having a network.
* To run the Unit tests make sure you have CUnit version 2.1-3 installed.
#### How to get the code
* Open a terminal and type
`git clone https://github.com/Emanuele-Tirendi/OS.git`

On one device it is possible to run one server and two clients. Since the server supports multiple concurrent client-connections, up to 100 clients can connect to the server at the same time.

### Server

For running the server, you need to navigate into the most top level folder of the project and call `python3 server.py [-c] [-r] [-l]`.
* Flag `-c` compiles the server.
* Flag `-r` runs the server.
* Flag `-l` deletes all log files that have been created by the clients and the server.

### Client

For running the client, you need to adjust the ipadrress in client.py to the ipaddress of the device that runs the server and you need to navigate into the most top level folder of the project and call `python3 client.py [-c1] [-r1]` or `python3 client.py [-c2] [-r2]`.
* Flags `-c1` and `c2` compile the client.
* Flags `-r1` and `-r2` run the clients.
* You can only compile and/or run one client (i.e. use a combination of `[-c1] [-r1]`) or the other client (i.e. use a combination o f`[-c2] [-r2]`) but not use both in the same call to `python3 client.py`.

## Interacting with the Client

Through the command line you can interact with the client. The following commands exist:

|Syntax|Semantics|
|---|---|
|`echo`|Sends an `echo` to the server which prints it on standard output and sends one back to the client which prints it on standard output as well.|
|`quit`|Closes the client.|
|`get_html`|Sends a request to the server to get the `html` file the server stores.|
|`open_html`|Opens the `html` file that the client stores. If `open_html` is called without calling `get_html` first, an error message is printed to standard output.|
|`pid`|Returns the process id of the client.|
|`insert$<number>$<content>`|It is recommended to call `get_html` before calling this command- Inserts in the `html` file stored on the server, but not in the `html` file stored on the client, one line. For having this change on the client, you need to call `get_html` again. In inserts a line containing `<content>` right above the `<number>`-th line. Here with `<number>`-th line it is meant the `<number>`-th line in the `html` file stored on the server, not stored on the client. <ul><li>An error is printed to standard output and nothing else happens when `<number>` is smaller than or equal to 0.</li><li>Am error is printed to standard output and nothing else happens when `<content>` contains a `$` character.</li><li>A message is printed to standard output and nothing else happens when `<number>` is bigger than the biggest line index on the `html` file.</li></ul>|
|`delete$<number>`|It is recommended to call `get_html` before calling this command. Deletes in the `html` file stored on the server, but not in the `html` file stored on the client, the `<number>`-th line. For having this change on the client, you need to call `get_html` again. Here with `<number>`-th line it is meant the `<number>`-th line in the `html` file stored on the server, not stored on the client. <ul><li>An error is printed to standard output and nothing else happens when `<number>` is smaller than or equal to 0.</li><li>A message is printed to standard output and nothing else happens when `<number>` is bigger than the biggest line index on the `html` file.</li></ul>|
|`change$<number>$<content>`|It is recommended to call `get_html` before calling this command. Changes in the `html` file stored on the server, but not in the `html` file stored on the client, one line. For having this change on the client, you need to call `get_html` again. In changes the `<number>`-th line's content to .`<content>`. Here with `<number>`-th line it is meant the `<number>`-th line in the `html` file stored on the server, not stored on the client. <ul><li>An error is printed to standard output and nothing else happens when `<number>` is smaller than or equal to 0.</li><li>Am error is printed to standard output and nothing else happens when `<content>` contains a `$` character.</li><li>A message is printed to standard output and nothing else happens when `<number>` is bigger than the biggest line index on the `html` file.</li></ul>|
|`id$insert$<number>$<content>`|It is recommended to call `get_html` before calling this command. Inserts in the `html` file stored on the server, but not in the `html` file stored on the client, one line. For having this change on the client, you need to call `get_html` again. In inserts a line containing `<content>` right above the line on the server that corresponds to the `<number>`-th line on the client but that might have another line index because of inconsistencies due to concurrent access of the `html` file on the server. <ul><li>An error is printed to standard output and nothing else happens when `<number>` is smaller than or equal to 0.</li><li>Am error is printed to standard output and nothing else happens when `<content>` contains a `$` character.</li><li>A message is printed to standard output and nothing else happens if the corresponding line on the server doesn't exist anymore.</li></ul>|
|`id$delete$<number>`|It is recommended to call `get_html` before calling this command. Deletes in the `html` file stored on the server, but not in the `html` file stored on the client, a  line. For having this change on the client, you need to call `get_html` again. Here with `<number>`-th line it is meant the line on on the server that corresponds to the `<number>`-th line on the client but that might have another line index because of inconsistencies due to concurrent access of the `html` file on the server. . <ul><li>An error is printed to standard output and nothing else happens when `<number>` is smaller than or equal to 0.</li><li>A message is printed to standard output and nothing else happens if the corresponding line on the server doesn't exist anymore.</li></ul>|
|`id$change$<number>$<content>`|It is recommended to call `get_html` before calling this command. Changes in the `html` file stored on the server, but not in the `html` file stored on the client, one line. For having this change on the client, you need to call `get_html` again. In changes to `<content>` the content of the line on the server that corresponds to the `<number>`-th line on the client but that might have another line index because of inconsistencies due to concurrent access of the `html` file on the server. <ul><li>An error is printed to standard output and nothing else happens when `<number>` is smaller than or equal to 0. </li><li>Am error is printed to standard output and nothing else happens when `<content>` contains a`$` character.</li><li>A message is printed to standard output and nothing else happens if the corresponding line on the server doesn't exist anymore.</li></ul>|
|`check_id`|Prints on the client and on the server some internally held information which are used for the identification of lines which is important for the calls `id$insert`, `id$delete` and `id$change`.|

As of now, only ... lines in the `html` file are supported and a file size of at most 9'500 characters. We could supoort much larger files, but since we plan to print the ids of the lines with `check_id` and we don't want to print too many lines during the presentation, we limit the size of the `html` file.

## Logging

The server and the client each of them has two own logging files while different clients have separate pairs of logging files. The server and each client has one file with Ping-Pong-Information logging and one file with general purpose logging.

Each client's logging files will be named with the process ids of the clients such that if several clients run on the same device or if old logging files haven't been deleted, it is clearly recognizable which client the logging files belong to.

Logging was rather used for development and debugging for developers, but can also provide useful information to the user. But don't expect too useful and too well formatted logging information.

Following events are logged:
* client and server logg: own start
* client and server logg:  Ping or Pong received
* server loggs: when the connection to a specific client is lost due to Ping-Pong inactivity
* client loggs: from user requested quit
* server loggs: when a client quits or the connection is lost
* client loggs: when the connection to a server is lost

Note here that losing the connection and losing the connection due to Ping-Pong inactivity aren't the same thing. Losing the connection due to Ping-Pong inactivity can happen even if there is a good connection, but the Ping-Pong time expires. For that, see more under section Ping-Pong.

## Ping-Pong and Client-Server-Communication

Client and server have two parallel communications. One is the communication cascade triggered by a user input in the client and the other one is the Ping-Pong-communication. In the Ping-Pong-communication, client and server exchange Ping- and Pong-messages and each time they get one such message, a timer is reset to zero and counts the time until the next Ping- or Pong-message arrives. If the time expires before the next Ping- or Pong-message arrives, it will have as consequence that the program detecting this will cut off the connection to the other program.

## Unit tests with CUnit

We have written several unit test that check the main functions of our server features which are insert, delete and change.
To run them you need to first make a few small adjustments in the code.
* First you need to change in server/html.c line 186 so it says `append(HTML_NAME, ORIGINAL_NALE, 1, -1);` instead of `append(HTML_NAME, CHESS_NAME, 1, -1);`
* Further you need to change line 63 in server/id.c so it says `for (int i = 1; i <= 12; i++) {` instead of `for (int i = 1; i <= 38; i++) {`
* You also need to change line 66 server/id.c so it says `for (int i = 13; i < FILE_LENGTH; i++) {` instead of `for (int i = 39; i < FILE_LENGTH; i++) {`
To compile the tests you need to be in the OS folder and not in any subfolder and run following command: `gcc server/html.c server/html.h server/id.c server/id.h server/IO.c server/IO.h server/test.h server/tests.c shared/constants.c shared/constants.h shared/html.c shared/html.h shared/log.c shared/log.h shared/time.c shared/time.h -o server/test -lcunit`
And to run the tests you need to navigate into the server folder and run the shell command `./test`.
