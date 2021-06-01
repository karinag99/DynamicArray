#ifndef _BUFFER_
#define _BUFFER_


#include <stdexcept>
#include <cassert>

/**
 * @brief Buffer class is a class template that stores dynamically allocated array,
 *  taking care of memory allocation and deallocation
 * 
 * @tparam Type - type of data stored in the array
 */
template <class Type>
class Buffer
{
protected:
    Type* data;         ///pointer indicating the dynamically allocated array
    size_t allocated;   ///length of the array

public:
    Buffer();
    Buffer(size_t);
    Buffer(size_t, const Buffer<Type>&);
    Buffer(size_t, size_t, const Buffer<Type>&);
    Buffer(const Buffer<Type>&) = delete;
    Buffer<Type>& operator=(const Buffer<Type>&) = delete;
    ~Buffer();

public:
    size_t size()const;

    void swap(Buffer<Type>&);

    Type& operator[](size_t);

    const Type& operator[](size_t s)const;

    void clear();
};

/**
 * @brief Construct a new Buffer object with zero elements
 */
template <class Type>
Buffer<Type>::Buffer() : Buffer(0)
{

}

/**
 * @brief Construct a new Buffer object with a specific size
 * 
 * @param size - size of the buffer
 */
template <class Type>
Buffer<Type>::Buffer(size_t size) : data(nullptr), allocated(0)
{ 
    if(size > 0)
    {
        data = new Type[size];
        allocated = size;
    }

}

/**
 * @brief Construct a new Buffer object with a specific size and copies all elements from another buffer
 * 
 * @param size  - size of the buffer
 * @param other - buffer from which to copy the elements
 */
template <class Type>
Buffer<Type>::Buffer(size_t size, const Buffer<Type>& other) : Buffer(size, other.allocated, other)
{

}

/**
 * @brief Construct a new Buffer object with a specific size and copies all elements from another buffer
 * 
 * @param size  - size of the buffer
 * @param elementsToCopy - number ot elements to be copied
 * @param other - buffer from which to copy the elements
 */
template <class Type>
Buffer<Type>::Buffer(size_t size, size_t elementsToCopy, const Buffer<Type>& other) : data(nullptr), allocated(0)
{
    if(size > 0) 
    {
        size_t used = size < elementsToCopy ? size : elementsToCopy;
        
        if(other.allocated < used)
            throw std::invalid_argument("Not enough elements in the buffer object");

        data = new Type[size];
        allocated = size;

        for(size_t i = 0; i < used; ++i)
        {
            data[i] = other.data[i];
        }
    }
}

/**
 * @brief Destroy the Buffer object
 */
template <class Type>
Buffer<Type>::~Buffer()
{
    clear();
}

/**
 * @brief returns the capacity of the buffer
 *  
 * @return size_t 
 */
template <class Type>
size_t Buffer<Type>::size()const
{
    return allocated;
}

/**
 * @brief Exchanges the elements of two buffers
 * 
 * @param other - a buffer providing the elements to be swapped
 */
template <class Type>
void Buffer<Type>::swap(Buffer<Type>& other)
{
    if(this != &other)
    {
    std::swap(data, other.data);
    std::swap(allocated, other.allocated);
    }
}

/**
 * @brief returns a reference to the element at a specified index
 * 
 * @param index - the index of the element
 * @return Type& 
 */
template <class Type>
Type& Buffer<Type>::operator[](size_t index)
{
    assert(index < allocated);

    return data[index];
}

/**
 * @brief returns a constant reference to the element at a specified index
 * 
 * @param index - the index of the element
 * @return const Type& 
 */
template <class Type>
const Type& Buffer<Type>::operator[](size_t index)const 
{
    return const_cast<Buffer<Type>*>(this)->operator[](index);
}
    

/**
 * @brief erases the elements of the buffer
 *  
 */
template <class Type>
void Buffer<Type>::clear()
{
    delete[] data;

    allocated = 0;
}

#endif 