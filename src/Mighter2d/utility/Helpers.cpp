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

#include "Mighter2d/utility/Helpers.h"
#include "Mighter2d/core/resources/ResourceManager.h"
#include "Mighter2d/ui/widgets/WidgetContainer.h"
#include <TGUI/Backends/SFML/BackendFontSFML.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/View.hpp>
#include <TGUI/Color.hpp>
#include "Mighter2d/ui/widgets/TabsContainer.h"

namespace mighter2d::utility {
    namespace {
        auto constexpr PIXELS_PER_METRE = 30.0f;
        auto constexpr PI = 3.14159265358979323846f;
    }

    SystemEvent convertToOwnEvent(const sf::Event &SFML_Event) {
        SystemEvent MIGHTER2D_Event{};
        switch (SFML_Event.type) {
            case sf::Event::Closed:
                MIGHTER2D_Event.type = SystemEvent::Type::Closed;
                return MIGHTER2D_Event;
            case sf::Event::Resized:
                MIGHTER2D_Event.type = SystemEvent::Type::Resized;
                MIGHTER2D_Event.size.width = SFML_Event.size.width;
                MIGHTER2D_Event.size.height = SFML_Event.size.height;
                return MIGHTER2D_Event;
            case sf::Event::LostFocus:
                MIGHTER2D_Event.type = SystemEvent::Type::LostFocus;
                return MIGHTER2D_Event;
            case sf::Event::GainedFocus:
                MIGHTER2D_Event.type = SystemEvent::Type::GainedFocus;
                return MIGHTER2D_Event;
            case sf::Event::TextEntered:
                MIGHTER2D_Event.type = SystemEvent::Type::TextEntered;
                MIGHTER2D_Event.text.unicode = SFML_Event.text.unicode;
                return MIGHTER2D_Event;
            case sf::Event::KeyReleased:
            case sf::Event::KeyPressed:
                if (SFML_Event.type == sf::Event::KeyPressed)
                    MIGHTER2D_Event.type = SystemEvent::Type::KeyPressed;
                else
                    MIGHTER2D_Event.type = SystemEvent::Type::KeyReleased;

                MIGHTER2D_Event.key.code = static_cast<input::Keyboard::Key>(SFML_Event.key.code);
                MIGHTER2D_Event.key.alt = SFML_Event.key.alt;
                MIGHTER2D_Event.key.control = SFML_Event.key.control;
                MIGHTER2D_Event.key.shift = SFML_Event.key.shift;
                MIGHTER2D_Event.key.system = SFML_Event.key.system;
                return MIGHTER2D_Event;
            case sf::Event::MouseWheelScrolled:
                MIGHTER2D_Event.type = SystemEvent::Type::MouseWheelScrolled;
                if (SFML_Event.mouseWheelScroll.wheel == sf::Mouse::Wheel::VerticalWheel)
                    MIGHTER2D_Event.mouseWheelScroll.wheel = input::Mouse::Wheel::VerticalWheel;
                else
                    MIGHTER2D_Event.mouseWheelScroll.wheel = input::Mouse::Wheel::HorizontalWheel;

                MIGHTER2D_Event.mouseWheelScroll.delta = SFML_Event.mouseWheelScroll.delta;
                MIGHTER2D_Event.mouseWheelScroll.x = SFML_Event.mouseWheelScroll.x;
                MIGHTER2D_Event.mouseWheelScroll.y = SFML_Event.mouseWheelScroll.y;
                return MIGHTER2D_Event;
            case sf::Event::MouseButtonPressed:
            case sf::Event::MouseButtonReleased:
                if (SFML_Event.type == sf::Event::MouseButtonPressed)
                    MIGHTER2D_Event.type = SystemEvent::Type::MouseButtonPressed;
                else
                    MIGHTER2D_Event.type = SystemEvent::Type::MouseButtonReleased;

                MIGHTER2D_Event.mouseButton.button = static_cast<input::Mouse::Button>(SFML_Event.mouseButton.button);
                MIGHTER2D_Event.mouseButton.x = SFML_Event.mouseButton.x;
                MIGHTER2D_Event.mouseButton.y = SFML_Event.mouseButton.y;
                return MIGHTER2D_Event;
            case sf::Event::MouseMoved:
                MIGHTER2D_Event.type = SystemEvent::Type::MouseMoved;
                MIGHTER2D_Event.mouseMove.x = SFML_Event.mouseMove.x;
                MIGHTER2D_Event.mouseMove.y = SFML_Event.mouseMove.y;
                return MIGHTER2D_Event;
            case sf::Event::MouseEntered:
                MIGHTER2D_Event.type = SystemEvent::MouseEntered;
                return MIGHTER2D_Event;
            case sf::Event::MouseLeft:
                MIGHTER2D_Event.type = SystemEvent::MouseLeft;
                return MIGHTER2D_Event;
            case sf::Event::JoystickButtonPressed:
                MIGHTER2D_Event.type = SystemEvent::JoystickButtonPressed;
                MIGHTER2D_Event.joystickButton.joystickId = SFML_Event.joystickButton.joystickId;
                MIGHTER2D_Event.joystickButton.button = SFML_Event.joystickButton.button;
                return MIGHTER2D_Event;
            case sf::Event::JoystickButtonReleased:
                MIGHTER2D_Event.type = SystemEvent::JoystickButtonReleased;
                MIGHTER2D_Event.joystickButton.joystickId = SFML_Event.joystickButton.joystickId;
                MIGHTER2D_Event.joystickButton.button = SFML_Event.joystickButton.button;
                return MIGHTER2D_Event;
            case sf::Event::JoystickMoved:
                MIGHTER2D_Event.type = SystemEvent::JoystickMoved;
                MIGHTER2D_Event.joystickMove.joystickId = SFML_Event.joystickMove.joystickId;
                MIGHTER2D_Event.joystickMove.axis = static_cast<input::Joystick::Axis>(SFML_Event.joystickMove.axis);
                MIGHTER2D_Event.joystickMove.position = SFML_Event.joystickMove.position;
                return MIGHTER2D_Event;
            case sf::Event::JoystickConnected:
                MIGHTER2D_Event.type = SystemEvent::JoystickConnected;
                MIGHTER2D_Event.joystickConnect.joystickId = SFML_Event.joystickConnect.joystickId;
                return MIGHTER2D_Event;
            case sf::Event::JoystickDisconnected:
                MIGHTER2D_Event.type = SystemEvent::JoystickDisconnected;
                MIGHTER2D_Event.joystickConnect.joystickId = SFML_Event.joystickConnect.joystickId;
                return MIGHTER2D_Event;
            default:
            {
                MIGHTER2D_Event.type = SystemEvent::Unknown;
                switch (SFML_Event.type) {
                    case sf::Event::TouchBegan:
                    case sf::Event::TouchEnded:
                        if (SFML_Event.touch.finger != 0) // Multi-touch not supported
                            return MIGHTER2D_Event;

                        // Treat touch events as mouse events
                        if (SFML_Event.type == sf::Event::TouchBegan)
                            MIGHTER2D_Event.type = SystemEvent::Type::MouseButtonPressed;
                        else
                            MIGHTER2D_Event.type = SystemEvent::Type::MouseButtonReleased;

                        MIGHTER2D_Event.mouseButton.button = input::Mouse::Button::Left;
                        MIGHTER2D_Event.mouseButton.x = SFML_Event.touch.x;
                        MIGHTER2D_Event.mouseButton.y = SFML_Event.touch.y;
                        return MIGHTER2D_Event;
                    case sf::Event::TouchMoved:
                        if (SFML_Event.touch.finger != 0) // Multi-touch not supported
                            return MIGHTER2D_Event;

                        // Treat moving touch as moving mouse cursor
                        MIGHTER2D_Event.type = SystemEvent::Type::MouseMoved;
                        MIGHTER2D_Event.mouseMove.x = SFML_Event.touch.x;
                        MIGHTER2D_Event.mouseMove.y = SFML_Event.touch.y;
                        return MIGHTER2D_Event;
                    case sf::Event::SensorChanged:
                        MIGHTER2D_PRINT_WARNING(
                            "Sensors are not supported in Mighter2d v"
                            + std::to_string(MIGHTER2D_VERSION_MAJOR) + "."
                            + std::to_string(MIGHTER2D_VERSION_MINOR) + + "."
                            + std::to_string(MIGHTER2D_VERSION_PATCH))
                        break;
                    default:
                        break;
                }
                return MIGHTER2D_Event;
            }
        }
    }

