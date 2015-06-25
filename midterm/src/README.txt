Supported functions by myls command.

1. ./myls   		-- for default
2. ./myls -l  		-- for long listing
3. ./myls -s		-- listing with size
4. ./myls -R		-- For recursive
5. ./myls -t		-- sort based on time
6. ./myls -S		-- Sort in DESC order
etc..

For more help you can use help command.

./myls -h

- Run make command for generate binary files
By default 'make' or 'make all' command generate 2 binaries 
One name 'myls' link with .so library.
Second name 'myls_static' link with .a library.
Both binaries sotre in ../bin folder and both the library store in ../lib folder.

commands:
make  -- for generate 'myls' and 'myls_static' both kind of binary.
make shared -- For generate specific binary 'myls' by using .so (shared) library.
make static -- For generate specific binary 'myls_static' by using .a (static) library.

NOTE: NEED TO SET THE LD_LIBRARY_PATH FIRST BEFORE USE THE 'myls' BINARY BECAUSE IT SEARCH FOR 'libmyls.so' LIBRARY.



- For time saving purpose use 'allcmd.sh' script file under ../bin folder and redirect output in a file.

 
