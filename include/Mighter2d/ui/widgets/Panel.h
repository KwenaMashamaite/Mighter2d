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

#ifndef MIGHTER2D_PANEL_H
#define MIGHTER2D_PANEL_H

#include "Mighter2d/Config.h"
#include "Mighter2d/ui/widgets/WidgetContainer.h"
#include "Mighter2d/ui/renderers/PanelRenderer.h"
#include <memory>

namespace mighter2d {
    namespace ui {
        /**
         * @brief A rectangular container for other widgets
         */
        class MIGHTER2D_API Panel : public WidgetContainer {
        public:
            using Ptr = std::unique_ptr<Panel>; //!< Unique widget pointer
            using ConstPtr = std::unique_ptr<const Panel>; //!< Const unique widget pointer

            /**
             * @brief Copy constructor
             */
            Panel(const Panel&);

            /**
             * @brief Copy assignment operator
             */
            Panel& operator=(const Panel&);

            /**
             * @brief Move constructor
             */
            Panel(Panel&&) noexcept = default;

            /**
             * @brief Move assignment operator
             */
            Panel& operator=(Panel&&) noexcept = default;

            /**
             * @brief Create a new Panel widget
             * @param width Width of the panel relative to the size of its
             *        parent
             * @param height Height of the panel relative to the size of its
             *        parent
             * @return The new Panel
             *
             * * The relative size is specified in percentages as shown:
             *
             * @code
             * Panel::create({"50%", "20%"});
             * @endcode
             * 
             * By default, the new panel is the same size as its parent
             */
            static Panel::Ptr create(const std::string& width = "100%",
                const std::string& height = "100%");

            /**
             * @brief Create a copy of this widget
             * @return A copy of this widget
             *
             * @see clone
             */
            Panel::Ptr copy() const;

            /**
             * @brief Get the panels renderer
             * @return The panels renderer
             *
             * The renderer gives access to functions that determine how the
             * panel is displayed. It allows you to manipulate things such
             * as the background colour, border colour etc...
             *
             * @see setRenderer
             */
            PanelRenderer* getRenderer();
            const PanelRenderer* getRenderer() const;

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
             * @brief Get the type of the panel
             * @return The type of the panel
             */
            std::string getWidgetType() const override;

            /**
             * @brief Destructor
             */
            ~Panel() override = default;

        private:
            /**
             * @brief Constructor
             * @param width Width of the panel relative to the size of its
             *        parent
             * @param height Height of the panel relative to the size of its
             *        parent
             *
             * The relative size is specified in percentages as shown:
             *
             * @code
             * Panel({"50%", "20%"});
             * @endcode
             *
             * By default, the panel is the same size as its parent
             */
            explicit Panel(const std::string& width = "100%", const std::string& height = "100%");

            /**
             * @brief Initialize widget events
             */
            void initEvents();
        };
    }
}

/**
 * @class mighter2d::ui::Panel
 * @ingroup ui
 *
 * <table>
 * <caption>Events generated by mighter2d::ui::Panel </caption>
 * <tr><th> Name            <th> Argument Type          <th> Description
 * <tr><td> doubleClick     <td> void or mighter2d::Vector2f  <td> The panel was double clicked. Optional parameter = Coordinates of the mouse cursor
 *                                                           relative to the panel
 * </table>
 *
 * Event usage example:
 * @code
 * panel.on("doubleClick", mighter2d::Callback<mighter2d::Vector2f>([](mighter2d::Vector2f mousePos) {
 *      std::cout << "Widget clicked at {" << mousePos.x << ", " << mousePos.y << "}" << "\n";
 * }));
 * @endcode
 */

#endif //MIGHTER2D_PANEL_H