    sf::Event convertToSFMLEvent(const SystemEvent &MIGHTER2D_Event) {
        sf::Event SFML_Event{};
        switch (MIGHTER2D_Event.type) {
            case SystemEvent::Closed:
                SFML_Event.type = sf::Event::EventType::Closed;
                return SFML_Event;
            case SystemEvent::Resized:
                SFML_Event.type = sf::Event::EventType::Resized;
                SFML_Event.size.width = MIGHTER2D_Event.size.width;
                SFML_Event.size.height = MIGHTER2D_Event.size.height;
                return SFML_Event;
            case SystemEvent::LostFocus:
                SFML_Event.type = sf::Event::EventType::LostFocus;
                return SFML_Event;
            case SystemEvent::GainedFocus:
                SFML_Event.type = sf::Event::EventType::GainedFocus;
                return SFML_Event;
            case SystemEvent::TextEntered:
                SFML_Event.type = sf::Event::EventType::TextEntered;
                SFML_Event.text.unicode = MIGHTER2D_Event.text.unicode;
                return SFML_Event;
            case SystemEvent::KeyReleased:
            case SystemEvent::KeyPressed:
                if (MIGHTER2D_Event.type == SystemEvent::KeyPressed)
                    SFML_Event.type = sf::Event::EventType::KeyPressed;
                else
                    SFML_Event.type = sf::Event::EventType::KeyReleased;

                SFML_Event.key.code = static_cast<sf::Keyboard::Key>(MIGHTER2D_Event.key.code);
                SFML_Event.key.alt = MIGHTER2D_Event.key.alt;
                SFML_Event.key.control = MIGHTER2D_Event.key.control;
                SFML_Event.key.shift = MIGHTER2D_Event.key.shift;
                SFML_Event.key.system = MIGHTER2D_Event.key.system;
                return SFML_Event;
            case SystemEvent::MouseWheelScrolled:
                SFML_Event.type = sf::Event::EventType::MouseWheelScrolled;
                if (MIGHTER2D_Event.mouseWheelScroll.wheel == input::Mouse::Wheel::VerticalWheel)
                    SFML_Event.mouseWheelScroll.wheel = sf::Mouse::Wheel::VerticalWheel;
                else
                    SFML_Event.mouseWheelScroll.wheel = sf::Mouse::Wheel::HorizontalWheel;

                SFML_Event.mouseWheelScroll.delta = MIGHTER2D_Event.mouseWheelScroll.delta;
                SFML_Event.mouseWheelScroll.x = MIGHTER2D_Event.mouseWheelScroll.x;
                SFML_Event.mouseWheelScroll.y = MIGHTER2D_Event.mouseWheelScroll.y;
                return SFML_Event;
            case SystemEvent::MouseButtonPressed:
            case SystemEvent::MouseButtonReleased:
                if (MIGHTER2D_Event.type == SystemEvent::MouseButtonPressed)
                    SFML_Event.type = sf::Event::EventType::MouseButtonPressed;
                else
                    SFML_Event.type = sf::Event::EventType::MouseButtonReleased;

                SFML_Event.mouseButton.button = static_cast<sf::Mouse::Button>(MIGHTER2D_Event.mouseButton.button);
                SFML_Event.mouseButton.x = MIGHTER2D_Event.mouseButton.x;
                SFML_Event.mouseButton.y = MIGHTER2D_Event.mouseButton.y;
                return SFML_Event;
            case SystemEvent::MouseMoved:
                SFML_Event.type = sf::Event::EventType::MouseMoved;
                SFML_Event.mouseMove.x = MIGHTER2D_Event.mouseMove.x;
                SFML_Event.mouseMove.y = MIGHTER2D_Event.mouseMove.y;
                return SFML_Event;
            case SystemEvent::MouseEntered:
                SFML_Event.type = sf::Event::EventType::MouseEntered;
                return SFML_Event;
            case SystemEvent::MouseLeft:
                SFML_Event.type = sf::Event::EventType::MouseLeft;
                return SFML_Event;
            case SystemEvent::JoystickButtonPressed:
                SFML_Event.type = sf::Event::JoystickButtonPressed;
                SFML_Event.joystickButton.joystickId = MIGHTER2D_Event.joystickButton.joystickId;
                SFML_Event.joystickButton.button = MIGHTER2D_Event.joystickButton.button;
                return SFML_Event;
            case SystemEvent::JoystickButtonReleased:
                SFML_Event.type = sf::Event::JoystickButtonReleased;
                SFML_Event.joystickButton.joystickId = MIGHTER2D_Event.joystickButton.joystickId;
                SFML_Event.joystickButton.button = MIGHTER2D_Event.joystickButton.button;
                return SFML_Event;
            case SystemEvent::JoystickMoved:
                SFML_Event.type = sf::Event::JoystickMoved;
                SFML_Event.joystickMove.joystickId = MIGHTER2D_Event.joystickMove.joystickId;
                SFML_Event.joystickMove.axis = static_cast<sf::Joystick::Axis>(MIGHTER2D_Event.joystickMove.axis);
                SFML_Event.joystickMove.position = MIGHTER2D_Event.joystickMove.position;
                return SFML_Event;
            case SystemEvent::JoystickConnected:
                SFML_Event.type = sf::Event::JoystickConnected;
                SFML_Event.joystickConnect.joystickId = MIGHTER2D_Event.joystickConnect.joystickId;
                return SFML_Event;
            case SystemEvent::JoystickDisconnected:
                SFML_Event.type = sf::Event::JoystickDisconnected;
                SFML_Event.joystickConnect.joystickId = MIGHTER2D_Event.joystickConnect.joystickId;
                return SFML_Event;
            default:
                return SFML_Event;
        }
    }

