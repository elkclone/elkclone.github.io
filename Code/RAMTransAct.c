// Transaction Processing program: Reads a Random-Access file
// sequentially, updates data already written to the file updateRecord()
// , creates new data to be placed in the file newRecord() and deletes data
// previously stored in the file deleteRecord().

// Ref: C how to program by Paul and Harvey Deitel p 494.

#include <stdio.h>

// clientData structure definition
struct clientData {
  unsigned int acctNum; // account number
  char lastName[15]; // account last name
  char firstName[10]; // account first name
  double balance; // account balance
};

// function prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);



int main(void)
{
  FILE *cfPtr; // accounts.dat file pointer

  // fopen opens the file; exits if file can't be opened.
  if ((cfPtr = fopen("accounts.dat", "rb+")) == NULL) {
    puts("File could not be opened. wtf!");
  }

  else {
    unsigned int choice; //user's inputed choice from main menue.

    // prompt user to specify menue choice. Each choice or case invokes
    // a function call to execute the function code block on the file pointer
    // cfPtr as an argument/parameter. 

    while ((choice = enterChoice()) != 5) {
      switch (choice) {
	// create text file from record file
      case 1:
	textFile(cfPtr);//calling function textFile on pointer to the file.
	break;
	// update record
      case 2:
	updateRecord(cfPtr);// function call to run updateRecord on cfPtr
	// Create newRecord
      case 3:
	newRecord(cfPtr);
	break;
	// delete existing record
      case 4:
	deleteRecord(cfPtr);
	break;
	// display message if user does not select valid choice
      default:
	puts("Incorrect choice");
	break;
	     }
      }
      fclose(cfPtr); // fclose closes the file
      
    }
  }

// now create File processing functions that were 'declared'
// as prototypes outside int main(void){program.c};  loop code block. 

// create formatted text file for stdio.h view/print screen printing.
void textFile(FILE *readPtr)
{
  FILE *writePtr; // accounts.txt file pointer
  
  // fopen opens the file; exits if file can't be opened
  if ((writePtr = fopen("accounts.txt", "w")) == NULL) {
    puts("File could not be opened. wtf");
  }
  else {
    rewind(readPtr); // sets pointer back to beginning of file
    fprintf(writePtr, "%-6s%-16s%-11s%10s\n",
	    "Acct", "Last Name", "First Name", "Balance");
    
    // copy all records from random-access file into text file
    while (!feof(readPtr)) {
      // create clientData with default information
      struct clientData client = { 0, "", "", 0.0 };
      int result =
	fread(&client, sizeof(struct clientData), 1, readPtr);
      
      //write single record to text file
      if (result != 0 && client.acctNum != 0) {
	fprintf(writePtr, "%-6d%-16s%-11s%10.2f\n",
		client.acctNum, client.lastName,
		client.firstName, client.balance);
      }
    }
    
    fclose(writePtr);  //fclose closes the file
  }
}

// Update balance in record
void updateRecord(FILE *fPtr)
{
  // Prompt user to enter the account number to update.
  printf("%s", "Enter account to update (1 - 100): ");
  unsigned int account; // variable declaration.
  scanf("%d", &account); // reads an integer value from standard input(keyboard)
  // and stores in in the variable name account. & operator = addressOf operator
  // and is required by the c built in function scanf.

  // Now we move file pointer to correct record in file

  fseek(fPtr, (account -1) * sizeof(struct clientData),
	SEEK_SET);

  // the * operator helps fseek function interact with the sizeof the clientData
  // structure as a stream to calculate and set the offset so we land on the
  // account number we want using fPtr.

  // Create clinetData with no information as a prototype
  struct clientData client = {0, "", "", 0.0};

  // read record from file at fPtr location.
  fread(&client, sizeof(struct clientData), 1, fPtr);

  // display error if account does not exits.
  if (client.acctNum == 0) {
    printf("Account #%d has no information. \n", account);
  }

  else { //update record
    printf("%-6d%-16s%-11s%10.2f\n\n",
      client.acctNum, client.lastName,
      client.firstName, client.balance);

    // request transaction amount from user keyboard input.
    printf("%s", "Enter charge (+) or payment (-): ");
    double transaction;  // variable declaration for transaction amount.
    scanf("%lf", &transaction);
	  client.balance += transaction;  // update record balance.

	  printf ("%-6d%-16s%-11s%10.2f\n",
		  client.acctNum, client.lastName,
		  client.firstName, client.balance);

	  // move file pointer to correct record in file
	  fseek(fPtr, (account - 1) * sizeof(struct clientData),
		SEEK_SET);

	  // write updated record over old record in file
	  fwrite(&client, sizeof(struct clientData), 1, fPtr);
  }
}


