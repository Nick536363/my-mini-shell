# A simple Unix shell written in C

## About


This project represents own written simple command interpreter for UNIX systems in the POSIX standard for educational purposes. It has no practical application because this project is educational only.


## Installation

To install and compile this project you'll need Git, GCC and make.

1. Download the repository

```bash
git clone https://github.com/Nick536363/my-mini-shell
```
    
2. Change working directory

```bash
cd my-mini-shell
```

3. Compile project using make or use GCC insted.

```bash
make
```


## Usage

### Running programs

You can run programs, just as default. Type the programm name and its done.

```bash
$ ./mysh 
/home/ λ ls
nickf
/home/ λ 
```

You can run a single program in the background by adding the **% symbol** to the end.

```bash
$ ./mysh 
/home/ λ ls -la %
[25521] ls - background
/home/ λ total 12
drwxr-xr-x  3 root  root  4096 Jun 27 16:05 .
drwxr-xr-x 16 root  root  4096 May 31 16:57 ..
drwxr-xr-x 32 nickf nickf 4096 Jun 29 00:55 nickf

/home/ λ 
```

### Pipes

You can use pipes by separating commands with the **| symbol**.

```bash
$ ./mysh
/home/ λ seq 1 100 | grep 2 | grep 5
25
52
/home/ λ 
```

### Redirection

You can use single redirection by using the **< or > symbol**.

**> symbol** will redirect *stdout of the program* to *the selected file*. If the file doesnt exists, it will be created.

```bash
$ sudo ./mysh
/home/ λ ls > file.txt
/home/ λ cat file.txt
file.txt
nickf
/home/ λ 
```

**< symbol** will redirect *stdin of selected file* to *stdin of the program*.

```bash
$ ./mysh
/home/ λ cat < file.txt
file.txt
nickf
/home/ λ 
```

## Notes
- Only single redirection is available now. So you cant redirect stdin and stdout at the same time.
- Redirection and pipes are separated now. So you cant use redirection with pipes at the same time.