    sf::Color convertToSFMLColour(Colour colour) {
        return {static_cast<sf::Uint8>(colour.red),
                static_cast<sf::Uint8>(colour.green),
                static_cast<sf::Uint8>(colour.blue),
                static_cast<sf::Uint8>(colour.opacity)};
    }

    tgui::Color convertToTGUIColour(Colour color) {
        return {static_cast<std::uint8_t>(color.red),
                static_cast<std::uint8_t>(color.green),
                static_cast<std::uint8_t>(color.blue),
                static_cast<std::uint8_t>(color.opacity)};
    }

    Colour convertFrom3rdPartyColour(sf::Color thirdPartyColour) {
        return {thirdPartyColour.r, thirdPartyColour.g, thirdPartyColour.b, thirdPartyColour.a};
    }

    // The implementation of this function was adapted from the following SFML wiki: https://github.com/SFML/SFML/wiki/Source:-Letterbox-effect-using-a-view
    sf::View letterbox(const sf::View &view, unsigned int windowWidth, unsigned int windowHeight) {
        float windowRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
        float viewRatio = view.getSize().x / view.getSize().y;
        bool horizontalSpacing = windowRatio > viewRatio;
        float sizeX = 1;
        float sizeY = 1;
        float posX = 0;
        float posY = 0;

        if (horizontalSpacing) { // Place black bars on the left and right side of the view
            sizeX = viewRatio / windowRatio;
            posX = (1 - sizeX) / 2.f;
        } else { // Place black bars at the top and bottom of the view
            sizeY = windowRatio / viewRatio;
            posY = (1 - sizeY) / 2.f;
        }

        sf::View letterboxView = view;
        letterboxView.setViewport( sf::FloatRect(posX, posY, sizeX, sizeY) );

        return letterboxView;
    }

