#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Represents a playing card
typedef struct {
    int value; // 1-11 for Ace, 2-10 for numbers, 10 for face cards
} Card;

// Represents a node in the stack
typedef struct Node {
    Card data;
    struct Node* next;
} Node;

// Represents the stack
typedef struct {
    Node* top;
} Stack;

// Function prototypes
Stack* createStack();
void push(Stack* stack, Card card);
Card pop(Stack* stack);
bool isEmpty(Stack* stack);
void freeStack(Stack* stack);
void initializeDeck(Stack* deck);
void shuffleDeck(Stack* deck);
void dealCard(Stack* source, Stack* destination);
int calculateHandValue(Stack* hand);
void printHand(Stack* hand, bool isDealer, bool hideFirst);
void playRound(Stack* deck);
void clearHands(Stack* playerHand, Stack* dealerHand);

int main() {
    srand(time(NULL));
    Stack* deck = createStack();

    while (true) {
        initializeDeck(deck);
        shuffleDeck(deck);
        playRound(deck);

        printf("\nPlay again? (y/n): ");
        char choice;
        scanf(" %c", &choice);
        if (choice != 'y' && choice != 'Y') {
            break;
        }
        printf("\n");
    }

    freeStack(deck);
    return 0;
}

// Creates an empty stack
Stack* createStack() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    stack->top = NULL;
    return stack;
}

// Pushes a card onto the stack
void push(Stack* stack, Card card) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = card;
    newNode->next = stack->top;
    stack->top = newNode;
}

// Pops a card from the stack
Card pop(Stack* stack) {
    if (isEmpty(stack)) {
        // This should not happen in a well-managed game
        printf("Stack underflow!\n");
        exit(EXIT_FAILURE);
    }
    Node* temp = stack->top;
    Card poppedCard = temp->data;
    stack->top = stack->top->next;
    free(temp);
    return poppedCard;
}

// Checks if the stack is empty
bool isEmpty(Stack* stack) {
    return stack->top == NULL;
}

// Frees all memory used by the stack
void freeStack(Stack* stack) {
    while (!isEmpty(stack)) {
        pop(stack);
    }
    free(stack);
}

// Populates the deck with 52 cards
void initializeDeck(Stack* deck) {
    const int NUM_DECKS = 1;
    const int NUM_SUITS = 4;
    const int NUM_CARDS_PER_SUIT = 13;
    Card card;

    // Clear any previous deck
    while (!isEmpty(deck)) {
        pop(deck);
    }

    // Populate the deck with multiple suits
    for (int d = 0; d < NUM_DECKS; d++) {
        for (int i = 0; i < NUM_SUITS; i++) {
            for (int j = 1; j <= NUM_CARDS_PER_SUIT; j++) {
                if (j >= 10) {
                    card.value = 10;
                } else if (j == 1) {
                    card.value = 11; // Ace starts as 11
                } else {
                    card.value = j;
                }
                push(deck, card);
            }
        }
    }
}

// Shuffles the deck by simulating a physical shuffle
void shuffleDeck(Stack* deck) {
    int numCards = 0;
    Node* current = deck->top;
    while (current != NULL) {
        numCards++;
        current = current->next;
    }

    if (numCards < 2) return;

    Card* tempArray = (Card*)malloc(numCards * sizeof(Card));
    if (tempArray == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numCards; i++) {
        tempArray[i] = pop(deck);
    }

    // Fisher-Yates shuffle algorithm
    for (int i = numCards - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card temp = tempArray[i];
        tempArray[i] = tempArray[j];
        tempArray[j] = temp;
    }

    for (int i = 0; i < numCards; i++) {
        push(deck, tempArray[i]);
    }
    free(tempArray);
}

// Deals a card from one stack to another
void dealCard(Stack* source, Stack* destination) {
    if (!isEmpty(source)) {
        Card card = pop(source);
        push(destination, card);
    }
}

// Calculates the value of a hand, adjusting for Aces
int calculateHandValue(Stack* hand) {
    int value = 0;
    int aceCount = 0;
    Node* current = hand->top;
    while (current != NULL) {
        value += current->data.value;
        if (current->data.value == 11) {
            aceCount++;
        }
        current = current->next;
    }

    // Adjust for Aces if the total is over 21
    while (value > 21 && aceCount > 0) {
        value -= 10;
        aceCount--;
    }
    return value;
}

// Prints the cards in a hand
void printHand(Stack* hand, bool isDealer, bool hideFirst) {
    Node* current = hand->top;
    if (isDealer && hideFirst && current != NULL) {
        printf("[Hidden] ");
        current = current->next;
    }

    while (current != NULL) {
        if (current->data.value == 11) {
            printf("[Ace] ");
        } else if (current->data.value == 10) {
            // Can be 10, J, Q, K
            printf("[%d] ", 10);
        } else {
            printf("[%d] ", current->data.value);
        }
        current = current->next;
    }
    printf("\n");
}

// Handles the logic for a single round of Blackjack
void playRound(Stack* deck) {
    Stack* playerHand = createStack();
    Stack* dealerHand = createStack();
    char choice;
    bool playerTurn = true;
    bool playerBust = false;
    bool dealerBust = false;
    int playerScore, dealerScore;

    // Initial deal
    dealCard(deck, playerHand);
    dealCard(deck, dealerHand);
    dealCard(deck, playerHand);
    dealCard(deck, dealerHand);

    // Player's turn
    while (playerTurn) {
        printf("Your hand: ");
        printHand(playerHand, false, false);
        playerScore = calculateHandValue(playerHand);
        printf("Your score: %d\n", playerScore);
        printf("Dealer's hand: ");
        printHand(dealerHand, true, true);

        if (playerScore > 21) {
            printf("You busted! Your score is over 21.\n");
            playerBust = true;
            playerTurn = false;
        } else {
            printf("Hit or Stand? (h/s): ");
            scanf(" %c", &choice);
            if (choice == 'h' || choice == 'H') {
                dealCard(deck, playerHand);
            } else {
                playerTurn = false;
            }
        }
    }

    // Dealer's turn
    if (!playerBust) {
        printf("\nDealer's turn...\n");
        printf("Dealer's hand: ");
        printHand(dealerHand, false, false);
        dealerScore = calculateHandValue(dealerHand);

        while (dealerScore < 17) {
            printf("Dealer hits.\n");
            dealCard(deck, dealerHand);
            printf("Dealer's hand: ");
            printHand(dealerHand, false, false);
            dealerScore = calculateHandValue(dealerHand);
        }
        if (dealerScore > 21) {
            printf("Dealer busted! Dealer's score is over 21.\n");
            dealerBust = true;
        }
    }

    // Determine winner
    printf("\n--- Final Results ---\n");
    printf("Your score: %d\n", calculateHandValue(playerHand));
    printf("Dealer's score: %d\n", calculateHandValue(dealerHand));

    if (playerBust) {
        printf("You lose.\n");
    } else if (dealerBust) {
        printf("You win!\n");
    } else if (playerScore > dealerScore) {
        printf("You win!\n");
    } else if (dealerScore > playerScore) {
        printf("You lose.\n");
    } else {
        printf("It's a tie.\n");
    }

    // Clean up hands
    clearHands(playerHand, dealerHand);
}

// Clears the hands after a round
void clearHands(Stack* playerHand, Stack* dealerHand) {
    freeStack(playerHand);
    freeStack(dealerHand);
}
