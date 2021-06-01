#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../Buffer.hpp"

#include <algorithm>

class TestBuffer
{
public:

    template <class Type>
    static bool containSameElements(const Buffer<Type>& lhs, const Buffer<Type>& rhs, size_t copiedElementsCount)
    {
        for(size_t i = 0; i < copiedElementsCount; ++i)
        {
            if(lhs[i] != rhs[i])
                return false;
        }
        return true;
    }

    template <class Type>
    static bool areEqual(const Buffer<Type>& lhs, const Buffer<Type>& rhs)
    {
        return lhs.size() == rhs.size() &&
               containSameElements(lhs, rhs, lhs.size());
    }

    static void init(Buffer<int>& buffer, size_t size)
    {
        for(size_t i = 0; i < size; ++i)
        {
            buffer[i] = i;
        }
        
    }

    static bool isValidBuffer(const Buffer<int>& buffer, size_t size)
    {
        return isValidBuffer(buffer, size, size);
    }

    static bool isValidBuffer(const Buffer<int>& buffer, size_t size, size_t used)
    {
        if(buffer.size() != size)
            return false;

        for(size_t i = 0; i < used; ++i)
        {
            if(buffer[i] != i)
                return false;
        }

        return true;
    }
};


SCENARIO("Testing default constructor")
{
    GIVEN("An empty bufer")
    {
        Buffer<int> testBuffer;

        THEN("Size should be 0")
        {
            REQUIRE(testBuffer.size() == 0);
        }

        THEN("Clear should not change the empty buffer")
        {
            testBuffer.clear();
            CHECK(TestBuffer::areEqual(testBuffer, Buffer<int>()));
        }    
    }
}

SCENARIO("Testing Buffer constructor with an argument that specifies the size")
{
    WHEN("The specified size is a positive number")
    {
        Buffer<int> testBuffer(12);

        THEN("The size of the buffer should be valid")
        {
            REQUIRE(testBuffer.size() == 12);
        }

        THEN("Clear should erase the allocated data")
        {
            testBuffer.clear();
            REQUIRE(testBuffer.size() == 0);
        }
    }

    WHEN("The specified size is less than zero")
    {
        THEN("An exception should ne thrown")
        {
            REQUIRE_THROWS_AS(Buffer<int>(-5), std::bad_array_new_length);
        }
    }

    WHEN("The specified size is zero")
    {
        Buffer<int> testBuffer(0);

        THEN("Size should return 0")
        {
            REQUIRE(testBuffer.size() == 0);
        }
    }
}

SCENARIO("Testing Buffer constructor with tho arguments that specify the size and the buffer, from which to copy elements")
{
    GIVEN("A non-empty buffer")
    {
        const size_t testBuffersize = 7;
        Buffer<int> testBuffer(testBuffersize);

        TestBuffer::init(testBuffer, testBuffersize);

        WHEN("A buffer is constructed")
        {    
            const size_t copyBufferSize1 = 10;
            Buffer<int> testCopyBuffer1(copyBufferSize1, testBuffer);

            const size_t copyBufferSize2 = 5;
            Buffer<int> testCopyBuffer2(copyBufferSize2, testBuffer);
            
            THEN("The content of the given buffer shoudln't change")
            {
                CHECK(TestBuffer::isValidBuffer(testBuffer, testBuffersize));
            }
            

            THEN("The size of the new buffer should be valid")
            {
                REQUIRE(testCopyBuffer1.size() == copyBufferSize1);
                REQUIRE(testCopyBuffer2.size() == copyBufferSize2);
            }
            
            THEN("The elements of the new buffer should be equal to their corresponding ones in the given buffer")
            {
                CHECK(TestBuffer::containSameElements(testBuffer, testCopyBuffer1, testBuffersize));
                CHECK(TestBuffer::containSameElements(testBuffer, testCopyBuffer2, copyBufferSize2));
            } 

            WHEN("The elements of the given buffer are erased")
            {
                testBuffer.clear();
                THEN("The new buffer should stay valid")
                {
                    CHECK(TestBuffer::isValidBuffer(testCopyBuffer1, copyBufferSize1, testBuffersize));
                    CHECK(TestBuffer::isValidBuffer(testCopyBuffer2, copyBufferSize2));
                }
            }
        }  

        WHEN("A new buffer with size 0 is constructed")
        {
            Buffer<int> copyBuffer(0, testBuffer);

            THEN("The size of the new buffer should be valid")
            {
                REQUIRE(copyBuffer.size() == 0);
            }
        }       
    }
}

