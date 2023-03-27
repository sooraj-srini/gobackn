# Introduction

This is an implementation of the Go Back N protocol using a sender and a receiver sending messages through UDP packets.

# Creating the executable

Run `make all` in the root directory to generate two executables: `SenderGBN` and `ReceiverGBN`. 
Run `make clean` to remove  these executables.

# Running the executables

There are multiple command line arguments that can be passed to the executables:
- DEBUG or NODEBUG mode