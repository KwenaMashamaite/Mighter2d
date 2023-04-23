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

#ifndef MIGHTER2D_CLICKABLEWIDGET_H
#define MIGHTER2D_CLICKABLEWIDGET_H

#include "Mighter2d/Config.h"
#include "Mighter2d/ui/widgets/Widget.h"
#include <string>
#include <memory>

namespace mighter2d {
    namespace ui {
        /**
         * @brief An Abstract Base Class  for clickable widgets
         */
        class MIGHTER2D_API ClickableWidget : public Widget {
        public:
            using Ptr = std::unique_ptr<ClickableWidget>; //!< Unique widget pointer

            /**
             * @internal
             * @brief Constructor
             * @param widgetImpl Widget implementation
             */
            explicit ClickableWidget(std::unique_ptr<priv::IWidgetImpl> widgetImpl);

            /**
             * @brief Copy constructor
             */
            ClickableWidget(const ClickableWidget&);

            /**
             * @brief Copy assignment operator
             */
            ClickableWidget& operator=(const ClickableWidget&);

            /**
             * @brief Move constructor
             */
            ClickableWidget(ClickableWidget&&) noexcept;

            /**
             * @brief Move assignment operator
             */
            ClickableWidget& operator=(ClickableWidget&&) noexcept;

            /**
             * @brief Destructor
             */
            ~ClickableWidget() override;

        public:
            ///////////////////////////////////////////////////////////////////
            // Event listeners
            ///////////////////////////////////////////////////////////////////

            /**
             * @brief Add an event listener to a click event
             * @param callback The function to be executed when widget is clicked
             * @param oneTime True to execute the callback one-time or false to
             *                execute it every time the event is triggered
             * @return The event listeners unique identification number
             *
             * You can add any number of event handlers to this event
             *
             * @see unsubscribe
             */
            int onClick(const Callback<>& callback, bool oneTime = false);

            /**
             * @brief Add an event listener to a click event
             * @param callback The function to be executed when widget is clicked
             * @param oneTime True to execute the callback one-time or false to
             *                execute it every time the event is triggered
             * @return The event listeners unique identification number
             *
             * You can add any number of event handlers to this event, the callback
             * is passed the coordinates of the mouse cursor relative to the widget
             *
             * @see unsubscribe
             */
            int onClick(const Callback<Vector2f>& callback, bool oneTime = false);

            /**
             * @brief Add an event listener to a left mouse down event
             * @param callback The function to be executed when the left mouse is down on the widget
             * @param oneTime True to execute the callback one-time or false to
             *                execute it every time the event is triggered
             * @return The event listeners unique identification number
             *
             * You can add any number of event handlers to this event
             *
             * @see unsubscribe
             */
            int onLeftMouseDown(const Callback<>& callback, bool oneTime = false);

            /**
             * @brief Add an event listener to a left mouse down event
             * @param callback The function to be executed when the left mouse is down on the widget
             * @param oneTime True to execute the callback one-time or false to
             *                execute it every time the event is triggered
             * @return The event listeners unique identification number
             *
             * You can add any number of event handlers to this event, the callback
             * is passed the coordinates of the mouse cursor relative to the widget
             *
             * @see unsubscribe
             */
            int onLeftMouseDown(const Callback<Vector2f>& callback, bool oneTime = false);

            /**
             * @brief Add an event listener to a left mouse up event
             * @param callback The function to be executed when the left mouse is released on the widget
             * @param oneTime True to execute the callback one-time or false to
             *                execute it every time the event is triggered
             * @return The event listeners unique identification number
             *
             * You can add any number of event handlers to this event
             *
             * @see unsubscribe
             */
            int onLeftMouseUp(const Callback<>& callback, bool oneTime = false);

            /**
             * @brief Add an event listener to a left mouse up event
             * @param callback The function to be executed when the left mouse is released on the widget
             * @param oneTime True to execute the callback one-time or false to
             *                execute it every time the event is triggered
             * @return The event listeners unique identification number
             *
             * You can add any number of event handlers to this event, the callback
             * is passed the coordinates of the mouse cursor relative to the widget
             *
             * @see unsubscribe
             */
            int onLeftMouseUp(const Callback<Vector2f>& callback, bool oneTime = false);

            /**
             * @brief Add an event listener to a right mouse down event
             * @param callback The function to be executed when the right mouse is down on the widget
             * @param oneTime True to execute the callback one-time or false to
             *                execute it every time the event is triggered
             * @return The event listeners unique identification number
             *
             * You can add any number of event handlers to this event
             *
             * @see unsubscribe
             */
            int onRightMouseDown(const Callback<>& callback, bool oneTime = false);

