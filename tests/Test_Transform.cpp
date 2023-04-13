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

#include "Mighter2d/common/Transform.h"
#include <doctest.h>

TEST_CASE("mighter2d::Transform class")
{
    SUBCASE("Constructors")
    {
        SUBCASE("Default constructor")
        {
            mighter2d::Transform transform;

            CHECK_EQ(transform.getPosition().x, 0.0f);
            CHECK_EQ(transform.getPosition().y, 0.0f);

            CHECK_EQ(transform.getOrigin().x, 0.0f);
            CHECK_EQ(transform.getOrigin().y, 0.0f);

            CHECK_EQ(transform.getScale().x, 1.0f);
            CHECK_EQ(transform.getScale().y, 1.0f);

            CHECK_EQ(transform.getRotation(), 0.0f);
        }
    }

    SUBCASE("Setters and getters")
    {
        SUBCASE("setPosition(float, float)")
        {
            mighter2d::Transform transform;
            transform.setPosition(2.0f, 3.0f);

            CHECK_EQ(transform.getPosition().x, 2.0f);
            CHECK_EQ(transform.getPosition().y, 3.0f);
        }

        SUBCASE("setPosition(Vector2f)")
        {
            mighter2d::Transform transform;
            transform.setPosition(mighter2d::Vector2f(2.0f, 3.0f));

            CHECK_EQ(transform.getPosition().x, 2.0f);
            CHECK_EQ(transform.getPosition().y, 3.0f);
        }

        SUBCASE("setScale(float, float)")
        {
            mighter2d::Transform transform;
            transform.setScale(2.0f, 3.0f);

            CHECK_EQ(transform.getScale().x, 2.0f);
            CHECK_EQ(transform.getScale().y, 3.0f);
        }

        SUBCASE("setScale(Vector2f)")
        {
            mighter2d::Transform transform;
            transform.setScale(mighter2d::Vector2f(5.0f, 10.0f));

            CHECK_EQ(transform.getScale().x, 5.0f);
            CHECK_EQ(transform.getScale().y, 10.0f);
        }

        SUBCASE("setOrigin(float, float)")
        {
            mighter2d::Transform transform;
            transform.setOrigin(2.0f, 3.0f);

            CHECK_EQ(transform.getOrigin().x, 2.0f);
            CHECK_EQ(transform.getOrigin().y, 3.0f);
        }

        SUBCASE("setOrigin(Vector2f)")
        {
            mighter2d::Transform transform;
            transform.setOrigin(mighter2d::Vector2f(5.0f, 10.0f));

            CHECK_EQ(transform.getOrigin().x, 5.0f);
            CHECK_EQ(transform.getOrigin().y, 10.0f);
        }

        SUBCASE("setRotation()")
        {
            mighter2d::Transform transform;
            transform.setRotation(60.0f);

            CHECK_EQ(transform.getRotation(), 60.0f);
        }
    }

    SUBCASE("scale(float, float)")
    {
        mighter2d::Transform transform;
        transform.setScale(2.0f, 3.0f);

        REQUIRE_EQ(transform.getScale().x, 2.0f);
        REQUIRE_EQ(transform.getScale().y, 3.0f);
        transform.scale(3.0f, 4.0f);

        CHECK_EQ(transform.getScale().x, 6.0f);
        CHECK_EQ(transform.getScale().y, 12.0f);
    }

    SUBCASE("scale(Vector2f)")
    {
        mighter2d::Transform transform;
        transform.setScale(2.0f, 3.0f);

        REQUIRE_EQ(transform.getScale().x, 2.0f);
        REQUIRE_EQ(transform.getScale().y, 3.0f);
        transform.scale(mighter2d::Vector2f(3.0f, 4.0f));

        CHECK_EQ(transform.getScale().x, 6.0f);
        CHECK_EQ(transform.getScale().y, 12.0f);
    }

    SUBCASE("rotate()")
    {
        mighter2d::Transform transform;
        transform.setRotation(60.0f);

        REQUIRE_EQ(transform.getRotation(), 60.0f);
        transform.rotate(30.0f);
        CHECK_EQ(transform.getRotation(), 90.0f);
    }

    SUBCASE("move(float, float)")
    {
        mighter2d::Transform transform;
        transform.setPosition(5.0f, 10.0f);

        CHECK_EQ(transform.getPosition().x, 5.0f);
        CHECK_EQ(transform.getPosition().y, 10.0f);

        transform.move(20.0f, -50.0f);

        CHECK_EQ(transform.getPosition().x, 25.0f);
        CHECK_EQ(transform.getPosition().y, -40.0f);
    }

    SUBCASE("move(Vector2f)")
    {
        mighter2d::Transform transform;
        transform.setPosition(5.0f, 10.0f);

        CHECK_EQ(transform.getPosition().x, 5.0f);
        CHECK_EQ(transform.getPosition().y, 10.0f);

        transform.move(mighter2d::Vector2f(20.0f, -50.0f));

        CHECK_EQ(transform.getPosition().x, 25.0f);
        CHECK_EQ(transform.getPosition().y, -40.0f);
    }

    SUBCASE("onPropertyChange()")
    {
        mighter2d::Transform transform;

        bool isInvoked = false;
        transform.onPropertyChange([&isInvoked](const mighter2d::Property&) {
            isInvoked = true;
        });

        transform.setPosition(1.0f, 2.0f);
        CHECK(isInvoked);

        isInvoked = false;
        transform.setScale(1.0f, 2.0f);
        CHECK(isInvoked);

        isInvoked = false;
        transform.setOrigin(1.0f, 2.0f);
        CHECK(isInvoked);

        isInvoked = false;
        transform.setRotation(7.0f);
        CHECK(isInvoked);

        isInvoked = false;
        transform.rotate(3.0f);
        CHECK(isInvoked);

        isInvoked = false;
        transform.scale(1.0f, 2.0f);
        CHECK(isInvoked);

        isInvoked = false;
        transform.move(1.0f, 2.0f);
        CHECK(isInvoked);
    }
}