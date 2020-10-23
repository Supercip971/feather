#pragma once
#include <cstdio>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
namespace fsl
{

    template <typename id, typename co>
    struct feather_map_element
    {
        id identifier;
        co code;
    };

    template <typename id, typename co>
    class feather_map
    {
        feather_map_element<id, co> *element;
        uint64_t current_list_length = 0;

    public:
        feather_map();
        feather_map(const feather_map_element<id, co> *elements, int elements_length);
        feather_map(feather_map_element<id, co> *elements, int elements_length);

        co find_from_id(id identifier);
        id find_from_co(co code);

        co find_from_id_verification(id identifier, bool (*verification)(id, id));
        id find_from_co_verification(co code, bool (*verification)(co, co));

        void push_back(const feather_map_element<id, co> element_to_add);

        feather_map_element<id, co> get_entry(uint64_t entry_id);
        void set_entry(uint64_t entry_id, feather_map_element<id, co> element_to_set);

        co operator[](id id_to_find);
        id operator[](co code_to_find);

        inline constexpr uint64_t get_list_length()
        {
            return current_list_length;
        }
    };
    template <typename id, typename co>
    feather_map<id, co>::feather_map()
    {
    }

    template <typename id, typename co>
    feather_map<id, co>::feather_map(const feather_map_element<id, co> *elements, int length)
    {
        element = reinterpret_cast<feather_map_element<id, co> *>(malloc(length * sizeof(feather_map_element<id, co>)));
        memcpy(element, elements, length * sizeof(feather_map_element<id, co>));
        current_list_length = length;
    }

    template <typename id, typename co>
    feather_map<id, co>::feather_map(feather_map_element<id, co> *elements, int length)
    {
        element = reinterpret_cast<feather_map_element<id, co> *>(malloc(length * sizeof(feather_map_element<id, co>)));
        memcpy(element, elements, length * sizeof(feather_map_element<id, co>));
        current_list_length = length;
    }

    template <typename id, typename co>
    co feather_map<id, co>::find_from_id(id identifier)
    {
        for (uint64_t i = 0; i < current_list_length; i++)
        {
            if (element[i].identifier == identifier)
            {
                return element[i].code;
            }
        }
        return 0;
    }
    template <typename id, typename co>
    id feather_map<id, co>::find_from_co(co code)
    {
        for (uint64_t i = 0; i < current_list_length; i++)
        {
            if (element[i].code == code)
            {
                return element[i].identifier;
            }
        }
        return 0;
    }

    template <typename id, typename co>
    co feather_map<id, co>::find_from_id_verification(id identifier, bool (*verification)(id, id))
    {
        for (uint64_t i = 0; i < current_list_length; i++)
        {
            if (verification(element[i].identifier, identifier))
            {
                return element[i].code;
            }
        }
        return 0;
    }
    template <typename id, typename co>
    id feather_map<id, co>::find_from_co_verification(co code, bool (*verification)(co, co))
    {
        for (uint64_t i = 0; i < current_list_length; i++)
        {
            if (verification(element[i].code, code))
            {
                return element[i].identifier;
            }
        }
        return 0;
    }

    template <typename id, typename co>
    void feather_map<id, co>::push_back(const feather_map_element<id, co> element_to_add)
    {
        current_list_length++;
        element = reinterpret_cast<feather_map_element<id, co> *>(realloc(element, current_list_length * sizeof(feather_map_element<id, co>)));
        element[current_list_length] = element_to_add;
    }

    template <typename id, typename co>
    feather_map_element<id, co> feather_map<id, co>::get_entry(uint64_t entry_id)
    {
        return element[entry_id];
    }

    template <typename id, typename co>
    void feather_map<id, co>::set_entry(uint64_t entry_id, feather_map_element<id, co> element_to_set)
    {
        element[entry_id] = element_to_set;
    }

    template <typename id, typename co>
    co feather_map<id, co>::operator[](id id_to_find)
    {
        for (uint64_t i = 0; i < current_list_length; i++)
        {
            if (element[i].identifier == id_to_find)
            {
                return (element[i].code);
            }
        }
        co temp = 0;
        return temp;
    }

    template <typename id, typename co>
    id feather_map<id, co>::operator[](co code_to_find)
    {
        for (uint64_t i = 0; i < current_list_length; i++)
        {
            if (element[i].code == code_to_find)
            {
                return element[i].identifier;
            }
        }
        id temp = 0;
        return temp;
    }

} // namespace fsl