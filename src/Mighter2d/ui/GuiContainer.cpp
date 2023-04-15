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

/*-----------------------------------------------------------------------------
 It pains me deeply that most of the code in this file is a duplicate of
 mighter2d::ui::WidgetContainer implementation :( . The problem is that an mighter2d::ui::GuiContainer
 is supposed to contain any instances of mighter2d::ui::Widget that the user creates
 without itself being a Widget. Another problem is that the this class and the
 mighter2d::ui::GuiContainer class both delegate to third party instances of unrelated
 types whose functionality is similar. So I can't contain an mighter2d::ui::WidgetContainer
 in this class and delegate to it nor can i use private inheritance as the result
 would not be as intended.

 Please don't shoot me when you see how awful the copy and paste is. There must
 be a way around this and I will find it. As soon as I do I will remove this
 disgusting thing I've done here. Bare with me as I add 1 and 1 together
 -----------------------------------------------------------------------------*/

#include "Mighter2d/ui/GuiContainer.h"
#include "Mighter2d/core/resources/ResourceManager.h"
#include "Mighter2d/graphics/RenderTarget.h"
#include "Mighter2d/utility/Helpers.h"
#include "Mighter2d/core/scene/Scene.h"
#include "Mighter2d/ui/widgets/TabsContainer.h"
#include <TGUI/Backends/SFML/GuiSFML.hpp>
#include <unordered_map>
#include <iostream>

namespace mighter2d::ui {
    //////////////////////////////////////////////////////////////////////////
    // GuiContainer implementation
    //////////////////////////////////////////////////////////////////////////
    class GuiContainer::GuiContainerImpl {
    public:
        GuiContainerImpl() = default;

        explicit GuiContainerImpl(priv::RenderTarget &window) :
            sfmlGui_{window.getThirdPartyWindow()}
        {
            sfmlGui_.setDrawingUpdatesTime(false);
        }

        void setAbsoluteViewport(const FloatRect &viewport) {
            sfmlGui_.setAbsoluteViewport({viewport.left, viewport.top,
                viewport.width, viewport.height});
        }

        void setRelativeViewport(const FloatRect &viewport) {
            sfmlGui_.setRelativeViewport({viewport.left, viewport.top,
                viewport.width, viewport.height});
        }

        FloatRect getViewport() const {
            return {sfmlGui_.getViewport().getPosition().x,
                    sfmlGui_.getViewport().getPosition().y,
                    sfmlGui_.getViewport().getSize().x,
                    sfmlGui_.getViewport().getSize().y};
        }

        void setAbsoluteView(const FloatRect &view) {
            sfmlGui_.setAbsoluteView({view.left, view.top, view.width, view.height});
        }

        void setRelativeView(const FloatRect &view) {
            sfmlGui_.setRelativeView({view.left, view.top, view.width, view.height});
        }

        FloatRect getView() const {
            return {sfmlGui_.getView().getPosition().x,
                    sfmlGui_.getView().getPosition().y,
                    sfmlGui_.getView().getSize().x,
                    sfmlGui_.getView().getSize().y};
        }

        void handleEvent(Event event) {
            sfmlGui_.handleEvent(utility::convertToSFMLEvent(event));
        }

        void setTabKeyUsageEnabled(bool enabled) {
            sfmlGui_.setTabKeyUsageEnabled(enabled);
        }

        bool isTabKeyUsageEnabled() const {
            return sfmlGui_.isTabKeyUsageEnabled();
        }

        void draw() {
            sfmlGui_.draw();
        }

        void setFont(const std::string& filename) {
            sfmlGui_.setFont(utility::get_TGUI_Font(filename));
        }

        void removeAllWidgets() {
            sfmlGui_.removeAllWidgets();
            widgets_.clear();
        }

        void setTarget(priv::RenderTarget &window) {
            sfmlGui_.setTarget(window.getThirdPartyWindow());
            sfmlGui_.setDrawingUpdatesTime(false);
        }

        Widget* getWidgetBelowMouseCursor(Vector2f mousePos) const {
            tgui::Widget::Ptr widget = sfmlGui_.getWidgetBelowMouseCursor({static_cast<int>(mousePos.x), static_cast<int>(mousePos.y)});

            if (widget)
                return getWidget(widget->getWidgetName().toStdString());

            return nullptr;
        }

        void unfocusAllWidgets() {
            sfmlGui_.unfocusAllWidgets();
        }

        void setOpacity(float opacity) {
            sfmlGui_.setOpacity(opacity);
        }

        float getOpacity() const {
            return sfmlGui_.getOpacity();
        }

        void setMouseCursor(CursorType cursorType) {
            sfmlGui_.setOverrideMouseCursor(static_cast<tgui::Cursor::Type>(cursorType));
        }

        void restoreMouseCursor() {
            sfmlGui_.restoreOverrideMouseCursor();
        }

        void requestMouseCursor(CursorType cursorType) {
            sfmlGui_.requestMouseCursor(static_cast<tgui::Cursor::Type>(cursorType));
        }

