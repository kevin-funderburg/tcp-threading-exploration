# Pthreads Exploration 

This project is an exploration of threads within networking.

## Configuration

To compile, navigate to `unpv13e/hw2'

### Make commands

```shell
make        # compile and create executables
make clean  # reset executables and object files
```

## Execution

### Problem 1

- Begin server with `./1-nagleserv`
- Begin client with `./1-naglecli`

### Problem 2

Problem 2 contains 5 parts, they can be tested as follows

```shell
./2-pthreads 1  #test part 1
./2-pthreads 2  #test part 2
./2-pthreads 3  #test part 3
./2-pthreads 4  #test part 4
./2-pthreads 5  #test part 5
```

### Problem 3

- Begin server with `./3-tcpserv01 0.0.0.0 9999`
- Begin client with `./3-tcpcli01 0.0.0.0 9999`

## Notes

View the project summary for more detailed explanations of each part.
 
## Licensing & thanks

This project is released under the [MIT License](./LICENSE.txt).

## Changelog

- v1.0.0