    float pixelsToMetres(float pixels) {
        return pixels / PIXELS_PER_METRE;
    }

    Vector2f pixelsToMetres(Vector2f pixels) {
        return {pixelsToMetres(pixels.x), pixelsToMetres(pixels.y)};
    }

    float metresToPixels(float metres) {
        return metres * PIXELS_PER_METRE;
    }

    Vector2f metresToPixels(Vector2f metres) {
        return {metresToPixels(metres.x), metresToPixels(metres.y)};
    }

    float radToDeg(float rad) {
        return (180.0f * rad) / PI;
    }

    float degToRad(float deg) {
        return (PI * deg) / 180.0f;
    }

    tgui::Font get_TGUI_Font(const std::string &filename) {
        auto tguiBackendFont = std::make_shared<tgui::BackendFontSFML>();
        tguiBackendFont->getInternalFont() = mighter2d::ResourceManager::getInstance()->getFont(filename);
        auto tguiFont = tgui::Font(std::move(tguiBackendFont), filename);
        return tguiFont;
    }

    ui::Widget* findRecursively(const std::unordered_map<std::string,
        std::unique_ptr<ui::Widget>>& widgets, const std::string &widgetName)
    {
        for (const auto& widget : widgets) {
            if (widget.second->isContainer()) {
                auto* container = static_cast<ui::WidgetContainer*>(widget.second.get());
                auto* widgetInContainer = container->getWidget(widgetName);
                if (widgetInContainer)
                    return widgetInContainer;
            } else if (auto* tabsContainer = dynamic_cast<ui::TabsContainer*>(widget.second.get()); tabsContainer) {
                auto* widgetsInTabs = tabsContainer->getWidget(widgetName);

                if (widgetsInTabs)
                    return widgetsInTabs;
            }
        }

        return nullptr;
    }

    bool removeRecursively(const std::unordered_map<std::string,
        std::unique_ptr<ui::Widget>>& widgets, const std::string &widgetName)
    {
        for (const auto& widget : widgets) {
            if (widget.second->isContainer()) {
                auto* container = static_cast<ui::WidgetContainer*>(widget.second.get());
                if (container->removeWidget(widgetName))
                    return true;
                else continue;
            }
        }

        return false;
    }
}