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
    return RUN_ALL_TESTS();
}
