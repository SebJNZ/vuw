# MinMax(s) Algorithm
Calculates the Minimum and Maximum points for a set of numbers of size $n$, where $n$ is a power of two.

## What is the cost of this algorithm?
The cost of this algorithm is $O(n)$.

Worst case: $O(n)$

Best case: $O(n)$

## Usage
By default, the size of the array is $8$, you can change the constant stored at the top of the file:

`#define SIZE 8` $\longrightarrow$ `#define SIZE n` where $2|n$.

Compile and Run:
```
gcc main.c -o main
./main
```

## Visualise
If you wish to visualise the array's being cut in half, you can uncomment the line at $37$