        Widget* addWidget(Widget::Ptr widget, const std::string &name) {
            MIGHTER2D_ASSERT(widget, "Cannot add nullptr to a GuiContainer")

            // Maintain Compatibility with v2.2.x
            if (!name.empty())
                widget->setName(name);

            auto [iter, inserted] = widgets_.insert({widget->getName(), std::move(widget)});
            if (inserted) {
                sfmlGui_.add(std::static_pointer_cast<tgui::Widget>(iter->second->getInternalPtr()), iter->second->getName());
                return iter->second.get();
            }

            std::cerr << "Mighter2d ERROR: A widget with the name \"" + iter->second->getName() + "\" already exists in the container, widget names must be unique";
            exit(-3); //@TODO - Replace magic number with error code
        }

        Widget* getWidget(const std::string &widgetName) const {
            if (utility::findIn(widgets_, widgetName))
                return widgets_.at(widgetName).get();
            return utility::findRecursively(widgets_, widgetName);
        }

        bool removeWidget(const std::string &widget) {
            if (utility::findIn(widgets_, widget)) {
                sfmlGui_.remove(std::static_pointer_cast<tgui::Widget>(widgets_[widget]->getInternalPtr()));
                widgets_.erase(widget);
                return true;
            } else
                return utility::removeRecursively(widgets_, widget);
        }

        Widget* getFocusedWidget() const {
            tgui::Widget::Ptr widget = sfmlGui_.getFocusedChild();

            if (widget)
                return widgets_.at(widget->getWidgetName().toStdString()).get();

            return nullptr;
        }

        Widget* getFocusedLeaf() const {
            tgui::Widget::Ptr widget = sfmlGui_.getFocusedLeaf();

            if (widget)
                return getWidget(widget->getWidgetName().toStdString());

            return nullptr;
        }

        Widget* getWidgetAtPosition(Vector2f pos) const {
            tgui::Widget::Ptr widget = sfmlGui_.getWidgetAtPosition({pos.x, pos.y});

            if (widget)
                return getWidget(widget->getWidgetName().toStdString());

            return nullptr;
        }

        bool focusNextWidget(bool recursive) {
            return sfmlGui_.focusNextWidget(recursive);
        }

        bool focusPreviousWidget(bool recursive) {
            return sfmlGui_.focusPreviousWidget(recursive);
        }

        void moveWidgetToFront(const Widget* widget) {
            MIGHTER2D_ASSERT(widget, "Widget to be moved cannot be a nullptr")
            sfmlGui_.moveWidgetToFront(std::static_pointer_cast<tgui::Widget>(widget->getInternalPtr()));
        }

        void moveWidgetToBack(const Widget* widget) {
            MIGHTER2D_ASSERT(widget, "Widget to be moved cannot be a nullptr")
            sfmlGui_.moveWidgetToBack(std::static_pointer_cast<tgui::Widget>(widget->getInternalPtr()));
        }

        size_t moveWidgetForward(const Widget* widget) {
            MIGHTER2D_ASSERT(widget, "Widget to be moved cannot be a nullptr")
            return sfmlGui_.moveWidgetForward(std::static_pointer_cast<tgui::Widget>(widget->getInternalPtr()));
        }

        size_t moveWidgetBackward(const Widget* widget) {
            MIGHTER2D_ASSERT(widget, "Widget to be moved cannot be a nullptr")
            return sfmlGui_.moveWidgetBackward(std::static_pointer_cast<tgui::Widget>(widget->getInternalPtr()));
        }

        void setTextSize(unsigned int size) {
            sfmlGui_.setTextSize(size);
        }

        unsigned int getTextSize() const {
            return sfmlGui_.getTextSize();
        }

        bool isTargetSet() const {
            return sfmlGui_.getTarget() != nullptr;
        }

        void update(Time deltaTime) {
            sfmlGui_.updateTime(tgui::Duration(deltaTime.asMilliseconds()));
        }

    private:
        tgui::GuiSFML sfmlGui_; //!< Gui controller and renderer
        std::unordered_map<std::string, Widget::Ptr> widgets_; //!< Widgets container
    }; // class Impl

    //////////////////////////////////////////////////////////////////////////
    // GuiContainer class delegation
    //////////////////////////////////////////////////////////////////////////

    GuiContainer::GuiContainer(Scene& scene) :
        Drawable(scene),
        scene_(&scene),
        pimpl_{std::make_unique<GuiContainerImpl>()}
    {
        scene.addUpdatable(this);

        onDestruction([&scene, this] {
            scene.removeUpdatable(this);
        });
    }

    GuiContainer::GuiContainer(Scene& scene, priv::RenderTarget &window) :
        Drawable(scene),
        scene_(&scene),
        pimpl_{std::make_unique<GuiContainerImpl>(window)}
    {
        scene.addUpdatable(this);

        onDestruction([&scene, this] {
            scene.removeUpdatable(this);
        });
    }

    GuiContainer::GuiContainer(GuiContainer &&) noexcept = default;

    GuiContainer &GuiContainer::operator=(GuiContainer &&) noexcept = default;

    Scene& GuiContainer::getScene() {
        return *scene_;
    }

    const Scene& GuiContainer::getScene() const {
        return *scene_;
    }