// Delete and existing record case.

void deleteRecord(FILE *fPtr)
{
  
  // Obtain number of account to delete from user keyboard input.

  printf("%s", "Enter account number to delete (1-100): ) ");
  unsigned int accountNum; // declaration of accountNum variable
  scanf("%d", &accountNum); // scanf takes the & 'addressOf' operator of accountNum

  // move file pointer to correct record in file
  fseek(fPtr, (accountNum -1) * sizeof(struct clientData),
	SEEK_SET);
  
  struct clientData client;  // stores record read form file. 

  // Read the record from file.
  fread(&client, sizeof(struct clientData), 1, fPtr);

  // Display error if record does not exist.
  if (client.acctNum == 0) {
    printf("Account %d does not exist.\n", accountNum);
  }
  else {  // Delete record.
    // Move file pointer to correct record in file
    fseek(fPtr, (accountNum -1) * sizeof(struct clientData),
	  SEEK_SET);

    struct clientData blankClient = {0, "", "", 0};  // blank client stucture.

    // Replace existing record with blank record
    fwrite(&blankClient,
	   sizeof(struct clientData), 1, fPtr);
  }
}

// Create and Insert a new record case.

void newRecord(FILE *fPtr)
{
  // Prompt user for number of account to create.

  printf("%s", "Enter new account number to create (1-100): ");
  unsigned int accountNum; // account number local var declaration.
  scanf("%d", &accountNum); // scanf gets the addressof accountNum.

  // move file pointer to correct record in file.
  // The * operator is for fseek to * stream interact with sizeof the structure
  // and find the offset to place the account number in correct order. 
  fseek(fPtr, (accountNum -1) * sizeof(struct clientData),
	SEEK_SET);

  // Create clientData structure blank with default information. a prototype.
  
  struct clientData client = { 0, "", "", 0.0 };

  // Read the record from file
  fread(&client, sizeof(struct clientData), 1, fPtr);

  // Display error if account already exists.

  if (client.acctNum != 0) {
    printf("Account #%d aready contains information.\n",
	   client.acctNum);
  }
  else
    {  //create new record :D .
      // prompt user to enter lastname, first name and balance (-/+).
      
      printf("%s", "Enter lastname, firstname, balance \n ");
      scanf("%14s%9s%lf", client.lastName, client.firstName, &client.balance);
      
      client.acctNum = accountNum;  // re assign to a local variable

      // Move file pointer to correct record place in file
      fseek(fPtr, (client.acctNum -1) *
	    sizeof(struct clientData), SEEK_SET);

      // Insert record into the file
      fwrite(&client,
	     sizeof(struct clientData), 1, fPtr);
    }
}

// Enable User to input menu choice from the switch case menu in main loop.

unsigned int enterChoice(void)
{
  // Display a prompt list of available options.
  printf("%s", "\nEnter your choice\n"
	 "1 - Save to formatted text file of accounts called accounts.txt\n"
	 "2 - Update an account\n"
	 "3 - Add a new account\n"
	 "4 - delete an account\n"
	 "5 - End Program/ Quit\n");
  unsigned int menuChoice;  // variable declared to store user's choice.
  scanf("%u", &menuChoice); // recieve the choice from user store in addressof
  // menuChoice.
  return menuChoice;
}

     
  

 
	
