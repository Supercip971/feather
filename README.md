# FEATHER
feather is a scripting language for WingOS (my kernel) 
it is a c++ scripting language with minimal dependencies in mind for the moment the only dependencies are : 

 - stdint.h header (uint64_t 32_t ...)
 - printf (with %x %i and %s support)
 - memcpy
 - malloc/realloc/free
 - strcmp/strncmp
 - stddef.h header (nullptr)

sample code 
```feather
func myfunction() -> int {
    int i = 13 ;
    int v = i - 3 + 4 * 2 ;
    return v ;
}
func main(){
    myfunction();
}
```

note: 

thank @N-LG @Freeloo and @meaxycomputer for the programming language name !
