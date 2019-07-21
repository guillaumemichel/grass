# GRASS (GRep As A Service)
Secure implementation of Grep as a Service by Alexandre Chambet, Guillaume Michel & Xavier Pantet.


## What is the aim of your project ?
The aim of the project was to develop a secure application running of the network. This project contains a client and a server that interract togheter. The client can upload and download files from the server, these files can be shared to other clients. When connected to the server, the user has first to login and then can use some basic commands such as `cd`, `ls`, `mkdir`, `rm` to navigate in the filesystem. The basic commands `ping`, `whoami`, `w`, `date` and of course `grep` are also included. To upload a file, the user can user the `put` command, and `get` to download a file.

## Which packages do I need to run your project ?
- `g++-multilib`, you can install it by typing
```bash
sudo apt-get install g++-multilib
```
in your command line.

## How do I compile your project?
- `make all`

## How do I configure your project?
Configuration is taken from `grass.conf`

## How do I run your project?
- Use `bin/server` to run the server
- Use `bin/client [server IP] [server port]` for the client
