# calculator

Basic calculator to do calculations

Has limited support for vectors, matrices, and corresponding operators

## Building
Only tested for Linux

Requirements for building:<br/>
GNU Readline library<br/>
GNU make<br/>

To build:
```
git clone https://github.com/royces9/calculator.git
make
```

To build with optimizations
```
make release
```


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

### Custom functions
create a function like:

function [out] = func(a, b)
out = a + b;

### Current known bugs:
