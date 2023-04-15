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

#include "Mighter2d/core/input/Mouse.h"
#include "Mighter2d/core/event/SystemEvent.h"
#include <doctest.h>

using Mouse = mighter2d::input::Mouse;

TEST_CASE("mighter2d::input::Mouse class")
{
    SUBCASE("Constructors")
    {
        SUBCASE("Default constructor")
        {
            Mouse mouse;
            CHECK(mouse.isEnabled());
        }
    }

    SUBCASE("Setters and Getters")
    {
        SUBCASE("setEnable()")
        {
            Mouse mouse;

            mouse.setEnable(false);
            CHECK_FALSE(mouse.isEnabled());

            mouse.setEnable(true);
            CHECK(mouse.isEnabled());
        }

        SUBCASE("setPosition()")
        {
            Mouse mouse;
            Mouse::setPosition(mighter2d::Vector2i(100, 50));

            CHECK_EQ(mouse.getPosition().x, 100);
            CHECK_EQ(mouse.getPosition().y, 50);
        }
    }

    SUBCASE("Callbacks")
    {
        SUBCASE("onButtonUp()")
        {
            Mouse mouse;
            bool isInvoked = false;
            mighter2d::Vector2i mousePos(-1, -1);

            mouse.onButtonUp([&isInvoked, &mousePos](Mouse::Button button, int x, int y) {
                isInvoked = true;
                mousePos.x = x;
                mousePos.y = y;
            });

            mighter2d::Event event;
            event.type = mighter2d::Event::MouseButtonReleased;
            event.mouseButton.x = 14;
            event.mouseButton.y = 88;

            REQUIRE(mouse.isEnabled());
            mouse.handleEvent(event);

            CHECK(isInvoked);
            CHECK_EQ(mousePos.x, 14);
            CHECK_EQ(mousePos.y, 88);
        }

        SUBCASE("onButtonDown()")
        {
            Mouse mouse;
            bool isInvoked = false;
            mighter2d::Vector2i mousePos(-1, -1);

            mouse.onButtonDown([&isInvoked, &mousePos](Mouse::Button button, int x, int y) {
                isInvoked = true;
                mousePos.x = x;
                mousePos.y = y;
            });

            mighter2d::Event event;
            event.type = mighter2d::Event::MouseButtonPressed;
            event.mouseButton.x = 52;
            event.mouseButton.y = 37;

            REQUIRE(mouse.isEnabled());
            mouse.handleEvent(event);

            CHECK(isInvoked);
            CHECK_EQ(mousePos.x, 52);
            CHECK_EQ(mousePos.y, 37);
        }

        SUBCASE("onMouseMove()")
        {
            Mouse mouse;
            bool isInvoked = false;
            mighter2d::Vector2i newPos(-1, -1);

            mouse.onMouseMove([&isInvoked, &newPos](int x, int y) {
                isInvoked = true;
                newPos.x = x;
                newPos.y = y;
            });

            mighter2d::Event event;
            event.type = mighter2d::Event::MouseMoved;
            event.mouseMove.x = 300;
            event.mouseMove.y = 87;

            REQUIRE(mouse.isEnabled());
            mouse.handleEvent(event);

            CHECK(isInvoked);
            CHECK_EQ(newPos.x, 300);
            CHECK_EQ(newPos.y, 87);
        }

        SUBCASE("onWheelScroll()")
        {
            Mouse mouse;
            Mouse::Wheel scrolledWheel;
            bool isInvoked = false;
            float wheelOffset = 0;
            mighter2d::Vector2i mousePos(-1, -1);

            mouse.onWheelScroll([&](Mouse::Wheel wheel, float offset, int x, int y) {
                isInvoked = true;
                scrolledWheel = wheel;
                wheelOffset = offset;
                mousePos.x = x;
                mousePos.y = y;
            });

            mighter2d::Event event;
            event.type = mighter2d::Event::MouseWheelScrolled;
            event.mouseWheelScroll.wheel = Mouse::Wheel::VerticalWheel;
            event.mouseWheelScroll.delta = 0.75f;
            event.mouseWheelScroll.x = 247;
            event.mouseWheelScroll.y = 475;

            REQUIRE(mouse.isEnabled());
            mouse.handleEvent(event);

            CHECK(isInvoked);
            CHECK_EQ(scrolledWheel, Mouse::Wheel::VerticalWheel);
            CHECK_EQ(wheelOffset, 0.75f);
            CHECK_EQ(mousePos.x, 247);
            CHECK_EQ(mousePos.y, 475);
        }
    }

    SUBCASE("unsubscribe()")
    {
        SUBCASE("Removing a non-existent event listener returns false")
        {
            CHECK_FALSE(Mouse().unsubscribe(mighter2d::MouseEvent::MouseUp, -1));
            CHECK_FALSE(Mouse().unsubscribe(mighter2d::MouseEvent::MouseDown, -1));
            CHECK_FALSE(Mouse().unsubscribe(mighter2d::MouseEvent::MouseMove, -1));
            CHECK_FALSE(Mouse().unsubscribe(mighter2d::MouseEvent::MouseWheelScroll, -1));
        }

        SUBCASE("Removing an existing event listener returns true")
        {
            Mouse mouse;

            int buttonUpListener = mouse.onButtonUp([](Mouse::Button) {});
            int buttonDownListener = mouse.onButtonDown([](Mouse::Button) {});
            int mouseMoveListener = mouse.onMouseMove([](int, int) {});
            int wheelScrollListener = mouse.onWheelScroll([](Mouse::Wheel, float, int, int) {});

            CHECK(mouse.unsubscribe(mighter2d::MouseEvent::MouseUp, buttonUpListener));
            CHECK(mouse.unsubscribe(mighter2d::MouseEvent::MouseDown, buttonDownListener));
            CHECK(mouse.unsubscribe(mighter2d::MouseEvent::MouseMove, mouseMoveListener));
            CHECK(mouse.unsubscribe(mighter2d::MouseEvent::MouseWheelScroll, wheelScrollListener));
        }

        SUBCASE("Removing an existing event listener from an event its not listening for returns false")
        {
            Mouse mouse;

            int buttonUpListener = mouse.onButtonUp([](Mouse::Button) {});

            CHECK_FALSE(mouse.unsubscribe(mighter2d::MouseEvent::MouseDown, buttonUpListener));
            CHECK_FALSE(mouse.unsubscribe(mighter2d::MouseEvent::MouseMove, buttonUpListener));
            CHECK_FALSE(mouse.unsubscribe(mighter2d::MouseEvent::MouseWheelScroll, buttonUpListener));
        }
    }

    SUBCASE("String-enum conversions")
    {
        SUBCASE("buttonToString()")
        {
            CHECK_EQ(Mouse::buttonToString(Mouse::Button::Left), "Left");
            CHECK_EQ(Mouse::buttonToString(Mouse::Button::Right), "Right");
            CHECK_EQ(Mouse::buttonToString(Mouse::Button::Middle), "Middle");
            CHECK_EQ(Mouse::buttonToString(Mouse::Button::XButton1), "XButton1");
            CHECK_EQ(Mouse::buttonToString(Mouse::Button::XButton2), "XButton2");
        }

        SUBCASE("stringToButton()")
        {
            CHECK_EQ(Mouse::stringToButton("Left"), Mouse::Button::Left);
            CHECK_EQ(Mouse::stringToButton("Right"), Mouse::Button::Right);
            CHECK_EQ(Mouse::stringToButton("Middle"), Mouse::Button::Middle);
            CHECK_EQ(Mouse::stringToButton("XButton1"), Mouse::Button::XButton1);
            CHECK_EQ(Mouse::stringToButton("XButton2"), Mouse::Button::XButton2);
        }
    }
}