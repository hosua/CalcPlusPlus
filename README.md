# MathExprSolver 
A C++ program that lexes and parses a mathematical expression string, and then calculates the result.

Uses the [shunting yard algorithm](https://en.wikipedia.org/wiki/Shunting_yard_algorithm) to parse input to be evaluated.

As of now, the syntax checker probably won't catch every error.

## Operators Supported
```
() Parenthesis
 ^ Exponent
 * Multiply
 / Divide
 + Addition
 - Subtraction
```

## Functions Supported
```
sin, cos, tan
csc, sec, cot
```

Note that you must parenthesize function parameters, i.e. cos(45).

## To be added (maybe?)
```
log, ln
sqrt, abs
asin, acos, atan
sinh, cosh, tanh
asinh, acosh, atanh

Some constants, like e and pi
```

![Example](images/math-expr-solver.gif)