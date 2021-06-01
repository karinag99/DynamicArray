#include "catch.hpp"
#include "../DynamicArray.cpp"

#include <algorithm>

class TestDynamicArray{
public:

    template <class Type>
    static bool haveSameElements(const DynamicArray<Type>& lhs, const DynamicArray<Type>& rhs, size_t elements)
    {
        for(size_t i = 0; i < elements; ++i)
        {
            if(lhs[i] != rhs[i])
                return false;
        }
        return true;
    }

    template <class Type>
    static bool areEqual(const DynamicArray<Type>& lhs, const DynamicArray<Type>& rhs)
    {
        return lhs.capacity() == rhs.capacity() &&
               lhs.size() == rhs.size() &&    
               haveSameElements(lhs, rhs, lhs.size());
    }

    template <class Type>
    static void init(DynamicArray<Type>& arr, size_t used)
    {
        for(size_t i = 0; i < used; ++i)
        {
            arr.push_back(i);
        }
    }

    static bool isValidArray(const DynamicArray<int>& array, size_t size)
    {
        return isValidArray(array, size, size);
    }

    static bool isValidArray(const DynamicArray<int>& array, size_t size, size_t used)
    {
        if(array.capacity() != size)
            return false;

        return hasValidElements(array, used);
    }

    static bool hasValidElements(const DynamicArray<int>& array, size_t used)
    {
        for(size_t i = 0 ; i < used; ++i)
        {
            if(array[i] != i)
                return false;
        }

        return true;
    }
};

SCENARIO("Testing Default contructor")
{   
    GIVEN("An empty array")
    {
        DynamicArray<int> testArray;

        THEN("Size should be zero")
        {
            REQUIRE(testArray.size() == 0); 
        }

        THEN("Capacity should be zero")
        {
            REQUIRE(testArray.capacity() == 0); 
        }

        THEN("Empty should return true")
        {
            CHECK(testArray.empty());
        }

        WHEN("An element is popped")
        {
            testArray.pop_back();

            THEN("Size and capacity should't change")
            {
                REQUIRE(testArray.size() == 0); 
                REQUIRE(testArray.capacity() == 0); 
            }
        }

        WHEN("A random element is accessed")
        {
            THEN("An exception should be thrown")
            {
                REQUIRE_THROWS_AS(testArray.at(5), std::out_of_range);
            }
        }

        WHEN("The front element is accessed")
        {
           THEN("An exception should be thrown")
            {
                REQUIRE_THROWS_AS(testArray.front(), std::out_of_range);
            } 
        }

        WHEN("The back element is accessed")
        {
           THEN("An exception should be thrown")
            {
                REQUIRE_THROWS_AS(testArray.back(), std::out_of_range);
            } 
        }

        WHEN("The content is erased")
        {
            testArray.clear();
            THEN("The array should't change")
            {
                CHECK(TestDynamicArray::areEqual(testArray, DynamicArray<int>()));
            }
        }
    }
}

SCENARIO("Testing constructor with one argument - size")
{
    GIVEN("An array with a specific size")
    {
        const size_t testArraySize = 10;
        DynamicArray<int> testArray(testArraySize);

        THEN("Size should be zero")
        {
            REQUIRE(testArray.size() == 0); 
        }

        THEN("Capacity should be valid")
        {
            REQUIRE(testArray.capacity() == testArraySize); 
        }

        THEN("Empty should return true")
        {
            CHECK(testArray.empty());
        }

        WHEN("An element is popped")
        {
            testArray.pop_back();

            THEN("Size and capacity should't change")
            {
                REQUIRE(testArray.size() == 0); 
                REQUIRE(testArray.capacity() == testArraySize); 
            }
        }

        WHEN("A random element is accessed")
        {
            THEN("An exception should be thrown")
            {
                REQUIRE_THROWS_AS(testArray.at(5), std::out_of_range);
            }
        }

        WHEN("The front element is accessed")
        {
           THEN("An exception should be thrown")
            {
                REQUIRE_THROWS_AS(testArray.front(), std::out_of_range);
            } 
        }

        WHEN("The back element is accessed")
        {
           THEN("An exception should be thrown")
            {
                REQUIRE_THROWS_AS(testArray.back(), std::out_of_range);
            } 
        }

        WHEN("The content is erased")
        {
            testArray.clear();
            THEN("Only the capacity should change")
            {
                REQUIRE(testArray.capacity() == 0);
                REQUIRE(testArray.size() == 0);
            }
        }  
    }
}

