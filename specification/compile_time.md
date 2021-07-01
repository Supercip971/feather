# compile time

## what are compile time things ?

you can use compile time variables or/and function

### compile time variable
for exemple a compile time variable:

```c++

int@ my_compile_time_value = 10;

```

a compile time variable can't be unitisialized

### compile time function

to use a compile time function you need to add '@' before it's parameters
```c++
int my_function@()
{
    // a compile time function !
   return my_compile_time_value; 
}
```

