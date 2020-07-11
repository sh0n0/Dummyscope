# Dummyscope

Dummyscope is a simplified version of [Kaleidoscope](https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/index.html) and its code structure is similar to [DummyCCompiler](https://github.com/Kmotiko/DummyCCompiler).

## Example
The example of dummyscope source code in the example directory is shown below.
```
def add(x y)
  x + y

def sub(x y)
  x - y

def mul(x y)
  x * y

def div(x y)
  x / y

def square(n)
  n * n

def main()
  print(add(2, 2) + sub(3, 1) * mul(3, 4) - div(square(4), 2))
```

You can see the result by following the commands:
```
$ cd example
$ ./dsc calc.ds
$ ./calc
20
```