SCENARIO("Testing copy assignment")
{
    GIVEN("An object with a specific size")
    {
        const size_t arraySize = 5;

        DynamicArray<int> testArray(arraySize);
        TestDynamicArray::init(testArray, arraySize);

        WHEN("A new array is constructed")
        {
            DynamicArray<int> copy(testArray);

            THEN("The constructed objects should be equal")
            {
                CHECK(TestDynamicArray::areEqual(testArray, copy));
            }

            WHEN("The content of the old object is erased")
            {
                testArray.clear();

                THEN("The new object should be valid")
                {
                    CHECK(TestDynamicArray::isValidArray(copy, arraySize));
                }
            }
        }
    }
}

SCENARIO("Testing assignment operator")
{
   GIVEN("Two objects with a specific size")
    {
        const size_t originSize = 5;

        DynamicArray<int> origin(originSize);
        TestDynamicArray::init(origin, originSize);

        const size_t copySize = 10;

        DynamicArray<int> copy(copySize);
        TestDynamicArray::init(copy, copySize);

        WHEN("The arrays are equalized")
        {
            copy = origin;

            THEN("The sizes of the arrays should be the same")
            {
                REQUIRE(copy.size() == originSize);
                REQUIRE(origin.size() == originSize);
            }

            THEN("Both arrays should have same content as the first array")
            {
                CHECK(TestDynamicArray::isValidArray(origin,originSize));
                CHECK(TestDynamicArray::isValidArray(copy,originSize));
            }

            WHEN("The content of one of the arrays is erased")
            {
                origin.clear();

                THEN("The other should be valid")
                {
                    CHECK(TestDynamicArray::isValidArray(copy,originSize));
                }
            }
        }

        WHEN("An array is equalized to itself") 
        {
            origin = origin;

            THEN("The array should be valid")
            {
                CHECK(TestDynamicArray::isValidArray(origin,originSize));
            }
        }        
    }
}

SCENARIO("Testing push_back function")
{
    GIVEN("A non-empty array with less used elements than capacity")
    {
        const size_t arraySize = 10;
        const size_t used = 5;

        DynamicArray<int> testArray(arraySize);
        TestDynamicArray::init(testArray, used);

        WHEN("An element is added at the end")
        {   
            testArray.push_back(used);

            THEN("The size should be increased with one")
            {
                REQUIRE(testArray.size() == used + 1);
            }

            THEN("The capacity shouldn't chang")
            {
                REQUIRE(testArray.capacity() == arraySize);
            }

            THEN("The current elements of the array should be valid")
            {
                CHECK(TestDynamicArray::hasValidElements(testArray, used));
            }

            THEN("The new element should be accessible")
            {
                REQUIRE(testArray[used] == 5);
            }

            THEN("Back function should return the new element")
            {
                REQUIRE(testArray.back() == 5);
            }
        }
    }

    GIVEN("A non-empty array that does not have enough space for more elements")
    {
        const size_t arraySize = 10;

        DynamicArray<int> testArray(arraySize);
        TestDynamicArray::init(testArray, arraySize);

        WHEN("An element is added at the end")
        {   
            testArray.push_back(10);

            THEN("The size should be increased with one")
            {
                REQUIRE(testArray.size() == arraySize + 1);
            }

            THEN("The capacity be doubled")
            {
                REQUIRE(testArray.capacity() == arraySize * 2);
            }

            THEN("The current elements of the array should be valid")
            {
                CHECK(TestDynamicArray::hasValidElements(testArray, arraySize));
            }

            THEN("The new element should be accessible")
            {
                REQUIRE(testArray[arraySize] == 10);
            }

            THEN("Back function should return the new element")
            {
                REQUIRE(testArray.back() == 10);
            }
        }
    }

    GIVEN("An empty array")
    {
        DynamicArray<int> testArray;

        WHEN("An element is added at the end")
        {
            testArray.push_back(10);

            THEN("Empty should return false")
            {
                CHECK_FALSE(testArray.empty());
            }

            THEN("The size should be 1")
            {
                REQUIRE(testArray.size() == 1);
            }

            THEN("The capacity should be greater than zero")
            {
                REQUIRE(testArray.capacity() > 0);
            }

            THEN("The new element should be accessible")
            {
                REQUIRE(testArray[0] == 10);
            }

            THEN("The front element should be the same as the new one")
            {
                REQUIRE(testArray.front() == testArray[0]);
            }

            THEN("The last element should be the same as the new one")
            {
                REQUIRE(testArray.back() == testArray[0]);
            }


        }
    }
}

