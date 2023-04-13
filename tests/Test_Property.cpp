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

#include "Mighter2d/common/Property.h"
#include "Mighter2d/core/exceptions/Exceptions.h"
#include <doctest.h>
#include <string>

TEST_CASE("mighter2d::Property class")
{
    SUBCASE("Constructors")
    {
        SUBCASE("(name) constructor")
        {
            mighter2d::Property property("WINDOW_TITLE");
            CHECK_EQ(property.getName(), "WINDOW_TITLE");
            CHECK_FALSE(property.hasValue());
        }

        SUBCASE("(name, value) constructor")
        {
            mighter2d::Property property("WINDOW_SIZE", 800);
            CHECK_EQ(property.getName(), "WINDOW_SIZE");
            CHECK(property.hasValue());
        }
    }

    SUBCASE("Setters and getters")
    {
        SUBCASE("setValue()")
        {
            mighter2d::Property property("");

            property.setValue(100);
            CHECK_EQ(property.getValue<int>(), 100);

            property.setValue(50u);
            CHECK_EQ(property.getValue<unsigned int>(), 50u);

            property.setValue(3.0);
            CHECK_EQ(property.getValue<double>(), 3.0);

            property.setValue(10.0f);
            CHECK_EQ(property.getValue<float>(), 10.0f);

            property.setValue(true);
            CHECK_EQ(property.getValue<bool>(), true);

            property.setValue(std::string("Mighter2d"));
            CHECK_EQ(property.getValue<std::string>(), std::string("Mighter2d"));

            SUBCASE("getValue() throws mighter2d::InvalidArgument when the template argument does not match the contained type")
            {
                property.setValue(25);
                CHECK_THROWS_AS(property.getValue<bool>(), mighter2d::InvalidArgumentException);
            }
        }

        SUBCASE("onValueChange()")
        {
            mighter2d::Property property("");
            int value = -1;
            bool isInvoked = false;

            property.onValueChange([&value, &isInvoked](mighter2d::Property* p) {
                isInvoked = true;
                value = p->getValue<int>();
            });

            property.setValue(30);
            CHECK(isInvoked);
            CHECK_EQ(value, 30);
        }

        SUBCASE("unsubscribe()")
        {
            SUBCASE("Removing a non-existent event listener returns false")
            {
                mighter2d::Property property("");
                CHECK_FALSE(property.unsubscribe(-1));
            }

            SUBCASE("Removing an existing event listener returns true")
            {
                mighter2d::Property property("");

                bool isInvoked = false;
                int valueChangeListener = property.onValueChange([&isInvoked](mighter2d::Property*) {
                    isInvoked = true;
                });

                CHECK(property.unsubscribe(valueChangeListener));

                SUBCASE("A removed event listener is not invoked when the value change event is raised")
                {
                    property.setValue(10);
                    CHECK_FALSE(isInvoked);
                }
            }
        }
    }
}
