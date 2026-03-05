# HW2-CS3339

Homework 2 - Binary Representation, Floating-point Math and Bit Manipulation

**Student:** Ly Huu Nhan To (Harry)

## Build

```bash
g++ -std=c++17 -o fp_overflow_checker main.cpp fp_utils.cpp
```

## Run

```bash
./fp_overflow_checker <loop_bound> <loop_counter>
```

Both arguments must be positive floating-point values.

### Examples

**Invalid usage (wrong number of arguments):**

```
$ ./fp_overflow_checker
usage:
    ./fp_overflow_checker loop_bound loop_counter

    loop_bound is a positive floating-point value
    loop_counter is a positive floating-point value
```

**Overflow detected:**

```
$ ./fp_overflow_checker 1e+08 1.0
Loop bound:   0 10011001 01111101011110000100000
Loop counter: 0 01111111 00000000000000000000000
Warning: Possible overflow!
Overflow threshold:
	1.67772e+07
	0 10010111 00000000000000000000000
```

**No overflow (loop bound below threshold):**

```
$ ./fp_overflow_checker 1e+05 2.0
Loop bound:   0 10001111 10000110101000000000000
Loop counter: 0 10000000 00000000000000000000000
There is no overflow!
```

**No overflow (small values):**

```
$ ./fp_overflow_checker 1.4567 3.14
Loop bound:   0 01111111 01110100111010100100101
Loop counter: 0 10000000 10010001111010111000011
There is no overflow!
```

**Only one argument provided:**

```
$ ./fp_overflow_checker 17.0
usage:
    ./fp_overflow_checker loop_bound loop_counter

    loop_bound is a positive floating-point value
    loop_counter is a positive floating-point value
```