SCENARIO("Testing Buffer constructor with three arguments - size of the new buffer, count of elements to copy and buffer, from which to copy")
{
    GIVEN("A non-empty bufer")
    {
        const size_t testBufferSize = 7;
        Buffer<int> testBuffer(testBufferSize);

        TestBuffer::init(testBuffer, testBufferSize);

        WHEN("A buffer with size 0 is constructed")
        {
            THEN("No exception should be thrown")
            {
                REQUIRE_NOTHROW(Buffer<int>(0, 5, testBuffer));
            }

            Buffer<int> copyArray(0, 5, testBuffer);

            THEN("The size of new buffer should be valid")
            {
                REQUIRE(copyArray.size() == 0);
            }
        }

        WHEN("The given buffer doesn't contain enough elements")
        {
            THEN("An exception should be thrown")
            {
                REQUIRE_THROWS_AS(Buffer<int>(10, 8, testBuffer), std::invalid_argument);
                REQUIRE_THROWS_AS(Buffer<int>(8, 10, testBuffer), std::invalid_argument);
            }
        }

        WHEN("The given buffer contains enough elements")
        {

            WHEN("The size of the new one is large enough to copy the required elements")
            {
                const size_t copyBufferSize = 10;

                Buffer<int> testCopyBuffer1(copyBufferSize, 7, testBuffer);
                Buffer<int> testCopyBuffer2(copyBufferSize, 5, testBuffer);

                THEN("The size of the new buffer should be valid ")
                {
                    REQUIRE(testCopyBuffer1.size() == copyBufferSize); 
                    REQUIRE(testCopyBuffer2.size() == copyBufferSize);
                }
            
                THEN("The content of the given buffer should't change")
                {
                    CHECK(TestBuffer::isValidBuffer(testBuffer, testBufferSize));
                }

                THEN("The elements of the new buffer should be equal to their corresponding ones in the given buffer")
                {
                    CHECK(TestBuffer::containSameElements(testBuffer, testCopyBuffer1, 7));
                    CHECK(TestBuffer::containSameElements(testBuffer, testCopyBuffer1, 5));
                }

                THEN("If the old buffer content is erased then the new buffer should be valid")
                {
                    testBuffer.clear();
                    CHECK(TestBuffer::isValidBuffer(testCopyBuffer1, copyBufferSize, 7));
                    CHECK(TestBuffer::isValidBuffer(testCopyBuffer2, copyBufferSize, 5));
                }
            }

            WHEN("The size of the new buffer is less than the count of the elements to copy")
            {
                const size_t copyBufferSize = 3;
            
                Buffer<int> testCopyBuffer1(copyBufferSize, 7, testBuffer);
                Buffer<int> testCopyBuffer2(copyBufferSize, 10, testBuffer);

                THEN("The size of the new byffer should be valid")
                {
                    REQUIRE(testCopyBuffer1.size() == copyBufferSize);
                    REQUIRE(testCopyBuffer2.size() == copyBufferSize);
                }

                THEN("The content of the given buffer should't change")
                {
                    CHECK(TestBuffer::isValidBuffer(testBuffer, testBufferSize));
                }

                THEN("The elements of the new buffer should be equal to their corresponding ones in the given buffer")
                {
                    CHECK(TestBuffer::containSameElements(testBuffer, testCopyBuffer1, copyBufferSize));
                    CHECK(TestBuffer::containSameElements(testBuffer, testCopyBuffer1, copyBufferSize));
                }

                THEN("If the elements of the given buffer are aresed than the new buffer should be valid")
                {
                    testBuffer.clear();
                    CHECK(TestBuffer::isValidBuffer(testCopyBuffer1, copyBufferSize));
                    CHECK(TestBuffer::isValidBuffer(testCopyBuffer1, copyBufferSize));
                }
            }
        } 
    }
}
        
