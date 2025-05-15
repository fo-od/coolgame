#include "array_list.hpp"

#include <cstdlib>

#include "types.hpp"
#include "util.hpp"

Array_List::Array_List( const usize item_size, const usize initial_capacity )
    : capacity(initial_capacity),
      item_size(item_size),
      items(malloc(item_size * initial_capacity))
{
    if ( !items ) {
        free(items);
        ERROR_LOG("Couldn't allocate memory for Array_List\n");
    }
}

Array_List::~Array_List() { free(items); }

usize Array_List::append( const void *item )
{
    if ( this->len == this->capacity ) {
        this->capacity = this->capacity > 0 ? this->capacity + this->capacity / 2 : 1;
        this->items = realloc(this->items, this->capacity * this->item_size);

        if ( !this->items )
            ERROR_RETURN(-1, "Couldn't allocate memory for Array_List\n");
    }

    const usize index = this->len++;

    memcpy(static_cast< u8 * >(this->items) + index * this->item_size, item, this->item_size);

    return index;
}

void *Array_List::get( const usize index ) const
{
    if ( index >= this->len )
        ERROR_RETURN(nullptr, "Index out of bounds\n");

    return static_cast< u8 * >(this->items) + index * this->item_size;
}

bool Array_List::remove( const usize index )
{
    if ( this->len == 0 )
        ERROR_RETURN(false, "this is empty\n");
    if ( index >= this->len )
        ERROR_RETURN(false, "Index out of bounds\n");

    if ( this->len == 1 ) {
        this->len = 0;
        return true;
    }

    --this->len;

    u8 *item_ptr = static_cast< u8 * >(this->items) + index * this->item_size;
    const u8 *end_ptr = static_cast< u8 * >(this->items) + this->len * this->item_size;
    memcpy(item_ptr, end_ptr, this->item_size);

    return true;
}