SCENARIO("Testing pop_back function")
{
    GIVEN("A non-empty array")
    {
        const size_t arraySize = 5;

        DynamicArray<int> testArray(arraySize);
        TestDynamicArray::init(testArray, arraySize);

        WHEN("An element is popped")
        {
            testArray.pop_back();

            THEN("The size should decrease with 1")
            {
                REQUIRE(testArray.size() == arraySize - 1);
            }

            THEN("The capacity shouldn't change")
            {
                REQUIRE(testArray.capacity() == arraySize);
            }

            THEN("The popped element shouldn't be accessible")
            {
                REQUIRE_THROWS_AS(testArray.at(4), std::out_of_range);
            }

            THEN("The other elements should be valid")
            {
                CHECK(TestDynamicArray::hasValidElements(testArray, arraySize - 1));
            }
        }
    }

    GIVEN("An empty array")
    {
        const size_t arraySize = 0;
        DynamicArray<int> testArray;

        WHEN("An element is popped")
        {
            testArray.pop_back();

            THEN("The size should remain 0")
            {
                REQUIRE(testArray.size() == arraySize);
            }

            THEN("The capacity should remain 0")
            {
                REQUIRE(testArray.capacity() == arraySize);
            }

            THEN("The array should remain empty")
            {
                CHECK(testArray.empty());
            }
        }
    }
}

SCENARIO("Testing at function")
{
    GIVEN("A non-empty array")
    {
        const size_t arraySize = 10;

        DynamicArray<int> testArray(arraySize);
        TestDynamicArray::init(testArray, 5);

        WHEN("An element with valid index is accessed")
        {
            THEN("No exception should be thrown")
            {
                REQUIRE_NOTHROW(testArray.at(2));
            }
        }

        WHEN("An element with valid index is overwritten")
        {
            testArray[2] = -2;

            THEN("The new elememt should be valid")
            {
                REQUIRE(testArray[2] == -2);
            }

            THEN("The other elements should be valid")
            {
                for(size_t i = 0; i < 5; ++i)
                {
                    if(i != 2 && testArray[i] != i)
                        CHECK(false);
                }

                CHECK(true);
            }
        }

        WHEN("An element with invalid index is accessed")
        {
            THEN("An exception should be thrown")
            {
                REQUIRE_THROWS_AS(testArray.at(7), std::out_of_range);
            }
        }
    }

    GIVEN("A constant non-empty array")
    {
        DynamicArray<int> copy(10);

        TestDynamicArray::init(copy, 5);

        const DynamicArray<int> testArray(copy);

        WHEN("An element with valid index is accessed")
        {
            THEN("No exception should be thrown")
            {
                REQUIRE_NOTHROW(testArray.at(2));
            }

            int elem = testArray.at(2);

            THEN("The array shouldn't change")
            {
                CHECK(TestDynamicArray::isValidArray(testArray, 10, 5));
            }
        }

        WHEN("An element with invalid index is accessed")
        {
            THEN("An exception should be thrown")
            {
                REQUIRE_THROWS_AS(testArray.at(7), std::out_of_range);
            }
        }
    }
}

