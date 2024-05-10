#include <gtest/gtest.h>

#include "ArrayPrimitive.hpp"
#include "CompoundCTS.hpp"
#include "CompoundRTS.hpp"
#include "IntDouble.hpp"
#include "IntDoubleLong.hpp"
#include "Picture.hpp"
#include "Point.hpp"
#include "Rectangle.hpp"
#include "String.hpp"
#include "StringIntLong.hpp"



int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    int res = RUN_ALL_TESTS();
    std::cout << "TOTAL_ALLOCATIONS: " << stats::total_allocations << std::endl;
    std::cout << "TOTAL_BYTES_ALLOCATED: " << stats::total_bytes_allocated << std::endl;
    return res;
}
