#include <stdio.h>

// Function declarations (prototypes)
int sum(int a, int b);
int sub(int a, int b);
int mul(int a, int b);
int divide(int a, int b);
int mod(int a, int b);
int calculate(int a, int b, int (*op)(int, int));

int main() {
    int (*operations[])(int, int) = {sum, sub, mul, divide, mod};
    char ops[] = {'+', '-', '*', '/', '%'};
    int num1, num2, choice;

    while (1) {
        // Display menu
        printf("\n====== Calculator Menu ======\n");
        printf("1. Addition (+)\n");
        printf("2. Subtraction (-)\n");
        printf("3. Multiplication (*)\n");
        printf("4. Division (/)\n");
        printf("5. Modulus (%%)\n");
        printf("6. Exit\n");
        printf("=============================\n");

        printf("Enter your choice (1-6): ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Exiting...\n");
            break;
        }

        if (choice == 6) {
            printf("Exiting...\n");
            break;
        }

        if (choice < 1 || choice > 5) {
            printf("Invalid choice! Try again.\n");
            continue;
        }

        printf("Enter first number: ");
        if (scanf("%d", &num1) != 1) {
            printf("Invalid input! Try again.\n");
            continue;
        }

        printf("Enter second number: ");
        if (scanf("%d", &num2) != 1) {
            printf("Invalid input! Try again.\n");
            continue;
        }

        int index = choice - 1;
        int result = calculate(num1, num2, operations[index]);
        printf("\n%d %c %d = %d\n", num1, ops[index], num2, result);
    }

    return 0;
}

// Function definitions (after main)

// Recursive addition
int sum(int a, int b) {
    if (b == 0) return a;
    return sum(a + 1, b - 1);
}

// Recursive subtraction
int sub(int a, int b) {
    if (b == 0) return a;
    return sub(a - 1, b - 1);
}

// Recursive multiplication
int mul(int a, int b) {
    if (b == 0) return 0;
    return a + mul(a, b - 1);
}

// Recursive division (via repeated subtraction)
int divide(int a, int b) {
    if (b == 0) return 0;       // avoid division by zero
    if (a < b) return 0;        // base case
    return 1 + divide(a - b, b);
}

// Recursive modulus (via repeated subtraction)
int mod(int a, int b) {
    if (b == 0) return 0;       // avoid division by zero
    if (a < b) return a;        // base case
    return mod(a - b, b);
}

// Function pointer handler
int calculate(int a, int b, int (*op)(int, int)) {
    return op(a, b);
}