//Reading and Printing a sequential file
//ref c How to Program by Deitel and Deitel p 481.

#include <stdio.h>

int main(void)
{
  FILE *cfPtr; // cfPtr - clients.txt file pointer

  // fopen opens file; exits program if file cannot be opened.

  if ((cfPtr = fopen("clients.txt", "r")) == NULL) {
      puts("File could not be opened. bad things tm happened!");

    }
    else { //Read account num, name and balance owing from seq file.
      unsigned int account; //account number
      char name[30]; // account name
      double balance; //account balance

      printf("%-10s%-13s%s\n", "Account", "Name", "Balance");
      fscanf(cfPtr, "%d%29s%lf", &account, name, &balance);
      //while not end of file ctrl-d
      while (!feof(cfPtr) ) {
	printf("%-10d%-13s%7.2f\n", account, name, balance);
	fscanf(cfPtr, "%d%29s%lf", &account, name, &balance);
      }
      fclose(cfPtr); //fclose closes the file
    }
    
}    
      
// A few runs of this and the ARseq program with attempts to add on to and even
// update the data by entering the same account number in an attempt to overwrite
// won't work. This is why random access files are created to have same record
// field sizes so they can update without overflowing and destroying the next
// data field. 
