# shooTR

A game.

### Pre-requirements

* Qt Creator Community or any IDE
* gcc (Strawberry Perl for built-in gcc)
* Python >= 3.11
* pipx
* cmake
* ninja
* clang-format

### clone

```
git clone https://github.com/faithgvn/shootr.git
```


### install build tools

```
pipx install cmake ninja clang-format
```

### configure

In directory `shootr`

```
cmake -B build -G Ninja
```

### build

```
cmake --build build
```

### run

```
./build/shootr
```
