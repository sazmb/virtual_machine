vm: o_files/vm.o o_files/main.o
	cc -g -O0 -o vm o_files/vm.o o_files/main.o
o_files/vm.o: vm.c vm.h
	cc -g -O0 -c vm.c -o o_files/vm.o
o_files/main.o: main.c vm.h
	cc -g -O0 -c main.c -o o_files/main.o









