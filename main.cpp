#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "prime.h"

int size = 0;
int count = 0;

void do_action_child_process(int read_end_first_pipe, int write_end_second_pipe){
	int start, end, prime[200], pos = 0, _size = 0;
	read(read_end_first_pipe, &start, sizeof(start));
	read(read_end_first_pipe, &end, sizeof(end));
	for(int i = start; i< end; i++){
		if(is_prime(i)){
			prime[pos] = i;
			pos ++;
			_size ++;
		}	
	}
	size = _size;
	write(write_end_second_pipe, &_size, sizeof(size));
	write(write_end_second_pipe, prime, sizeof(prime));
	exit(1);
}

void do_action_parent_process(int read_end_second_pipe, int write_end_first_pipe){
	int start, end, prime[200];
	start = (count - 1) * 1000;
	end =  count * 1000;
	write(write_end_first_pipe, &start, sizeof(int));
	write(write_end_first_pipe, &end, sizeof(int));
	read(read_end_second_pipe, &size, sizeof(size));
	read(read_end_second_pipe, &prime, sizeof(prime));
	for(int  i = 0; i < size; i++){
		 std::cout<<prime[i]<< " ";	
	}
	std::cout<<"\n\n";
}

int main (){
	int pipe1[2], pipe2[2], pid;
	if(pipe(pipe1)== -1){
		std::cout<<"Pipe not created"<<std::endl;
		exit(-1);	
	}
	if(pipe(pipe2) == -1){
		std::cout<<"Pipe not created"<<std::endl;
		exit(-1);
	}
	for(int index = 0 ; index < 10 ; index++ ){
		pid = fork();
		count +=1;
		if(pid){
			if(pid == -1){
				std::cout<<"Process not created \n";
				exit(-1);
			}
			do_action_parent_process(pipe2[0], pipe1[1]);

		}else{
			do_action_child_process(pipe1[0], pipe2[1]);
		}
	}
}
