#pragma once

#include <stdlib.h>

namespace fsl {
    template <typename T>
    class feather_list{
        unsigned int allocated_length = 0;
        unsigned int data_length = 0;
        T* data;
    public:
        inline constexpr
        unsigned int get_length(){
            return data_length;
        }

        void create();

        void increase(unsigned int new_length);

        void push(T d);

        void remove_entry(unsigned int entry_id);



        T* get_entry(unsigned int entry_id);
        inline T* operator[](unsigned int entry_id){
            return get_entry(entry_id);
        }
    };


    template <typename T>
    void feather_list<T>::create(){
        data = malloc(sizeof (T));
        allocated_length = 1;
        data_length = 0;
    }

    template <typename T>
    void feather_list<T>::increase(unsigned int new_length){
        if(allocated_length < new_length){
            data = realloc(data, new_length*sizeof (T));
            data_length = new_length;
            allocated_length = new_length;
        }else if(allocated_length > data_length){
            data_length = allocated_length;
        }
    }

    template <typename T>
    void feather_list<T>::push(T d){
        increase(data_length + 1);
        data[data_length-1] = d;
    }
    template <typename T>
    void feather_list<T>::remove_entry(unsigned int entry_id){
        for(int i =entry_id; i< data_length; i++){
            data[i] = data[i+1];
        }
        data_length--;
    }

    template <typename T>
    T* feather_list<T>::get_entry(unsigned int entry_id){
        return data[entry_id];
    }

}