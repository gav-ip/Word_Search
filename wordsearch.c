#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
void printPuzzle(char **arr);
void searchPuzzle(char **arr, char *word);
char toLower(char ch);
int searchFrom(char **arr, int *path, int row, int col, char *word, int index);

// global puzzle size
int bSize;

// using directions for dfs search represented in an array
int directions[] = {
    -1, -1, // Up-Left
    -1, 0,  // Up
    -1, 1,  // Up-Right
    0, -1,  // Left
    0, 1,   // Right
    1, -1,  // Down-Left
    1, 0,   // Down
    1, 1    // Down-Right
};
// Main function, DO NOT MODIFY
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
        return 2;
    }
    int i, j;
    FILE *fptr;

    // Open file for reading puzzle
    fptr = fopen(argv[1], "r");
    if (fptr == NULL)
    {
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }

    // Read the size of the puzzle block
    fscanf(fptr, "%d\n", &bSize);

    // Allocate space for the puzzle block and the word to be searched
    char **block = (char **)malloc(bSize * sizeof(char *));
    char *word = (char *)malloc(20 * sizeof(char));

    // Read puzzle block into 2D arrays
    for (i = 0; i < bSize; i++)
    {
        *(block + i) = (char *)malloc(bSize * sizeof(char));
        for (j = 0; j < bSize - 1; ++j)
        {
            fscanf(fptr, "%c ", *(block + i) + j);
        }
        fscanf(fptr, "%c \n", *(block + i) + j);
    }
    fclose(fptr);

    printf("Enter the word to search: ");
    scanf("%s", word);

    // Print out original puzzle grid
    printf("\nPrinting puzzle before search:\n");
    printPuzzle(block);

    // Call searchPuzzle to the word in the puzzle
    searchPuzzle(block, word);

    return 0;
}

void printPuzzle(char **arr)
{
    // This function will print out the complete puzzle grid (arr).
    // It must produce the output in the SAME format as the samples
    // in the instructions.

    int i, j;

    for (i = 0; i < bSize; i++)
    {
        for (j = 0; j < bSize; j++)
        {
            printf("%c\t", *(*(arr + i) + j));
        }
    printf("\n");
    }

}


char toLower(char ch)
{
    if (ch >= 'A' && ch <= 'Z')
    {   
        return ch + ('a' - 'A');
    }
    return ch; 
}


int dfs_Search(char **arr, int *path, int row, int col, char *word, int index)
{

    // Recursive search function that matches the word starting at (row, col)
    // for the character at word[index]. 'path' holds the search path in a one-dimensional
    // represenation of the grid.

    // Base case, end of the word
    if (*(word + index) == '\0')
        return 1;

    // Check grid boundaries 
    if (row < 0 || row >= bSize || col < 0 || col >= bSize)
        return 0;

    // Get the current character from the grid and the word.
    char gridChar = *(*(arr + row) + col);
    char wordChar = *(word + index);

    // If characters don't match (ignoring case), return 
    if (toLower(gridChar) != toLower(wordChar))
        return 0;

    // Determine the current cell index in the one-dimensional path array.
    int position = row * bSize + col;
    int original = *(path + position);

    // Append the path order visited  (index + 1) to the current path cell
    // Use a multipler (10 for numbers less than 10, else do 100) for formatting
    int char_order = index + 1;
    int multiplier = (char_order < 10) ? 10 : 100;
    *(path + position) = original * multiplier + char_order;

    // searching through all 8 directions and recursively call function
    for (int d = 0; d < 8; d++){
        int curr_Row = row + *(directions +  2 * d);
        int curr_Col = col + *(directions + 2 * d + 1);
        if (dfs_Search(arr, path, curr_Row, curr_Col, word, index + 1) == 1){
            return 1;
        }
    }

    // Backtracking: reset the appended path number
    *(path + position) = original; 
    return 0;
}

void searchPuzzle(char **arr, char *word){
    // This function checks if arr contains the search word. If the
    // word appears in arr, it will print out a message and the path
    // as shown in the sample runs. If not found, it will print a
    // different message as shown in the sample runs.
    // Your implementation here...
    
    int arrSize = bSize * bSize;
    int *answerPath = (int*)malloc(arrSize * sizeof(int));

    if (answerPath == (void *)0){
        printf("Memeory allocatino error for solution path\n");
        return;
    }
    
    // convert word to lowercase
    for (int i = 0; word[i]; i++){
        word[i] = toLower(word[i]);
    }

    // create solution path array to 0 for each row
    for (int i = 0; i < arrSize; i++){
        *(answerPath + i) = 0;
    }

    // Recurse the whole array till word is found
    int found = 0;
    for (int row = 0; row < bSize && !found; row++){
        for (int col = 0; col < bSize && !found; col++){
            if (dfs_Search(arr, answerPath, row, col, word, 0) == 1){
                found = 1; // word found
                break;
            }
        }
    }

    if (found != 0) {
        printf("Word found!\n Printing the search path:\n");

        for (int i = 0; i < bSize; i++){
            for (int j = 0; j < bSize; j++){
                int position = i * bSize + j; // explain why we add position
                printf("%d\t", *(answerPath + ((i * bSize) + j)));
            }
            printf("\n");
        }
        
    }
    
    else
        printf("Word not found.\n");

    free(answerPath);

}