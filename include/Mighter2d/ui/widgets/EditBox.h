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

#ifndef MIGHTER2D_EDITBOX_H
#define MIGHTER2D_EDITBOX_H

#include "Mighter2d/Config.h"
#include "Mighter2d/ui/widgets/ClickableWidget.h"
#include "Mighter2d/ui/renderers/EditBoxRenderer.h"
#include <memory>

namespace mighter2d {
    namespace ui {
        /**
         * @brief A widget that can capture user input on a keyboard
         */
        class MIGHTER2D_API EditBox : public ClickableWidget {
        public:
            using Ptr = std::unique_ptr<EditBox>; //!< Unique widget pointer
            using ConstPtr = std::unique_ptr<const EditBox>; //!< Const unique widget pointer

            /**
             * @brief Copy constructor
             */
            EditBox(const EditBox&);

            /**
             * @brief Copy assignment operator
             */
            EditBox& operator=(const EditBox&);

            /**
             * @brief Move constructor
             */
            EditBox(EditBox&&) noexcept;

            /**
             * @brief Move assignment operator
             */
            EditBox& operator=(EditBox&&) noexcept;

            /**
             * @brief Create a new edit box widget
             * @param defaultText Text is displayed when the edit box is empty
             * @return The new edit box
             */
            static EditBox::Ptr create(const std::string& defaultText = "");

            /**
             * @brief Create a copy of this widget
             * @return A copy of this widget
             *
             * @see clone
             */
            EditBox::Ptr copy() const;

            /**
             * @brief Get the labels renderer
             * @return The labels renderer
             *
             * The renderer gives access to functions that determine how the
             * widget is displayed. It allows you to manipulate things such
             * as the background colour, text colour, border colour etc...
             */
            EditBoxRenderer* getRenderer();
            const EditBoxRenderer* getRenderer() const;

            /**
             * @brief Set the text inside the edit box
             * @param text Text inside the edit box
             *
             * The last characters of the text might be removed if:
             * - There is a set a character limit and this text contains
             *   too much characters.
             * - The text width is limited and the text does not fit inside
             *   the EditBox.
             *
             * @see setMaximumCharacters
             * @see limitTextWidth
             */
            void setText(const std::string &text);

            /**
             * @brief Get the text inside the edit box
             * @return The text inside the edit box
             */
            std::string getText() const;

            /**
             * @brief Set the default text of the editbox
             * @param text The text to set
             *
             * The default text is displayed when the edit box is empty
             */
            void setDefaultText(const std::string& text);

            /**
             * @brief Get the default text
             * @return The default text
             */
            std::string getDefaultText() const;

            /**
             * @brief Set the character limit
             * @param maxChars The character limit
             *
             * The characters are not limited by default
             */
            void setMaximumCharacters(unsigned int maxChars);

            /**
             * @brief Get the character limit
             * @return The character limit or 0 if n limit is set
             */
            unsigned int getMaximumCharacters() const;

            /**
             * @brief Limit the text field width
             * @param limitWidth True to limit the text width otherwise false
             *
             * When the text width is limited, you wont be able to enter more
             * text when the text field is full. The text field width is not
             * limited by default
             */
            void limitTextWidth(bool limitWidth = true);

            /**
             * @brief Check if the text field width is limited or not
             * @return True if the text field is limited otherwise false
             */
            bool isTextWidthLimited() const;

            /**
             * @brief Make the edit box read-only or make it writable again
             * @param readOnly True to set the edit box as reda-only, otherwise
             *        false
             *
             * When the edit box is read-only, you can no longer delete
             * characters and type text. However, selecting text, copying text
             * and even calling the setText() function will still work
             */
            void setReadOnly(bool readOnly = true);

            /**
             * @brief Check if the edit box read-only or writable
             * @return True if the edit box is read-only or false if it is
             *         writable
             */
            bool isReadOnly() const;

            /**
             * @brief Place the caret after a specific character
             * @param charactersBeforeCaret The new position of the caret
             */
            void setCaretPosition(std::size_t charactersBeforeCaret);

            /**
             * @brief Get the position of the caret
             * @return The position of the caret
             */
            std::size_t getCaretPosition() const;

            /**
             * @brief Place a suffix at the right side of the edit box
             * @param suffix Text to show on right side of edit box
             *
             * Setting a suffix can be useful for inputting numbers where you
             * want to unit to be displayed inside the edit box
             */
            void setSuffix(const std::string& suffix);

            /**
             * @brief Get suffix currently displayed on the right side of the
             *        edit box
             * @return Text shown on right side of edit box
             */
            std::string getSuffix() const;

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
             * @brief Get the type of the edit box
             * @return The type of the edit box
             */
            std::string getWidgetType() const override;

            /**
             * @brief Destructor
             */
            ~EditBox() override;

        public:
            ///////////////////////////////////////////////////////////////////
            // Event listeners
            ///////////////////////////////////////////////////////////////////

            /**
             * @brief Add an event listener to text change event
             * @param callback The function to be executed when edit box text changes
             * @param oneTime True to execute the callback one-time or false to
             *                execute it every time the event is triggered
             * @return The event listeners unique identification number
             *
             * You can add any number of event handlers to this event, the callback is
             * passed the new editbox text
             *
             * @see unsubscribe
             */
            int onTextChange(const Callback<std::string>& callback, bool oneTime = false);

            /**
             * @brief Add an event listener to a enter key press event
             * @param callback The function to be executed when the enter is pressed whilst the edit box is focused
             * @param oneTime True to execute the callback one-time or false to
             *                execute it every time the event is triggered
             * @return The event listeners unique identification number
             *
             * You can add any number of event handlers to this event, the callback is
             * passed the text in the edit box
             *
             * @see unsubscribe
             */
            int onEnterKeyPress(const Callback<std::string>& callback, bool oneTime = false);

        private:
            /**
             * @brief Construct an edit box
             * @param defaultText Text is displayed when the edit box is empty
             */
            explicit EditBox(const std::string& defaultText = "");

            /**
             * @brief Initialize events emitted by the widget
             */
            void initEvents();

        private:
            class EditBoxImpl;
            std::unique_ptr<EditBoxImpl> pimpl_;
        };
    }
}

/**
 * @class mighter2d::ui::EditBox
 * @ingroup ui
 *
 * <table>
 * <caption>Events generated by mighter2d::ui::EditBox </caption>
 * <tr><th> Name            <th> Argument Type        <th> Description
 * <tr><td> textChange      <td> std::string          <td> The text in the edit box changed. Parameter = New text in the edit box
 * <tr><td> enterKeyPress   <td> void or std::string  <td> Enter key was pressed while widget was focused.
 *                                                         Optional parameter = The text in the edit box
 * </table>
 *
 * Usage Example:
 * @code
 * editBox.onTextChange([](std::string text) {
 *      std::cout << "You entered " << text << "\n";
 * });
 * @endcode
 */

#endif // MIGHTER2D_EDITBOX_H
