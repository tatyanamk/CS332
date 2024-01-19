#include <stdio.h>
int main () {
    //Define an integer variable as given_number
    int given_number;

    //Use the C function scanf to read the integer variable given_number.
    printf("Enter an integer:");
    scanf("%d", &given_number);

    //Use the conditional statement to find out if given_number is an odd or even number.
    if (given_number %2 == 0)
    //Print the final output as “The number is an odd number” or “The number is an even number”
    printf("The number %d is even.\n", given_number);
    else
    printf("The number %d is odd.\n", given_number);
    return 0;
}
