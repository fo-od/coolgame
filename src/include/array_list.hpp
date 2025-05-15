#pragma once

#include "types.hpp"

class Array_List
{
    usize len = 0;
    usize capacity{};
    usize item_size{};
    void *items{};

    public:
        Array_List() = default;

        Array_List( usize item_size, usize initial_capacity );

        ~Array_List();

        usize append( const void *item );

         [[nodiscard]] void *get( usize index ) const;

        bool remove( usize index );
};
