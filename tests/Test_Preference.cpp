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

#include "Mighter2d/common/Preference.h"
#include <doctest.h>

TEST_CASE("mighter2d::Preference class")
{
    SUBCASE("Constructors")
    {
        SUBCASE("(key, type) constructor")
        {
            mighter2d::Preference preference("VOLUME", mighter2d::PrefType::Float);

            CHECK_EQ(preference.getKey(), "VOLUME");
            CHECK_EQ(preference.getType(), mighter2d::PrefType::Float);
            CHECK_EQ(preference.getDescription(), "");
            CHECK_FALSE(preference.hasValue());
        }

        SUBCASE("(key, type, value) constructor")
        {
            mighter2d::Preference preference("NAME", mighter2d::PrefType::String, std::string("Mighter2d"));

            CHECK_EQ(preference.getKey(), "NAME");
            CHECK_EQ(preference.getType(), mighter2d::PrefType::String);
            CHECK_EQ(preference.getDescription(), "");
            CHECK(preference.hasValue());
            CHECK_EQ(preference.getValue<std::string>(), std::string("Mighter2d"));
        }

        SUBCASE("(key, type, value, description) constructor")
        {
            mighter2d::Preference preference("FULL_SCREEN", mighter2d::PrefType::Bool, true, "Game window full screen option");

            CHECK_EQ(preference.getKey(), "FULL_SCREEN");
            CHECK_EQ(preference.getType(), mighter2d::PrefType::Bool);
            CHECK_EQ(preference.getDescription(), "Game window full screen option");
            CHECK(preference.hasValue());
            CHECK_EQ(preference.getValue<bool>(), true);
        }
    }

    SUBCASE("Setters and getters")
    {
        SUBCASE("setValue()")
        {
            mighter2d::Preference preference("VOLUME", mighter2d::PrefType::Float);
            preference.setValue(10.0f);

            CHECK_EQ(preference.getValue<float>(), 10.0f);

            SUBCASE("getValue() throws mighter2d::InvalidArgument when the template argument does not match the contained type")
            {
                CHECK_THROWS_AS(preference.getValue<bool>(), mighter2d::InvalidArgumentException);
            }
        }

        SUBCASE("setDescription()")
        {
            mighter2d::Preference preference("VOLUME", mighter2d::PrefType::Float);
            preference.setDescription("The master volume");

            CHECK_EQ(preference.getDescription(), "The master volume");
        }
    }
}