SCENARIO("Testing operator[] on a dynamic array")
{
    GIVEN("A non-empty array")
    {
        DynamicArray<int> testArray(10);;

        TestDynamicArray::init(testArray, 5);

        WHEN("An element with valid index is accessed")
        {
            int elem = testArray[2];

            THEN("The programm should not terminate")
            {
                CHECK(true);
            }

            THEN("The array shouldn't change")
            {
                CHECK(TestDynamicArray::isValidArray(testArray, 10, 5));
            }

        }
    }
}

SCENARIO("Testing front and back function")
{
    GIVEN("A non-empty array")
    {
        DynamicArray<int> testArray(10);
        TestDynamicArray::init(testArray, 5);

        const DynamicArray<int> testConstArray(testArray);

        WHEN("The front element is accessed")
        {
            THEN("No exception should be thrown")
            {
                REQUIRE_NOTHROW(testArray.front());
                REQUIRE_NOTHROW(testConstArray.front());
                
            }

            THEN("It should be valid")
            {
                REQUIRE(testArray.front() == testArray[0]);
                REQUIRE(testConstArray.front() == testConstArray[0]);
            }
        }

        WHEN("The last element is accessed")
        {
            THEN("No exception should be thrown")
            {
                REQUIRE_NOTHROW(testArray.back());
                REQUIRE_NOTHROW(testConstArray.back());
                
            }

            THEN("It should be valid")
            {
                REQUIRE(testArray.back() == testArray[4]);
                REQUIRE(testConstArray.back() == testConstArray[4]);
            }
        }
    }

    GIVEN("An empty array")
    {
        DynamicArray<int> emptyArray;

        const DynamicArray<int> emptyConstArray;

        WHEN("The front element is accessed")
        {
            THEN("An exception should be thrown")
            {
                REQUIRE_THROWS_AS(emptyArray.front(), std::out_of_range);
                REQUIRE_THROWS_AS(emptyConstArray.front(), std::out_of_range);
            }
        }

        WHEN("The last element is accessed")
        {
            THEN("An exception should be thrown")
            {
                REQUIRE_THROWS_AS(emptyArray.back(), std::out_of_range);
                REQUIRE_THROWS_AS(emptyConstArray.back(), std::out_of_range);
            }
        }
    }
}

SCENARIO("Testing size and capacity function")
{
    GIVEN("An empty array")
    {
        DynamicArray<int> emptyArray;

        THEN("Size and capacity should be equal to 0")
        {
            REQUIRE(emptyArray.size() == 0);
            REQUIRE(emptyArray.capacity() == 0);
        }

        WHEN("An element is pushed")
        {
            emptyArray.push_back(10);

            THEN("Size and capacity should be increased")
            {
                REQUIRE(emptyArray.size() == 1);
                REQUIRE(emptyArray.capacity() > 0);
            }
        }
    }

    GIVEN("A non-empty array")
    {
        DynamicArray<int> testArray(10);
        TestDynamicArray::init(testArray, 5);

        const DynamicArray<int> testConstArray(testArray);

        THEN("Size and capacity should be valid")
        {
            REQUIRE(testArray.size() == 5);
            REQUIRE(testArray.capacity() == 10);
        }

        WHEN("An element is pushed")
        {
            testArray.push_back(10);

            THEN("Size should be increased and capacity shouldn't change")
            {
                REQUIRE(testArray.size() == 6);
                REQUIRE(testArray.capacity() == 10);
            }
        }

        WHEN("An element is popped")
        {
            testArray.pop_back();

            THEN("Only size should change")
            {
                REQUIRE(testArray.size() == 4);
                REQUIRE(testArray.capacity() == 10);
            }
        }
    }
}