SCENARIO("Testing size function")
{
    GIVEN("An empty buffer object")
    {
        Buffer<int> testBuffer;
        
        THEN("size should return zero")
        {
            REQUIRE(testBuffer.size() == 0);
        }
    }

    GIVEN("An object with a specific size")
    {
        int elems = GENERATE(1, 3, 15);

        Buffer<int> testBuffer(elems);

        THEN("size should return the count of allocated elements")
        {
            REQUIRE(testBuffer.size() == elems);
        }
    }
}

SCENARIO("Testing swap function")
{
    GIVEN("A non-empty buffer")
    {
        const size_t testBufferSize1 = 12; 

        Buffer<int> testBuffer1(testBufferSize1);

        TestBuffer::init(testBuffer1, testBufferSize1);

        WHEN("The same object is swapped")
        {
            THEN("The buffer should not be changed")
            {
                CHECK(TestBuffer::isValidBuffer(testBuffer1, testBufferSize1));
            }
        }

        GIVEN("Another non-empty buffer")
        {
            const size_t testBufferSize2 = 15; 

            Buffer<int> testBuffer2(testBufferSize2);

            TestBuffer::init(testBuffer2, testBufferSize2);
            
            WHEN("The buffers are swapped")
            {                         
                Buffer<int> copyTest1(testBufferSize1, testBuffer1);
                Buffer<int> copyTest2(testBufferSize2, testBuffer2);

                testBuffer1.swap(testBuffer2);

                THEN("both contents should be change")
                {
                    REQUIRE(TestBuffer::areEqual(testBuffer1, copyTest2));
                    REQUIRE(TestBuffer::areEqual(testBuffer2, copyTest1)); 
                }
            }
        }

        GIVEN("An empty buffer object")
        {
            Buffer<int> testEmpty;

            Buffer<int> copyTest1(testBufferSize1, testBuffer1);

            WHEN("The buffers are swapped")
            {
                testBuffer1.swap(testEmpty);

                THEN("both contents should change")
                {
                    REQUIRE(testBuffer1.size() == 0);
                    REQUIRE(TestBuffer::areEqual(testEmpty, copyTest1)); 
                }
            }
        }
    }
}

SCENARIO("Testing clear funcion")
{
    GIVEN("A buffer object with a specific size")
    {
        Buffer<int> testBuffer(15);

        TestBuffer::init(testBuffer, 15);

        THEN("size should be 0")
        {
            testBuffer.clear();
            REQUIRE(testBuffer.size() == 0);      
        }
    }

    GIVEN("An empty buffer object")
    {
        Buffer<int> testBuffer;

        THEN("clear should not throw an exception")
        {
            REQUIRE_NOTHROW(testBuffer.clear());
        }
    }
}

SCENARIO("Testing operator[] on a buffer ")
{
    GIVEN("A non-empty buffer")
    {
        const size_t tesBufferSize = 15;
        Buffer<int> testBuffer(tesBufferSize);

        TestBuffer::init(testBuffer, tesBufferSize);

        WHEN("An element is overwritten")
        {
            testBuffer[5] = 0;

            THEN("The element should be changed")
            {
                REQUIRE(testBuffer[5] == 0);
            }

            THEN("Other elements should stay same")
            {
                bool areEqual = true;
                for(size_t i = 0; i < tesBufferSize; ++i)
                {
                    if(i != 5 && testBuffer[i] != i ){
                        areEqual = false;
                        break;
                    }
                }

                CHECK(areEqual);
            }
        }

        GIVEN("A non-empty constant buffer")
        {
            const Buffer<int> testConstBuff(15, testBuffer);

            WHEN("An element is extracted")
            {
                const int elem = testConstBuff[5];

                THEN("The buffer shouldn't be changed")
                {
                    CHECK(TestBuffer::isValidBuffer(testConstBuff, 15));
                }
            }
        }
    }
}


