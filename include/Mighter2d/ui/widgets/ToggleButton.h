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

#ifndef MIGHTER2D_TOGGLEBUTTON_H
#define MIGHTER2D_TOGGLEBUTTON_H

#include "Mighter2d/Config.h"
#include "Mighter2d/ui/widgets/ClickableWidget.h"
#include "Mighter2d/ui/renderers/ButtonRenderer.h"
#include <memory>

namespace mighter2d {
    namespace ui {
        /**
         * @brief A button that can be toggled between two states (on and off)
         */
        class MIGHTER2D_API ToggleButton : public ClickableWidget {
        public:
            using Ptr = std::unique_ptr<ToggleButton>; //!< Unique widget pointer
            using ConstPtr = std::unique_ptr<const ToggleButton>; //!< Const unique widget pointer

            /**
             * @brief Copy constructor
             */
            ToggleButton(const ToggleButton&);

            /**
             * @brief Copy assignment operator
             */
            ToggleButton& operator=(const ToggleButton&);

            /**
             * @brief Move constructor
             */
            ToggleButton(ToggleButton&&) noexcept;

            /**
             * @brief Move assignment operator
             */
            ToggleButton& operator=(ToggleButton&&) noexcept;

            /**
             * @brief Create a new toggle button widget
             * @param text The text to display on the button
             * @param checked True to create button checked otherwise false
             * @return The new toggle button
             */
            static ToggleButton::Ptr create(const std::string& text = "", bool checked = false);

            /**
             * @brief Create a copy of this widget
             * @return A copy of this widget
             */
            ToggleButton::Ptr copy() const;

            /**
             * @brief Get the buttons renderer
             * @return The buttons renderer
             *
             * The renderer gives access to functions that determine how the
             * button is displayed. It allows you to manipulate things such
             * as the background colour, text colour, border colour etc...
             *
             * @see setRenderer
             */
            ButtonRenderer* getRenderer();
            const ButtonRenderer* getRenderer() const;

            /**
             * @brief Set the text displayed on the button
             * @param text Text to display
             *
             * This function will overwrite any text that was previously
             * set
             */
            void setText(const std::string &text);

            /**
             * @brief Get the text displayed on the button
             * @return The text displayed oon the button
             */
            std::string getText() const;

            /**
             * @brief Set the checked state of the button
             * @param checked True to check or false to uncheck the button
             */
            void setChecked(bool checked);

            /**
             * @brief Check if the button is checked or not
             * @return True if checked, otherwise false
             */
            bool isChecked() const;

            /**
             * @brief Make a copy of this widget
             * @return A copy of this widget
             *
             * You should use this function if you don't care about the type
             * of the widget, otherwise use the widgets copy function
             *
             * @see copy
             */
            Widget::Ptr clone() const override;

            /**
             * @brief Get the type of the button
             * @return The type of the button
             */
            std::string getWidgetType() const override;

            /**
             * @brief Destructor
             */
            ~ToggleButton() override;

        public:
            ///////////////////////////////////////////////////////////////////
            // Event listeners
            ///////////////////////////////////////////////////////////////////

            /**
             * @brief Add an event listener to toggle event
             * @param callback The function to be executed when the button is toggled
             * @param oneTime True to execute the callback one-time or false to
             *                execute it every time the event is triggered
             * @return The event listeners unique identification number
             *
             * You can add any number of event handlers to this event, the callbakc is passed
             * the toggle state of the button
             *
             * @see unsubscribe
             */
            int onToggle(const Callback<bool>& callback, bool oneTime = false);

        private:
            /**
            * @brief Construct a toggle button objet
            * @param text The text to display on the button
            * @param checked True to create button checked otherwise false
            */
            explicit ToggleButton(const std::string& text = "", bool checked = false);

            /**
             * @brief Initialize events emitted by the widget
             */
            void initEvents();

        private:
            class ButtonImpl;
            std::unique_ptr<ButtonImpl> pimpl_;
        };
    }
}

/**
 * @class mighter2d::ui::ToggleButton
 * @ingroup ui
 *
 * <table>
 * <caption>Events generated by mighter2d::ui::ToggleButton </caption>
 * <tr><th> Name            <th> Argument Type          <th> Description
 * <tr><td> toggle          <td> void or bool           <td> The button state was toggled.
 *                                                           Optional parameter = The new state of the button
 * </table>
 *
 * Event usage example:
 * @code
 * toggleBtn.onToggle([](bool on) {
 *      if (on)
 *          std::cout << "Button turned on";
 *      else
 *          std::cout << "Button turned off";
 * });
 * @endcode
 */

#endif //MIGHTER2D_TOGGLEBUTTON_H
