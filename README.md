# Calc++
A scientific calculator made in C++.

Uses the [shunting yard algorithm](https://en.wikipedia.org/wiki/Shunting_yard_algorithm) to parse input to be evaluated.

![Example](images/calc++.gif)

* Syntax rules follow regular math syntax. The calculator follows order of operations.
* ~Implicit multiplication is supported. i.e. `5cos(45)5` is a valid expression.~ Implicit multiplication is not fully working properly right now.
* You can use the output of the previous calculation by using the `ans` constant
* Function parameters must be parenthesized i.e. `cos(45)`.
* You can change degrees/radians mode with `!deg` and `!rad`. By default, the calculator is set to degrees.

Note: The syntax checker may not catch every input error.

## Commands
```
!deg - Change to degrees mode
!rad - Change to radians mode
```
## Operators Supported
```
() Parenthesis
 ^ Exponent
 * Multiply
 / Divide
 + Addition
 - Subtraction
 % Modulo
```

## Constants Supported
```
e, pi, ans (previous answer)
```

## Functions Supported
```
sin, cos, tan
csc, sec, cot

asin, acos, atan

asinh, acosh, atanh
sinh, cosh, tanh

log, ln, sqrt, abs

```