    void GuiContainer::setAbsoluteViewport(const FloatRect &viewport) {
        pimpl_->setAbsoluteViewport(viewport);
    }

    void GuiContainer::setRelativeViewport(const FloatRect &viewport) {
        pimpl_->setRelativeViewport(viewport);
    }

    FloatRect GuiContainer::getViewport() const {
        return pimpl_->getViewport();
    }

    void GuiContainer::setAbsoluteView(const FloatRect &view) {
        pimpl_->setAbsoluteView(view);
    }

    void GuiContainer::setRelativeView(const FloatRect &view) {
        pimpl_->setRelativeView(view);
    }

    FloatRect GuiContainer::getView() const {
        return pimpl_->getView();
    }

    void GuiContainer::handleEvent(Event event) {
        pimpl_->handleEvent(event);
    }

    void GuiContainer::setTabKeyUsageEnabled(bool enabled) {
        pimpl_->setTabKeyUsageEnabled(enabled);
    }

    bool GuiContainer::isTabKeyUsageEnabled() const {
        return pimpl_->isTabKeyUsageEnabled();
    }

    void GuiContainer::setFont(const std::string& filename) {
        pimpl_->setFont(filename);
    }

    void GuiContainer::removeAllWidgets() {
        pimpl_->removeAllWidgets();
    }

    void GuiContainer::setTarget(priv::RenderTarget &window) {
        pimpl_->setTarget(window);
    }

    Widget* GuiContainer::getWidgetBelowMouseCursor(Vector2f mousePos) const {
        return pimpl_->getWidgetBelowMouseCursor(mousePos);
    }

    void GuiContainer::unfocusAllWidgets() {
        pimpl_->unfocusAllWidgets();
    }

    void GuiContainer::setOpacity(float opacity) {
        pimpl_->setOpacity(opacity);
    }

    float GuiContainer::getOpacity() const {
        return pimpl_->getOpacity();
    }

    void GuiContainer::setMouseCursor(CursorType cursorType) {
        pimpl_->setMouseCursor(cursorType);
    }

    void GuiContainer::restoreMouseCursor() {
        pimpl_->restoreMouseCursor();
    }

    void GuiContainer::requestMouseCursor(CursorType cursorType) {
        pimpl_->requestMouseCursor(cursorType);
    }

    Widget* GuiContainer::addWidget(Widget::Ptr widget, const std::string &widgetName) {
        return pimpl_->addWidget(std::move(widget), widgetName);
    }

    Widget* GuiContainer::getWidget(const std::string &widgetName) const {
        return pimpl_->getWidget(widgetName);
    }

    bool GuiContainer::removeWidget(const std::string &widget) {
        return pimpl_->removeWidget(widget);
    }

    Widget* GuiContainer::getFocusedWidget() const {
        return pimpl_->getFocusedWidget();
    }

    Widget* GuiContainer::getFocusedLeaf() const {
        return pimpl_->getFocusedLeaf();
    }

    Widget* GuiContainer::getWidgetAtPosition(Vector2f pos) const {
        return pimpl_->getWidgetAtPosition(pos);
    }

    bool GuiContainer::focusNextWidget(bool recursive) {
        return pimpl_->focusNextWidget(recursive);
    }

    bool GuiContainer::focusPreviousWidget(bool recursive) {
        return pimpl_->focusPreviousWidget(recursive);
    }

    void GuiContainer::moveWidgetToFront(const Widget* widget) {
        pimpl_->moveWidgetToFront(widget);
    }

    void GuiContainer::moveWidgetToBack(const Widget* widget) {
        pimpl_->moveWidgetToBack(widget);
    }

    size_t GuiContainer::moveWidgetForward(const Widget* widget) {
        return pimpl_->moveWidgetForward(widget);
    }

    size_t GuiContainer::moveWidgetBackward(const Widget* widget) {
        return pimpl_->moveWidgetBackward(widget);
    }

    void GuiContainer::moveWidgetToFront(const std::string &widget) {
        moveWidgetToFront(getWidget(widget));
    }

    void GuiContainer::moveWidgetToBack(const std::string &widget) {
        moveWidgetToBack(getWidget(widget));
    }

    size_t GuiContainer::moveWidgetForward(const std::string &widget) {
        return moveWidgetForward(getWidget(widget));
    }

    size_t GuiContainer::moveWidgetBackward(const std::string &widget) {
        return moveWidgetBackward(getWidget(widget));
    }

    void GuiContainer::setTextSize(unsigned int size) {
        pimpl_->setTextSize(size);
    }

    unsigned int GuiContainer::getTextSize() const {
        return pimpl_->getTextSize();
    }

    bool GuiContainer::isTargetSet() const {
        return pimpl_->isTargetSet();
    }

    void GuiContainer::update(mighter2d::Time deltaTime) {
        pimpl_->update(deltaTime);
    }

    std::string GuiContainer::getClassName() const {
        return "GuiContainer";
    }

    void GuiContainer::draw(priv::RenderTarget&) const {
        pimpl_->draw();
    }

    GuiContainer::~GuiContainer() = default;
}
