# Coa
Coa is a text transfer application that runs on terminal. It works by creating a host on the target machine which listens for incoming messages from clients. TCP sockets are used for the connection and the port number is predefined as 8080. After every transfer, Coa will compare checksums between the host and the client to make sure the integrity of the text.

To make the program efficient, a size of the file had been limited to 65536 bytes.

## Usage
### Server
Start a server to listen incoming messages on the target machine:

```
coa -s
```

Additionally, use `--no-save` option if you do not want the message to be saved into a file.

### Client

Sending a message as a command line argument:
```
coa -c [hostname] -m [message]
```

Sending a message as a text file:
```
coa -c [hostname] -f [filename.txt]
```

## Building
If you have `clang`, you can just use the [makefile](Makefile). Otherwise, build all three `.c` files and link them into an executable.

## Author
- Boran Seckin

## Reference
The [tutorial](https://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html) I used for the socket setup in C.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
