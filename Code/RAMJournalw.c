// Writing data randomly to a random-access file with fwrite,
// fseek, pointers and structures. 
// ref C how to code by Paul and Harvey Dietel p 489

#include <stdio.h>

// clientData sturcture definition
struct clientData {
  unsigned int acctNum; // account number
  char lastName[15];  // account last name
  char firstName[10]; // account first name
  double balance;   //account balance
}; //end structure clientData

int main(void)
{
  FILE *cfPtr; // accounts.dat file pointer

  // fopen opens the file; exits if file can't be opened
  if ((cfPtr = fopen("accounts.dat", "rb+")) == NULL) {
    puts("File could not be opened. wtf!");
  }
  else {
    //create new clientData struct named client
    //with default information
    struct clientData client = {0, "", "", 0.0}; //object

    // Require User to specify an account number first
    printf("%s", "Enter account number"
	  " (1 to 100, 0 to end input): ");
    scanf("%d", &client.acctNum);// dot operator to access
    // individual members of a structure. 

    // User enters information that is copied to file.

    while (client.acctNum != 0) {
      
      // prompt so user enters last name, first name, balance
      
      printf("%s", "\nEnter lastname, firstname, balance: ");
      
      // set record lastName, firstName and balance value
      
      fscanf(stdin, "%14s%9s%lf", client.lastName,
	     client.firstName, &client.balance);
      
      // seek position in file to user-specified record by
      // account number

      fseek(cfPtr, (client.acctNum - 1) *
	    sizeof(struct clientData), SEEK_SET);

      // write use-specified information by account number
      // to a file.

      fwrite(&client, sizeof(struct clientData), 1, cfPtr);

      // prompt user to enable input of another account number
      printf("%s", "\nEnter account number: ");
      scanf("%d", &client.acctNum);
    }
    fclose(cfPtr); // fclose closes the file
  }
}

							     

      
  
