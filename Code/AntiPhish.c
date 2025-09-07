// Ref: Project Phishing Scanner C How to Program by
// Paul and Harvey deitel P 508.

// After reading the project requirements in the comp
// sci text I captured a snippit of the project description
// and plugged it into google. Initial boiler plate code
// courtesy of google ai. 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // For tolower()

// Structure to hold keyword/phrase data
typedef struct {
    char *term;
    int points;
    int occurrences;
    long total_points;
} KeywordInfo;

// Function to convert a string to lowercase keeping
// things simple.

void to_lowercase(char *s) {
    for (int i = 0; s[i]; i++) {
        s[i] = tolower((unsigned char)s[i]);
    }
}

int main() {
  
    // Define keywords/phrases array of lists and their points.
    // I collected these words from a few of my own email spam box.
    // and some online examples and suggestions. I will add more
    // to make the program more accurate. Perhaps even keep a
    // keyword/phrase data file to be read in each time idk. 
  
    KeywordInfo keywords[] = {
        {"important", 10, 0, 0},
        {"critical", 15, 0, 0},
        {"urgent", 20, 0, 0},
        {"solution", 5, 0, 0},
        {"analysis", 7, 0, 0},
        {"username", 25, 0, 0},
	{"password", 25, 0, 0},
	{"interested", 8, 0, 0},
	{"school", 8, 0, 0},
	{"compensation",8, 0, 0},
	{"imf",10, 0, 0},
	{"fbi",15, 0, 0},
	{"transaction", 15, 0, 0},
	{"ai automation", 25, 0, 0},
	{"open to new ideas", 15, 0, 0},
	{"username password", 25, 0, 0},
	{"suspicious login detected action required", 25, 0, 0},
	{"reactivate account", 25, 0, 0},
	{"was this you please confirm identity", 25, 0, 0},
	{"login password reset", 25, 0, 0},
	{"notifications and updates", 25, 0, 0},
	{"compensating all scammed victims", 25, 0, 0},
	{"your full name", 25, 0, 0},
	{"your home address", 25, 0, 0},
	{"your phone number", 25, 0, 0},
	{"wire transfer", 25, 0, 0},
	{"hearby advised", 25, 0, 0},
	{"urgent cooperaton", 25, 0, 0},
	{"united states dollars", 25, 0, 0}// Example of a phrase
    };

    // Declare some variables, a FILE pointer *file, char array for filename.
    // A char array word[] to be a buffer to receive words from file to scan.
    // Declare a long var for overall_total_points.
    
    int num_keywords = sizeof(keywords) / sizeof(KeywordInfo);

    FILE *file;
    char filename[100];
    char word[256]; // Buffer for reading words
    long overall_total_points = 0;
    
    // Prompt the user to enter the filename to scan. read it in with scanf().
    
    printf("Enter the filename to scan: ");
    scanf("%s", filename);
    
    // open the file with fopen() in r read mode.
    
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read the file word by word (simplified approach for single words)
    // For phrases, more complex string matching is required (e.g., KMP algorithm or strstr)
    
    while (fscanf(file, "%255s", word) == 1) {
        to_lowercase(word); // Convert word to lowercase for case-insensitive matching

        // Remove punctuation from the end of the word
        int len = strlen(word);
        while (len > 0 && ispunct((unsigned char)word[len - 1])) {
            word[len - 1] = '\0';
            len--;
        }

        for (int i = 0; i < num_keywords; i++) {
            // Check for single word keywords
            if (strcmp(keywords[i].term, word) == 0) {
                keywords[i].occurrences++;
                keywords[i].total_points += keywords[i].points;
                overall_total_points += keywords[i].points;
            }
            // For phrases, a more robust check is needed, potentially reading line by line
            // and using strstr() or a custom phrase matching function.
            // This example above focuses on single words for simplicity.
        }
    }

    // Let's Reopen and read line by line for phrase matching (more robust)
    
    fseek(file, 0, SEEK_SET); // Reset file pointer to beginning
    char line_buffer[1024];//set aside an array to hold the line read in. 
    while (fgets(line_buffer, sizeof(line_buffer), file) != NULL) { // while loop using fgets until hitting 1024.
      // make the line all lower case. 
        to_lowercase(line_buffer);
        for (int i = 0; i < num_keywords; i++) {
            if (strstr(line_buffer, keywords[i].term) != NULL && strlen(keywords[i].term) > 1) { // Check for phrases
	      
                // Simple count for phrases, may overcount if phrase appears multiple times in one line
                // For exact phrase counting, more advanced logic is needed.
                // This counts at least one occurrence per line containing the phrase.
                keywords[i].occurrences++;
                keywords[i].total_points += keywords[i].points;
                overall_total_points += keywords[i].points;
	
            }
        }
    }

    fclose(file);

    printf("\n---Phishing Keyword/Phrase Analysis ---\n");
    for (int i = 0; i < num_keywords; i++) {
        if (keywords[i].occurrences > 0) {
            printf("%-15s: Occurrences: %d, Total Points: %ld\n",
                   keywords[i].term, keywords[i].occurrences, keywords[i].total_points);
	   
        }
    }

    printf("\nTotal points for the entire message: %ld\n", overall_total_points);

    return 0;
}
