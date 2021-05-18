#include <iostream>
#include <stdexcept>
#include <cassert>

//I - allocated + used => CopyCnstr + operator=
//II - allocated => Buffer(size_t size, const Buffer& other)
//III - operator= - exception safety
//IV - Buffer(size_t size, const Buffer& other)?

template <class Type>
class Buffer{
private:
    Type* data;
    size_t allocated;
//  size_t used;

private:
    void copyFrom(size_t, size_t, const Buffer<Type>&);
//  void free();

public:
    Buffer();
    Buffer(size_t);
//    Buffer(size_t, const Buffer<Type>&);
    Buffer(size_t, size_t, const Buffer<Type>&);
    Buffer(const Buffer<Type>&) = delete;
    Buffer<Type>& operator=(const Buffer<Type>&) = delete;
    ~Buffer();

public:
    size_t size()const;

    void swap(Buffer<Type>&);

    Type& operator[](size_t);
//    const Type& operator[](size_t)const;

    void clear();
};

/*
template <class Type>
void Buffer<Type>::copyFrom(const Buffer<Type>& other)
{
    data = new Type[other.allocated];

    for(size_t i = 0; i < other.used; ++i)
    {
        data[i] = other.data[i];
    }

    allocated = other.allocated;
    used = other.used;
}
*/

template <class Type>
void Buffer<Type>::copyFrom(size_t size, size_t elementsToCopy, const Buffer<Type>& other)
{
    size_t used = elementsToCopy < size ? elementsToCopy : size;

    for(size_t i = 0; i < used; ++i)
    {
        data[i] = other.data[i];
    }
}

template <class Type>
Buffer<Type>::Buffer() : data(nullptr), allocated(0)
{

}

template <class Type>
Buffer<Type>::Buffer(size_t size) : data(new Type[size]), allocated(size)
{ 

}

/*
template <class Type>
Buffer<Type>::Buffer(size_t size, const Buffer<Type>& other) : data(new Type[size]), allocated(size)
{
    copyFrom(size, other.allocated, other);
}
*/

template <class Type>
Buffer<Type>::Buffer(size_t size, size_t elementsToCopy, const Buffer<Type>& other) : data(new Type[size]), allocated(size)
{
    copyFrom(size, elementsToCopy, other);
}

/*
template <class Type>
Buffer<Type>::Buffer(const Buffer<Type>& other)
{
    copyFrom(other);
}

template <class Type>
Buffer<Type>& Buffer<Type>::operator=(const Buffer<Type>& other)
{
    if(this != &other)
    {
        // Buffer<Type> temp(other);
        // swap(temp);

        
    }
}
*/

template <class Type>
Buffer<Type>::~Buffer()
{
    clear();
}

template <class Type>
size_t Buffer<Type>::size()const
{
    return allocated;
}

template <class Type>
void Buffer<Type>::swap(Buffer<Type>& other)
{
    std::swap(data, other.data);
    std::swap(allocated, other.allocated);
}

template <class Type>
Type& Buffer<Type>::operator[](size_t index)
{
    assert(index < allocated);

    return data[index];
}

/*
template <class Type>
const Type& Buffer<Type>::operator[](size_t index)const
{
    assert(index < allocated);

    return data[index];
}
*/

template <class Type>
void Buffer<Type>::clear()
{
    delete[] data;

    allocated = 0;
}

//V - resizeBuffer - default params
template <class Type>
class DynamicArray {
private:
    Buffer<Type> buffer;
    size_t used;

private:
    void resizeBuffer(size_t size, size_t elementsToCopy);

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

template <class Type>
void DynamicArray<Type>::resizeBuffer(size_t size, size_t elementsToCopy)
{
    if(size == buffer.size())
        return;

    Buffer<Type> temp(size, elementsToCopy, buffer);
    buffer.swap(temp);
}

template <class Type>
DynamicArray<Type>::DynamicArray() : used(0)
{

}

template <class Type>
DynamicArray<Type>::DynamicArray(size_t size) : buffer(size), used(0) 
{

}

template <class Type>
DynamicArray<Type>::DynamicArray(const DynamicArray<Type>& other) : buffer(other.capacity(), other.used, other.buffer), used(other.used)
{

}

template <class Type>
DynamicArray<Type>& DynamicArray<Type>::operator=(const DynamicArray<Type>& other)
{
    if(this != &other) 
    {
        Buffer<Type> temp(other.capacity(), other.used, other.buffer);
        buffer.swap(temp);
    }
    return *this;
}

template <class Type>
void DynamicArray<Type>::push_back(const Type& elem)
{
    if(used >= buffer.size())
        resizeBuffer(buffer.size() * 2, buffer.size());

    buffer[used++] = elem;
}

template <class Type>
void DynamicArray<Type>::pop_back()
{
    if(used > 0)
        --used;
}

template <class Type>
Type& DynamicArray<Type>::at(size_t index)
{
    if(index < used)
        return buffer[index];
    throw std::out_of_range("The index is out of range!");
}

template <class Type>
const Type& DynamicArray<Type>::at(size_t index)const
{
    return const_cast<DynamicArray<Type>*>(this)->at(index);
}

template <class Type>
Type& DynamicArray<Type>::operator[](size_t index)
{
    assert(index < used);
    return buffer[index];
}

template <class Type>
const Type& DynamicArray<Type>::operator[](size_t index)const
{
    return const_cast<DynamicArray<Type>>(*this)->operator[](index);
}

template <class Type>
Type& DynamicArray<Type>::front()
{
    if(!empty())
        return buffer[0];
    throw std::out_of_range("The array is empty!");
}

template <class Type>
const Type& DynamicArray<Type>::front()const
{
    return const_cast<DynamicArray<Type>*>(this)->front();
}

template <class Type>
Type& DynamicArray<Type>::back()
{
    if(!empty())
        return buffer[used - 1];
    throw std::out_of_range("The array is empty!");
}

template <class Type>
const Type& DynamicArray<Type>::back()const
{
    return const_cast<DynamicArray<Type>*>(this)->back();
}

template <class Type>
size_t DynamicArray<Type>::size()const
{
    return used;
}

template <class Type>
size_t DynamicArray<Type>::capacity()const
{
    return buffer.size();
}

template <class Type>
bool DynamicArray<Type>::empty()const
{
    return used == 0;
}

template <class Type>
void DynamicArray<Type>::clear()
{
    buffer.clear();

    used = 0;
}

//1, 2, 3, 4, 5, 6, 7 -- size = 4
//1, 2, 3, 4 -- size = 8, used = 4
template <class Type>
void DynamicArray<Type>::resize(size_t size, Type value)
{
    resize(size, used);

    for(size_t i = used; i < size; ++i)
    {
        push_back(value);
    }
}

template <class Type>
void DynamicArray<Type>::reserve(size_t size)
{
    resizeBuffer(size, used);
}

int main()
{
    DynamicArray<int> v(5);
    v.push_back(5);

    const DynamicArray<int> v1 = v;

    const int a = v1.back();
    std::cout << a << std::endl;

    return 0;
}
