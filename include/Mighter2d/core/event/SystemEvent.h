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

#ifndef MIGHTER2D_SYSTEMEVENT_H
#define MIGHTER2D_SYSTEMEVENT_H

#include "Mighter2d/core/input/Keyboard.h"
#include "Mighter2d/core/input/Mouse.h"
#include "Mighter2d/core/input/Joystick.h"

namespace mighter2d {
    /**
     * @brief Defines a system event and its parameters
     *
     * This class is based on sf::Event from SFML (www.sfml-dev.org)
     */
    class SystemEvent {
    public:
        /**
         * @brief Size events parameters (Resized)
         */
        struct SizeEvent {
            unsigned int width;  //!< New width, in pixels
            unsigned int height; //!< New height, in pixels
        };

        /**
         * @brief Keyboard event parameters (KeyPressed, KeyReleased)
         */
        struct KeyEvent {
            input::Keyboard::Key code; //!< Code of the key that has been pressed
            bool          alt;         //!< Is the Alt key pressed?
            bool          control;     //!< Is the Control key pressed?
            bool          shift;       //!< Is the Shift key pressed?
            bool          system;      //!< Is the System key pressed?
        };

        /**
         * @brief Text event parameters (TextEntered)
         */
        struct TextEvent {
            unsigned int unicode; //!< UTF-32 Unicode value of the character
        };

        /**
         * @brief Mouse move event parameters (MouseMoved)
         */
        struct MouseMoveEvent {
            int x; //!< X position of the mouse pointer, relative to the left of the owner window
            int y; //!< Y position of the mouse pointer, relative to the top of the owner window
        };

        /**
         * @brief Mouse buttons events parameters
         *       (MouseButtonPressed, MouseButtonReleased)
         */
        struct MouseButtonEvent {
            input::Mouse::Button button; //!< Code of the button that has been pressed
            int           x;             //!< X position of the mouse pointer, relative to the left of the owner window
            int           y;             //!< Y position of the mouse pointer, relative to the top of the owner window
        };

        /**
         * @brief Mouse wheel events parameters (MouseWheelScrolled)
         */
        struct MouseWheelScrollEvent {
            input::Mouse::Wheel wheel; //!< Which wheel (for mice with multiple ones)
            float        delta;        //!< Wheel offset (positive is up/left, negative is down/right). High-precision mice may use non-integral offsets.
            int          x;            //!< X position of the mouse pointer, relative to the left of the owner window
            int          y;            //!< Y position of the mouse pointer, relative to the top of the owner window
        };

        /**
         * @brief Joystick connection events parameters
         *        (JoystickConnected, JoystickDisconnected)
         */
        struct JoystickConnectEvent {
            unsigned int joystickId; //!< Index of the joystick (in range [0 .. Joystick::Count - 1])
        };

        /**
         * @brief Joystick axis move event parameters (JoystickMoved)
         */
        struct JoystickMoveEvent {
            unsigned int   joystickId;  //!< Index of the joystick (in range [0 .. Joystick::Count - 1])
            input::Joystick::Axis axis; //!< Axis on which the joystick moved
            float          position;    //!< New position on the axis (in range [-100 .. 100])
        };

        /**
         * @brief Joystick buttons events parameters
         *        (JoystickButtonPressed, JoystickButtonReleased)
         */
        struct JoystickButtonEvent {
            unsigned int joystickId; //!< Index of the joystick (in range [0 .. Joystick::Count - 1])
            unsigned int button;     //!< Index of the button that has been pressed (in range [0 .. Joystick::ButtonCount - 1])
        };

        /**
         * @brief Enumeration of the different types of events
         */
        enum Type {
            Unknown = -1,            //!< Unhandled event
            Closed = 0,              //!< The window requested to be closed (no data)
            Resized,                 //!< The window was resized (data in event.size)
            LostFocus,               //!< The window lost the focus (no data)
            GainedFocus,             //!< The window gained the focus (no data)
            TextEntered,             //!< A character was entered (data in event.text)
            KeyPressed,              //!< A key was pressed (data in event.key)
            KeyReleased,             //!< A key was released (data in event.key)
            MouseWheelScrolled,      //!< The mouse wheel was scrolled (data in event.mouseWheelScroll)
            MouseButtonPressed,      //!< A mouse button was pressed (data in event.mouseButton)
            MouseButtonReleased,     //!< A mouse button was released (data in event.mouseButton)
            MouseMoved,              //!< The mouse cursor moved (data in event.mouseMove)
            MouseEntered,            //!< The mouse cursor entered the area of the window (no data)
            MouseLeft,               //!< The mouse cursor left the area of the window (no data)
            JoystickButtonPressed,   //!< A joystick button was pressed (data in event.joystickButton)
            JoystickButtonReleased,  //!< A joystick button was released (data in event.joystickButton)
            JoystickMoved,           //!< The joystick moved along an axis (data in event.joystickMove)
            JoystickConnected,       //!< A joystick was connected (data in event.joystickConnect)
            JoystickDisconnected,    //!< A joystick was disconnected (data in event.joystickConnect)
        };

        ////////////////////////////////////////////////////////////
        // Member data
        ////////////////////////////////////////////////////////////
        Type type; //!< Type of the event

        union {
            SizeEvent             size;              //!< Size event parameters (SystemEvent::Resized)
            KeyEvent              key;               //!< Key event parameters (SystemEvent::KeyPressed, SystemEvent::KeyReleased)
            TextEvent             text;              //!< Text event parameters (SystemEvent::TextEntered)
            MouseMoveEvent        mouseMove;         //!< Mouse move event parameters (SystemEvent::MouseMoved)
            MouseButtonEvent      mouseButton;       //!< Mouse button event parameters (SystemEvent::MouseButtonPressed, SystemEvent::MouseButtonReleased)
            MouseWheelScrollEvent mouseWheelScroll;  //!< Mouse wheel event parameters (SystemEvent::MouseWheelScrolled)
            JoystickMoveEvent     joystickMove;      ///< Joystick move event parameters (SystemEvent::JoystickMoved)
            JoystickButtonEvent   joystickButton;    ///< Joystick button event parameters (SystemEvent::JoystickButtonPressed, SystemEvent::JoystickButtonReleased)
            JoystickConnectEvent  joystickConnect;   ///< Joystick (dis)connect event parameters (SystemEvent::JoystickConnected, SystemEvent::JoystickDisconnected)
        };
    };
}

#endif //MIGHTER2D_SYSTEMEVENT_H