SCENARIO("Testing empty function")
{
    GIVEN("An empty array")
    {
        DynamicArray<int> emptyArray;

        THEN("Empty should return true")
        {
            CHECK(emptyArray.empty());
        }
    }

    GIVEN("A non-empty array")
    {
        DynamicArray<int> testArray(10);

        TestDynamicArray::init(testArray, 5);

        THEN("Empty should return false")
        {
            CHECK_FALSE(testArray.empty());
        }
    }
}

SCENARIO("Testing clear function")
{
    GIVEN("An empty array")
    {
        DynamicArray<int> emptyArray;

        WHEN("The content of the object is erased")
        {
            emptyArray.clear();
            
            THEN("The object should remain empty")
            {
                CHECK(emptyArray.empty());
            }
        }
    }

    GIVEN("A non-empty array")
    {
        DynamicArray<int> testArray(10);

        TestDynamicArray::init(testArray, 5);

        WHEN("The content of the object is erased")
        {
            testArray.clear();

            THEN("The object should be empty")
            {
                CHECK(testArray.empty());
            }
        }
    }
}

SCENARIO("Testing resize fucntion")
{
    GIVEN("An array")
    {
        size_t arrayCapacity = 10;

        DynamicArray<int> testArray(arrayCapacity);

        TestDynamicArray::init(testArray, 5);

        DynamicArray<int> copy(testArray);

        WHEN("The array is resized with the same value as its capacity")
        {
            testArray.resize(arrayCapacity, 0);

            THEN("Capaicty shouldn't change")
            {
                REQUIRE(testArray.capacity() == arrayCapacity);
            }

            THEN("Size should be equal to capacity")
            {
                REQUIRE(testArray.size() == testArray.capacity());
            }

            THEN("The elements added before resizing the array shouldn't change")
            {
                CHECK(TestDynamicArray::hasValidElements(testArray, 5));
            }

            THEN("All new elements should be accesiible")
            {
                for(size_t i = 0; i < arrayCapacity; ++i)
                {
                    try
                    {
                        testArray.at(i);
                    }
                    catch(std::out_of_range)
                    {
                        CHECK(false);
                    }                
                }

                CHECK(true);
            }

            THEN("All new elements should be equal to the specified value")
            {
                for(size_t i = 5; i < arrayCapacity; ++i)
                {
                    if(testArray[i] != 0)
                        CHECK(false);               
                }

                CHECK(true);
            }
        }
        
        WHEN("The array is resized with a smaller value than its capacity")
        {
            copy.resize(8, 0);
            testArray.resize(3, 0);
            
            THEN("Capacity should change")
            {
                REQUIRE(testArray.capacity() == 3);
                REQUIRE(copy.capacity() == 8);
            }

            THEN("Size should be equal to capacity")
            {
                REQUIRE(testArray.size() == testArray.capacity());
                REQUIRE(copy.size() == copy.capacity());
            }
            WHEN("The size of the array is smaller than the new capacity")
            {
                THEN("All new elements should be equal to the specified value")
                {
                    for(size_t i = 5; i < 8; ++i)
                    {
                        if(copy[i] != 0)
                            CHECK(false);               
                    }

                    CHECK(true);
                }
            }

            THEN("All elements added before the resizing shouldn't be changed")
            {
                CHECK(TestDynamicArray::hasValidElements(testArray, 3));
                CHECK(TestDynamicArray::hasValidElements(copy, 5));
            }

        }

        WHEN("The array is resized with a bigger or smaller value than its doubled capacity")
        {

            testArray.resize(15);
            copy.resize(30);

            WHEN("The value is smaller than the doubled capacity")
            {
                THEN("Capacity should be doubled")
                {
                    REQUIRE(testArray.capacity() == 20);
                }
            }

            WHEN("The value is bigger than the doubled capacity")
            {
                THEN("Capacity should be equal to the specified value")
                {
                    REQUIRE(copy.capacity() == 30);
                }
            }

            THEN("Size should be equal to capacity")
            {
                REQUIRE(testArray.size() == testArray.capacity());
                REQUIRE(copy.size() == copy.capacity());
            }

            THEN("The elements added before resizing the array shouldn't change")
            {
                CHECK(TestDynamicArray::hasValidElements(testArray, 5));
                CHECK(TestDynamicArray::hasValidElements(copy, 5));
            }

            THEN("All new elements should be accesiible")
            {
                for(size_t i = 5; i < 20; ++i)
                {
                    try
                    {
                        testArray.at(i);
                    }
                    catch(std::out_of_range)
                    {
                        CHECK(false);
                    }                
                }

                for(size_t i = 5; i < 30; ++i)
                {
                    try
                    {
                        copy.at(i);
                    }
                    catch(std::out_of_range)
                    {
                        CHECK(false);
                    }                
                }

                CHECK(true);
            }

            THEN("All new elements should be equal to the specified value")
            {
                for(size_t i = 5; i < 20; ++i)
                {
                    if(testArray[i] != 0)
                        CHECK(false);               
                }

                for(size_t i = 5; i < 30; ++i)
                {
                    if(copy[i] != 0)
                        CHECK(false);               
                }

                CHECK(true);
            }
        }
    }
}

