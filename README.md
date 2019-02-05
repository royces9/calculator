# calculator

Basic calculator to do calculations

Has limited support for vectors, matrices, and corresponding operators

## Currently implemented functions

### Arithmetic operators
addition \+

subtraction \-

multiplication \*

division \/

exponentitation \^

### Trigonometry
sin(), cos(), tan(), asin(), acos(), atan(), pi

### Logarithms and powers
log(), ln(), sqrt(), e

### Miscellaneous functions
avg(), min(), max(), floor(), ceil(), round(), factorial()

### Numerical functions
derivate(), integral(), solve()

### Scripts
run()

### Current known bugs:
9 ^ (((log(2)) / log(3)))
causes a seg fault

9 ^ (log(2) / log(3))
evalutes as (9 ^ log(2)) / log(3)