            /**
             * @brief Add an event listener to a right mouse down event
             * @param callback The function to be executed when the right mouse is down on the widget
             * @param oneTime True to execute the callback one-time or false to
             *                execute it every time the event is triggered
             * @return The event listeners unique identification number
             *
             * You can add any number of event handlers to this event, the callback
             * is passed the coordinates of the mouse cursor relative to the widget
             *
             * @see unsubscribe
             */
            int onRightMouseDown(const Callback<Vector2f>& callback, bool oneTime = false);

            /**
             * @brief Add an event listener to a right mouse up event
             * @param callback The function to be executed when the right mouse is released on the widget
             * @param oneTime True to execute the callback one-time or false to
             *                execute it every time the event is triggered
             * @return The event listeners unique identification number
             *
             * You can add any number of event handlers to this event
             *
             * @see unsubscribe
             */
            int onRightMouseUp(const Callback<>& callback, bool oneTime = false);

            /**
             * @brief Add an event listener to a right mouse up event
             * @param callback The function to be executed when the right mouse is released on the widget
             * @param oneTime True to execute the callback one-time or false to
             *                execute it every time the event is triggered
             * @return The event listeners unique identification number
             *
             * You can add any number of event handlers to this event, the callback
             * is passed the coordinates of the mouse cursor relative to the widget
             *
             * @see unsubscribe
             */
            int onRightMouseUp(const Callback<Vector2f>& callback, bool oneTime = false);

            /**
             * @brief Add an event listener to a right click event
             * @param callback The function to be executed when widget is right clicked
             * @param oneTime True to execute the callback one-time or false to
             *                execute it every time the event is triggered
             * @return The event listeners unique identification number
             *
             * You can add any number of event handlers to this event
             *
             * @see unsubscribe
             */
            int onRightClick(const Callback<>& callback, bool oneTime = false);

            /**
             * @brief Add an event listener to a right click event
             * @param callback The function to be executed when widget is right clicked
             * @param oneTime True to execute the callback one-time or false to
             *                execute it every time the event is triggered
             * @return The event listeners unique identification number
             *
             * You can add any number of event handlers to this event, the callback
             * is passed the coordinates of the mouse cursor relative to the widget
             *
             * @see unsubscribe
             */
            int onRightClick(const Callback<Vector2f>& callback, bool oneTime = false);

        private:
            /**
             * @brief Initialize the widgets events
             */
            void initEvents();

        private:
            struct ClickableWidgetImpl;
            std::unique_ptr<ClickableWidgetImpl> pimpl_;
        };
    }
}

/**
 * @class mighter2d::ui::ClickableWidget
 * @ingroup ui
 *
 * <table>
 * <caption>Events generated by mighter2d::ui::ClickableWidget </caption>
 * <tr><th> Name            <th> Argument Type          <th> Description
 * <tr><td> click           <td> void or mighter2d::Vector2f  <td> The widget was clicked. Optional parameter = Coordinates of the mouse cursor relative to the widget
 * <tr><td> leftMouseUp     <td> void or mighter2d::Vector2f  <td> The left mouse went up on the widget. Optional parameter = Coordinates of the mouse cursor relative to the widget
 * <tr><td> leftMouseDown   <td> void or mighter2d::Vector2f  <td> The left mouse went down on the widget. Optional parameter = Coordinates of the mouse cursor relative to the widget
 * <tr><td> rightMouseUp    <td> void or mighter2d::Vector2f  <td> The right mouse went up on the widget. Optional parameter = Coordinates of the mouse cursor relative to the widget
 * <tr><td> rightMouseDown  <td> void or mighter2d::Vector2f  <td> The right mouse went down on the widget. Optional parameter = Coordinates of the mouse cursor relative to the widget
 * <tr><td> rightClick      <td> void or mighter2d::Vector2f  <td> The widget was right clicked. Optional parameter = Coordinates of the mouse cursor relative to the widget
 * </table>
 *
 * Event usage example:
 * @code
 *
 * widget.onClick([]{
 *      std::cout << widget clicked << "\n";
 * });
 *
 * widget.onRightClick([](mighter2d::Vector2f mousePos) {
 *      std::cout << "Widget clicked at {" << mousePos.x << ", " << mousePos.y << "}" << "\n";
 * }));
 * @endcode
 */

#endif // MIGHTER2D_CLICKABLEWIDGET_H