SCENARIO("Testing reserve function")
{
    GIVEN("An array")
    {
        size_t arrayCapacity = 10;

        DynamicArray<int> testArray(arrayCapacity);

        TestDynamicArray::init(testArray, 5);

        DynamicArray<int> copy(testArray);
        WHEN("The same capacity is reserved")
        {
            testArray.reserve(arrayCapacity);

            THEN("Capacity shouldn't change")
            {
                REQUIRE(testArray.capacity() == arrayCapacity);
            }

            THEN("Size shouldn't change")
            {
                REQUIRE(testArray.size() == 5);
            }

            THEN("The elements added before resizing the array shouldn't change")
            {
                CHECK(TestDynamicArray::hasValidElements(testArray, 5));
            }
        }

        WHEN("Less capacity is reserved")
        {
            copy.reserve(8);
            testArray.reserve(3);
            
            THEN("Capaicty should change")
            {
                REQUIRE(testArray.capacity() == 3);
                REQUIRE(copy.capacity() == 8);
            }

            WHEN("The specified value is smaller than the array's size")
            {
                THEN("Size should be equal to capacity")
                {
                    REQUIRE(testArray.size() == testArray.capacity());
                }
            }

            WHEN("The specified value is bigger than the array's size")
            {
                THEN("Size shouldn't change")
                {
                    REQUIRE(copy.size() == 5);
                }
            }

            THEN("All elements added before the resizing shouldn't be changed")
            {
                CHECK(TestDynamicArray::hasValidElements(testArray, 3));
                CHECK(TestDynamicArray::hasValidElements(copy, 5));
            }
        }

        WHEN("More capacity is reserved")
        {
            testArray.reserve(15);
            copy.reserve(30);

            WHEN("The specified value is smaller than the doubled capacity")
            {
                THEN("Capacity should be doubled")
                {
                    REQUIRE(testArray.capacity() == 20);
                }
            }

            WHEN("The specified value is bigger than the doubled capacity")
            {
                THEN("Capacity should be equal to the specified value")
                {
                    REQUIRE(copy.capacity() == 30);
                }
            }

            THEN("Size shouldn't change")
            {
                REQUIRE(testArray.size() == 5);
                REQUIRE(copy.size() == 5);
            }

            THEN("The elements added before reserving the capacity shouldn't change")
            {
                CHECK(TestDynamicArray::hasValidElements(testArray, 5));
                CHECK(TestDynamicArray::hasValidElements(copy, 5));
            }
        }
    }
}
