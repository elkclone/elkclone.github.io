// Credit inquiry program
// ref: C How to Program by Paul and Harvey Dietel
// p 483.

// function main begins progam execution
#include <stdio.h>

int main(void)
{
  FILE *cfPtr; // clients.txt file pointer
  // fopen opens the file; exits program if can't open
  if ((cfPtr = fopen("clients.txt", "r")) == NULL) {
    puts("File could not be opened. omg!");
  }
  else {
    // Display menue request options

    printf("%s", "Enter request \n"
	   "1 - List accounts with zero balances \n"
	   "2 - List accounts with -credit balances\n"
	   "3 - List accounts with +debit balances\n"
	   "4 - End Program and exit?\n");
    
    // declare variable request
    // to store the users input choice

    unsigned int request; 
    
    scanf("%u", &request);

    // process user's request
    while (request != 4) {
      unsigned int account; //account number
      double balance; // account balance
      char name[30]; // account name

      // read account, name and balance from file
      fscanf(cfPtr, "%d%29s%lf", &account, name, &balance);

      switch (request) {
         case 1:
	   puts("\nAccounts with zero balances:");

	   // read file contents (until eof)

	   while (!feof(cfPtr)) {
	     // output if balance = 0
	     if (balance == 0) {
	       printf("%-10d%-13s%7.2f\n",
		      account, name, balance);
	     }
	     // read account, name and balance from file
	     fscanf(cfPtr, "%d%29s%lf",
		    &account, name, &balance);
	   }

	   break;

      case 2:
	puts("\nAccounts with credit balances:\n");

	// read file contents (until eof)
	while (!feof(cfPtr)) {
	  // output only if balance is credit less than zero
	  if (balance <0) {
	    printf("%-10d%-13s%7.2f\n",
		   account, name, balance);
	  }

	  // read account, name and balance from file
	  fscanf(cfPtr, "%d%29s%lf",
		 &account, name, &balance);
	}
	
	break;
      case 3:
	puts("\nAccounts with debit + positive balances:\n");
	//read file contents (until eof)
	while (!feof(cfPtr)) {
	  // output only if balance > 0
	  if (balance > 0) {
	    printf("%-10d%-13s%7.2f\n",
		   account, name, balance);
	  }

	  // read account, name and balance from file
	  fscanf(cfPtr, "%d%29s%lf",
		 &account, name, &balance);
	}
	break;
      }
      rewind(cfPtr); // return cfPtr to beginning of seq file
      printf("%s", "\n? ");
      scanf("%d", &request);
      
    }
    puts("End of Program run.");
    fclose(cfPtr); // fclose closes the file
  }
}


	
	   
