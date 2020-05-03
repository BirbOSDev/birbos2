#include "../include/shell.h"
void launch_shell(int n)
{
	string ch = (string) malloc(200); // util.h
	int counter = 0;
	do
	{
                    print(">>>");
		    ch = readStr(); //memory_copy(readStr(), ch,100);
		    if(strEql(ch,"clear"))
		    {
		            clearScreen();
		    }
		    else if(strEql(ch,"echo"))
		    {
		    	echo();
		    }
		    else if(strEql(ch,"ls"))
		    {
			print("\nbruh there is no file system\n");
		    }
		    else if(strEql(ch,"about"))
		    {
			  print("\n");
			  print("oooooo____oo__________oo____________oooo_____ooooo__\n");
		          print("oo____oo______oo_ooo__oooooo______oo____oo__oo___oo_\n");
		          print("oooooooo__oo__ooo___o_oo___oo____oo______oo__oo_____\n");
		          print("oo____oo__oo__oo______oo___oo____oo______oo____oo___\n");
        		  print("oo____oo__oo__oo______oo___oo_____oo____oo__oo___oo_\n");
        		  print("ooooooo__oooo_oo______oooooo________oooo_____ooooo__\n");
        		  print("____________________________________________________\n");
		          print("\nBirb OS 1.0.0\n");
		    }
		    else if(strEql(ch,"help"))
		    {
		    	help();
		    }
		    else if(strEql(ch,""))
		    {
			print("\n");
		    }
		    else
		    {
		            print("\nBad command!\n");
		            print("");
		    } 
	} while (!strEql(ch,"exit"));
}




void echo()
{
	print("\n");
	string str = readStr();
	print("\n");
	print(str);
	print("\n");
}
void print_array(int arr[],int n)
{
	int i = 0;
	for (i = 0;i<n;i++)
	{
		/*print("ARR[");
		print(int_to_string(i));
		print("]: ");*/
		print(int_to_string(arr[i]));
		print("   ");
	}
	print("\n");
}

int sum_array(int arr[],int n)
{
	int i = 0;
	int s = 0;
	for (i = 0;i<n;i++)
	{
		s += arr[i];
	}
	return s;
}



void print_matrix(int matrix[][100],int rows,int cols)
{
	int i =0;
	int j = 0;
	for (i = 0;i<rows;i++)
	{
		for(j =0;j<cols;j++)
		{
			print(int_to_string(matrix[i][j]));
			print("   ");
		}
		print("\n");
	}
}




void help()
{
	print("\nclear     : Clears the screen");
	print("\necho      : Reprint a given text");
	print("\nabout     : Prints out the version number of os");
	print("\n\n");
}
