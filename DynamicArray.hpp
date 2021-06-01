#ifndef _DYNAMIC_ARRAY_
#define _DYNAMIC_ARRAY_

#include <stdexcept>
#include <cassert>

#include "Buffer.hpp"

/**
 * @brief DynamicArray class is a class template that stores elements of a given type in a linear arrangement and
 *  gives access to any element
 * 
 * @tparam Type - type of data stored in the array
 */
template <class Type>
class DynamicArray {
private:
    Buffer<Type> buffer;
    size_t used;

private:
    void resizeBuffer();
    void resizeBuffer(size_t size);

public:
    DynamicArray();
    DynamicArray(size_t size);
    DynamicArray(const DynamicArray<Type>&);
    DynamicArray<Type>& operator=(const DynamicArray<Type>&);

public:
    void push_back(const Type&);
    void pop_back();

    Type& at(size_t);
    const Type& at(size_t)const;

    Type& operator[](size_t);
    const Type& operator[](size_t)const;

    Type& front();
    const Type& front()const;

    Type& back();
    const Type& back()const;

    size_t size()const;
    size_t capacity()const;
    bool empty()const;

    void clear();
    void resize(size_t, Type value = Type());
    void reserve(size_t);  
};

/**
 * @brief resizes the array by doublig the capacity if the array isn't empty and maiking it 4 otherwise
 */
template <class Type>
void DynamicArray<Type>::resizeBuffer()
{
    if(buffer.size() == 0)
    {
        Buffer<Type> temp(4);
        buffer.swap(temp);
        return;
    }

    Buffer<Type> temp(buffer.size() * 2, buffer);
    buffer.swap(temp);
}

/**
 * @brief resizes the array with a spesific size
 *  - if the size is equal to the current capacity it does nothing
 *  - if it is smaller -  resizes to the specified size
 *  - if it is bigger - chooses the bigger value between the doubled capacity and the specified size
 * 
 * @param size - new size of the array
 */
template <class Type>
void DynamicArray<Type>::resizeBuffer(size_t size)
{
    if(size == buffer.size())
        return;

    if(size < buffer.size() * 2 && size > buffer.size())
        size = buffer.size() * 2;

    used = size < used ? size : used;

    Buffer<Type> temp(size, used, buffer);
    buffer.swap(temp);
}

/**
 * @brief Construct a new Dynamic Array object
 */
template <class Type>
DynamicArray<Type>::DynamicArray() : used(0)
{

}

/**
 * @brief Construct a new Dynamic Array object with a specific size
 * 
 * @param size 
 */
template <class Type>
DynamicArray<Type>::DynamicArray(size_t size) : buffer(size), used(0) 
{

}

/**
 * @brief Construct a new Dynamic Array object with a copy of each of the elements in other
 * 
 * @param other - container from which to copy the elements
 */
template <class Type>
DynamicArray<Type>::DynamicArray(const DynamicArray<Type>& other) : buffer(other.capacity(), other.used, other.buffer), used(other.used)
{

}

/**
 * @brief Assigns new content to the container, replacing the current elements and modifying its size
 * 
 * @param other - container from which to copy the elements
 * @return DynamicArray<Type>& 
 */
template <class Type>
DynamicArray<Type>& DynamicArray<Type>::operator=(const DynamicArray<Type>& other)
{
    if(this != &other) 
    {
        Buffer<Type> temp(other.capacity(), other.used, other.buffer);
        buffer.swap(temp);
        
        used = other.used;
    }
    return *this;
}

/**
 * @brief adds a new item to the end of the container, modifying its size if necessary
 * 
 * @param elem - element to be pushed
 */
template <class Type>
void DynamicArray<Type>::push_back(const Type& elem)
{
    if(used >= buffer.size())
        resizeBuffer();

    buffer[used++] = elem;
}

/**
 * @brief deletes the element at the end of the vector
 */
