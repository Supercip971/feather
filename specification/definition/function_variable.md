# variable definition

```
{type} name;
or
{type} name = {math expression};
``` 

# function definition

```
[{attributes}] {type} name({args}); // pre definition
or
[{attributes}] {type} name({args})
{
   {code}
};
``` 

#class definition

```
class {name}
{
public:
    {public member}    

private:
    {private member}
}
```

# struct definition

```c++

struct {name}
{
    {members}
};
```
----
# class function
```c++
{type} {class_name}.{func_name}({args})
{

}
```

# lambda definition

```c++
my_lambda = $(args){my code}

my_lambda = ${my code};
```

exemple: 

```c++
my_array.for_each(
        $(int* entry){ *entry = 10;}
        );
// for lambda in function that specify the arg for exemple:
for_each(lambda(type *entry) func);
// you can do directly:

my_array.foreach(${*entry = 10;})

my_array.for_each(func.set(10));
```

# variable type

if you want a variable type or a func type you can do:

```c++
struct my_func_type(type@ arg1, type@ mapped, int@ size)
{
}
// so when you use my_func_type you need to do

my_func_type(char, char, 10) my_value;

// you can do specific implementation 

struct my_func_type(type@ arg1)
{


    size_t size()
    {
        return 10;
    }
}

size_t my_func_type(char).size()
{
    return 8;
}

```

# variable function

```C++

T my_function(type@ T)
{
    return T(1);    
}
````



