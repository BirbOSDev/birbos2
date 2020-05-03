#include "../include/shell.h"
void launch_shell(int n)
{
	string ch = (string) malloc(200); // util.h
	int counter = 0;
	do{
                    print(">>>");
		    ch = readStr(); //memory_copy(readStr(), ch,100);
		    if(strEql(ch,"clear")){
		            clearScreen();
		    }
		    else if(strEql(ch,"echo")){
		    	echo();
		    }
		    else if(strEql(ch,"ls")){
			print("\nbruh there is no file system\n");
		    }
		    else if(strEql(ch,"about")){
			  print("\n");
			  print("oooooo____oo__________oo__________oooo_____ooooo__\n");
		          print("oo____oo______oo_ooo__oooooo____oo____oo__oo___oo_\n");
		          print("oooooooo__oo__ooo___o_oo___oo__oo______oo__oo_____\n");
		          print("oo____oo__oo__oo______oo___oo__oo______oo____oo___\n");
        		  print("oo____oo__oo__oo______oo___oo___oo____oo__oo___oo_\n");
        		  print("ooooooo__oooo_oo______oooooo______oooo_____ooooo__\n");
        		  print("____________________________________________________\n");
		          print("\nBirb OS 1.0.0\n");
		    }
		    else if(strEql(ch,"help")){
		    	help();
		    }
		    else if(strEql(ch,"calc")){
			calc();
		    }
		    else if(strEql(ch,"")){
			print("\n");
		    }
		    else{
		            print("\nBad command!\n");
		            print("");
		    } 
	} while (!strEql(ch,"exit"));
}




void echo(){
	print("\n");
	string str = readStr();
	print("\n");
	print(str);
	print("\n");
}
void print_array(int arr[],int n){
	int i = 0;
	for (i = 0;i<n;i++){
		/*print("ARR[");
		print(int_to_string(i));
		print("]: ");*/
		print(int_to_string(arr[i]));
		print("   ");
	}
	print("\n");
}

int sum_array(int arr[],int n){
	int i = 0;
	int s = 0;
	for (i = 0;i<n;i++){
		s += arr[i];
	}
	return s;
}



void print_matrix(int matrix[][100],int rows,int cols){
	int i =0;
	int j = 0;
	for (i = 0;i<rows;i++){
		for(j =0;j<cols;j++){
			print(int_to_string(matrix[i][j]));
			print("   ");
		}
		print("\n");
	}
}




void help(){
	print("\nclear     : Clears the screen");
	print("\necho      : Reprint a given text");
	print("\nabout     : Prints outs detail of the os");
	print("\ncalc      : Calculator");
	print("\n\n");
}

void add(){
	print("\nNumber 1: ");
	int x = str_to_int(readStr());
	print("\nNumber 2: ");
	int y = str_to_int(readStr());
	print("\nResult: ");
	print(int_to_string(x+y));
	print("\n");
}
void multiply(){
        print("\nNumber 1: ");
        int x = str_to_int(readStr());
        print("\nNumber 2: ");
        int y = str_to_int(readStr());
        print("\nResult: ");
        print(int_to_string(x*y));
        print("\n");
}
void sub(){
        print("\nNumber 1: ");
        int x = str_to_int(readStr());
        print("\nNumber 2: ");
        int y = str_to_int(readStr());
        print("\nResult: ");
        print(int_to_string(x-y));
        print("\n");
}
void div(){
        print("\nNumber 1: ");
        int x = str_to_int(readStr());
        print("\nNumber 2: ");
        int y = str_to_int(readStr());
        print("\nResult: ");
	int z = x/y;
        print(int_to_string(z));
        print("\n");
}

void calc(){
	print("\nEnter 1 to add, 2 to subtract, 3 to multiply and 4 to divide\n");
	string opcode = readStr();
	if(strEql(opcode,"1")){
		add();
	}
	else if(strEql(opcode,"2")){
		sub();
	}
	else if(strEql(opcode,"3")){
		multiply();
	}
	else if(strEql(opcode,"4")){
		div();
	}
	else{
		print("\nenter valid operation rerun the program\n");
	}
}