template <class Type>
void DynamicArray<Type>::pop_back()
{
    if(used > 0)
        --used;
}

/**
 * @brief returns a reference to the element at a specified index.
 * 
 * @param index - index of the element to be returned
 * @return Type& 
 */
template <class Type>
Type& DynamicArray<Type>::at(size_t index)
{
    if(index < used)
        return buffer[index];
    throw std::out_of_range("The index is out of range!");
}

/**
 * @brief returns a constant reference to the element at a specified index.
 * 
 * @param index - index of the element to be returned
 * @return const Type& 
 */
template <class Type>
const Type& DynamicArray<Type>::at(size_t index)const
{
    return const_cast<DynamicArray<Type>*>(this)->at(index);
}

/**
 * @brief returns a reference to the element at a specified index.
 * 
 * @param index - index of the element to be returned
 * @return Type& 
 */
template <class Type>
Type& DynamicArray<Type>::operator[](size_t index)
{
    assert(index < used);
    return buffer[index];
}

/**
 * @brief returns a sonstant reference to the element at a specified index.
 * 
 * @param index - index of the element to be returned
 * @return Type& 
 */
template <class Type>
const Type& DynamicArray<Type>::operator[](size_t index)const
{
    return const_cast<DynamicArray<Type>*>(this)->operator[](index);
}

/**
 * @brief return a reference to the first element
 * 
 * @return Type& 
 */
template <class Type>
Type& DynamicArray<Type>::front()
{
    if(!empty())
        return buffer[0];
    throw std::out_of_range("The array is empty!");
}

/**
 * @brief return a constant reference to the first element
 * 
 * @return Type& 
 */
template <class Type>
const Type& DynamicArray<Type>::front()const
{
    return const_cast<DynamicArray<Type>*>(this)->front();
}

/**
 * @brief return a reference to the last element
 * 
 * @return Type& 
 */
template <class Type>
Type& DynamicArray<Type>::back()
{
    if(!empty())
        return buffer[used - 1];
    throw std::out_of_range("The array is empty!");
}

/**
 * @brief return a constant reference to the first element
 * 
 * @return Type& 
 */
template <class Type>
const Type& DynamicArray<Type>::back()const
{
    return const_cast<DynamicArray<Type>*>(this)->back();
}

/**
 * @brief returns the number of the elements in the container
 * 
 * @return size_t 
 */
template <class Type>
size_t DynamicArray<Type>::size()const
{
    return used;
}

/**
 * @brief returns the capacity of the container
 * 
 * @return size_t 
 */
template <class Type>
size_t DynamicArray<Type>::capacity()const
{
    return buffer.size();
}

/**
 * @brief checks of the container is empty
 * 
 * @return true 
 * @return false 
 */
template <class Type>
bool DynamicArray<Type>::empty()const
{
    return used == 0;
}

/**
 * @brief erases the elements of the container 
 */
template <class Type>
void DynamicArray<Type>::clear()
{
    buffer.clear();

    used = 0;
}

/**
 * @brief resizes the array with a spesific size and fills it with a specific value
 *  - if the size is equal to the current capacity it does nothing
 *  - if it is smaller -  resizes to the specified size
 *  - if it is bigger - chooses the bigger value between the doubled capacity and the specified size
 *  
 * @param size - new size of the array
 * @param value - value with which to fill the array
 */
template <class Type>
void DynamicArray<Type>::resize(size_t size, Type value)
{
    resizeBuffer(size);

    for(size_t i = used; i < buffer.size(); ++i)
    {
        push_back(value);
    }
}

/**
 * @brief resizes the array with a spesific size
 *  - if the size is equal to the current capacity it does nothing
 *  - if it is smaller -  resizes to the specified size
 *  - if it is bigger - chooses the bigger value between the doubled capacity and the specified size
 * 
 * @param size - new size of the array
 */
template <class Type>
void DynamicArray<Type>::reserve(size_t size)
{
    resizeBuffer(size);
}

#endif