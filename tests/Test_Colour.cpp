////////////////////////////////////////////////////////////////////////////////
// Mighter2d
//
// Copyright (c) 2023 Kwena Mashamaite
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

#include "Mighter2d/graphics/Colour.h"
#include <doctest.h>

TEST_CASE("mighter2d::Colour class")
{
    SUBCASE("Constructors")
    {
        SUBCASE("Default constructor")
        {
            mighter2d::Colour colour;
            CHECK_EQ(colour.red, 0);
            CHECK_EQ(colour.green, 0);
            CHECK_EQ(colour.blue, 0);
            CHECK_EQ(colour.opacity, 255);
        }

        SUBCASE("Construct from RGB components")
        {
            mighter2d::Colour colour{5, 10, 15};
            CHECK_EQ(colour.red, 5);
            CHECK_EQ(colour.green, 10);
            CHECK_EQ(colour.blue, 15);
            CHECK_EQ(colour.opacity, 255);
        }

        SUBCASE("Construct from RGBA components")
        {
            mighter2d::Colour colour{10, 20, 30, 40};
            CHECK_EQ(colour.red, 10);
            CHECK_EQ(colour.green, 20);
            CHECK_EQ(colour.blue, 30);
            CHECK_EQ(colour.opacity, 40);
        }

        SUBCASE("Construct from #RRGGBB hex code")
        {
            mighter2d::Colour colour{"#000000"};
            CHECK_EQ(colour.red, 0);
            CHECK_EQ(colour.green, 0);
            CHECK_EQ(colour.blue, 0);
            CHECK_EQ(colour.opacity, 255);
        }

        SUBCASE("Construct from #RRGGBBAA hex code")
        {
            mighter2d::Colour colour{"#00FF00C8"};
            CHECK_EQ(colour.red, 0);
            CHECK_EQ(colour.green, 255);
            CHECK_EQ(colour.blue, 0);
            CHECK_EQ(colour.opacity, 200);
        }
    }

    SUBCASE("Operators")
    {
        SUBCASE("operator+")
        {
            mighter2d::Colour c1(1, 1, 1, 1);
            mighter2d::Colour c2(1, 2, 3, 4);
            CHECK_EQ(c1 + c2, mighter2d::Colour(2, 3, 4, 5));

            SUBCASE("clamped")
            {
                mighter2d::Colour c3(255, 255, 255, 255);
                CHECK_EQ(c3 + c1, mighter2d::Colour(255, 255, 255, 255));
            }
        }

        SUBCASE("operator-")
        {
            mighter2d::Colour c1(10, 20, 30, 40);
            mighter2d::Colour c2(5, 10, 15, 20);
            CHECK_EQ(c1 - c2, mighter2d::Colour(5, 10, 15, 20));

            SUBCASE("clamped")
            {
                CHECK_EQ(c2 - c1, mighter2d::Colour(0, 0, 0, 0));
            }
        }

        SUBCASE("operator*")
        {
            mighter2d::Colour c1(255, 255, 255, 255);
            mighter2d::Colour c2(2, 4, 6, 8);
            CHECK_EQ(c1 * c2, mighter2d::Colour(2, 4, 6, 8));
        }

        SUBCASE("operator==")
        {
            CHECK(mighter2d::Colour(0, 0, 0) == mighter2d::Colour(0, 0, 0));
        }

        SUBCASE("operator!=")
        {
            CHECK(mighter2d::Colour(0, 0, 0) != mighter2d::Colour(255, 0, 0));
        }

        SUBCASE("operator+=")
        {
            mighter2d::Colour c1(10, 20, 30, 40);
            c1 += mighter2d::Colour(5, 10, 15, 20);
            CHECK_EQ(c1, mighter2d::Colour(15, 30, 45, 60));

            SUBCASE("clamped")
            {
                c1 += mighter2d::Colour(255, 255, 255, 255);
                CHECK_EQ(c1, mighter2d::Colour(255, 255, 255, 255));
            }
        }

        SUBCASE("operator-=")
        {
            mighter2d::Colour c1(10, 20, 30, 40);
            mighter2d::Colour c2(5, 10, 15, 20);

            c1 -= c2;
            CHECK_EQ(c1, mighter2d::Colour(5, 10, 15, 20));

            SUBCASE("clamped")
            {
                c1 -= mighter2d::Colour(255, 255, 255, 255);
                CHECK_EQ(c1, mighter2d::Colour(0, 0, 0, 0));
            }
        }

        SUBCASE("operator*=")
        {
            mighter2d::Colour c1(255, 255, 255, 255);
            c1 *= mighter2d::Colour(2, 4, 6, 8);
            CHECK_EQ(c1, mighter2d::Colour(2, 4, 6, 8));
        }
    }

    SUBCASE("Predefined colours")
    {
        CHECK_EQ(mighter2d::Colour::Black, mighter2d::Colour(0, 0, 0, 255));
        CHECK_EQ(mighter2d::Colour::White, mighter2d::Colour(255, 255, 255, 255));
        CHECK_EQ(mighter2d::Colour::Red, mighter2d::Colour(255, 0, 0, 255));
        CHECK_EQ(mighter2d::Colour::Green, mighter2d::Colour(0, 255, 0, 255));
        CHECK_EQ(mighter2d::Colour::Blue, mighter2d::Colour(0, 0, 255, 255));
        CHECK_EQ(mighter2d::Colour::Yellow, mighter2d::Colour(255, 255, 0, 255));
        CHECK_EQ(mighter2d::Colour::Orange, mighter2d::Colour(255, 165, 0, 255));
        CHECK_EQ(mighter2d::Colour::Brown, mighter2d::Colour(165, 42, 42, 255));
        CHECK_EQ(mighter2d::Colour::Maroon, mighter2d::Colour(128, 0, 0, 255));
        CHECK_EQ(mighter2d::Colour::Indigo, mighter2d::Colour(75, 0, 130, 255));
        CHECK_EQ(mighter2d::Colour::Cyan, mighter2d::Colour(0, 255, 255, 255));
        CHECK_EQ(mighter2d::Colour::Magenta, mighter2d::Colour(255, 0, 255, 255));
        CHECK_EQ(mighter2d::Colour::Purple, mighter2d::Colour(128, 0, 128, 255));
        CHECK_EQ(mighter2d::Colour::Pink, mighter2d::Colour(255, 192, 203, 255));
        CHECK_EQ(mighter2d::Colour::Grey, mighter2d::Colour(128, 128, 128, 255));
        CHECK_EQ(mighter2d::Colour::Violet, mighter2d::Colour(238, 130, 238, 255));
        CHECK_EQ(mighter2d::Colour::Turquoise, mighter2d::Colour(64, 224, 208, 255));
        CHECK_EQ(mighter2d::Colour::Gold, mighter2d::Colour(255, 215, 0, 255));
        CHECK_EQ(mighter2d::Colour::Silver, mighter2d::Colour(192, 192, 192, 255));
        CHECK_EQ(mighter2d::Colour::Transparent, mighter2d::Colour(0, 0, 0, 0));
    }
}