# c-program
my c programs 
A pointer named operations(also an array) is declared which stores the adress of the functions {sum, sub, mul, divide, mod}
the character array ops[] is made to make the output more undestanding to the user.

The while loop is used to :
1. display the actions that the program can perform
2. take the action to perform input from the user
3. take the input of two numbers from the user
4. the while loop also contains the an option to exit the code
5. make the loop only take the input of options which are available and display an error messagewhen an option other than the given option is selected.

An integer named index is made cause an array starts from 0 and we dont have an 0th option in the menu to perform any operationand 

An integer named result is made to store the function calculate(num1, num2, operations[index]); at line 56 of the code in this line the operations is just a pointer we created and declared the vakue at kube 12 as int (*operations[])(int, int) = {sum, sub, mul, divide, mod};

the line 57 is a bit difficult to undestand as the line says  printf("\n%d %c %d = %d\n", num1, ops[index], num2, result); 
as here num1 and num2 is the value we took from the user in the while loop before and ops[index] is the array which have the symbols of the operations stored like {'+', '-', '*', '/', '%'} and index is the choice the user selected minus 1 (index = choice - 1)

as when the integer named 'result' is displayed it shows the calculate(num1,num2,operation[index]) whcih calls the function calculate , where the calculate function then points to other functions hence the functions named {sum, sub, mul, divide, mod} .

as we can see in the line 98 of the program int calculate(int a, int b, int (*op)(int, int)) { return op(a , b); } 
the op in this function is used to dereference the pointer and access the value inside the pointer , for example if the pointer op points to add function then return op(a , b); looks like return add(a , b);

---

## sort_benchmark.c — Sorting Algorithms Benchmark

Compares three hand-written **O(n²)** sorting algorithms against the standard library **qsort** (O(n log n)):

- **Bubble Sort** — repeatedly swaps adjacent out-of-order pairs, with an early-exit if a pass makes no swaps
- **Selection Sort** — finds the minimum of the unsorted tail and swaps it into place
- **Insertion Sort** — grows a sorted prefix by inserting each next element (like sorting cards in your hand)
- **qsort** — from `<stdlib.h>`, using a `compare_ints` comparator function

Each algorithm sorts an **identical copy of the same random array** (fixed seed 42), timed with `clock()`, at sizes n = 1000, 5000, 10000, 20000. Results are verified with `is_sorted()` after every run.

### Build and run

```bash
gcc -Wall -O2 -o sort_benchmark sort_benchmark.c
./sort_benchmark          # default sizes: 1000, 5000, 10000, 20000
./sort_benchmark 50000    # single custom size (10 to 100000)
```

### What to observe

- The three O(n²) sorts roughly **quadruple** their time when n doubles
- qsort stays fast even at n = 20000
- Insertion sort is the fastest of the O(n²) group on random data (fewer swaps, better cache behavior)
- The **best-case section** (already-sorted input) shows insertion sort dropping to O(n) — nearly instant — while selection sort stays slow, since it doesn't care about existing order
