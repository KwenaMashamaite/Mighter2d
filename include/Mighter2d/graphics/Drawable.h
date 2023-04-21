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

#ifndef MIGHTER2D_DRAWABLE_H
#define MIGHTER2D_DRAWABLE_H

#include "Mighter2d/Config.h"
#include "Mighter2d/core/object/Object.h"

namespace mighter2d {
    class Scene;

    /// @internal
    namespace priv {
        class RenderTarget;
    }

    /**
     * @brief Class for objects drawable on a widow
     */
    class MIGHTER2D_API Drawable : public Object {
    public:
        /**
         * @brief Constructor
         * @param scene The scene the drawable belongs to
         */
        explicit Drawable(Scene& scene);

        /**
         * @brief Set the drawables render layer
         * @param renderLayer The render layer to set
         * @param renderOrder The render order of the drawable
         */
        void setRenderLayer(const std::string& renderLayer, int renderOrder = 0);

        /**
         * @brief Get the render layer
         * @return The render layer
         */
        std::string getRenderLayer() const;

        /**
         * @brief Get the render order
         * @return The render order
         */
        unsigned int getRenderOrder() const;

        /**
         * @brief Show or hide the drawable
         * @param visible True to show or false to hide
         *
         * When hidden the drawable will not be shown on the render target
         *
         * By default, the drawable is visible
         *
         * @see isVisible
         */
        void setVisible(bool visible);

        /**
         * @brief Check whether or not the drawable is visible
         * @return True if visible, otherwise false
         *
         * @see setVisible
         */
        bool isVisible() const;

        /**
         * @brief Toggle the visibility of the drawable
         *
         * This function will hide the sprite if its currently
         * visible or show it if it is currently hidden
         *
         * @see setVisible
         */
        void toggleVisibility();

        /**
         * @brief Draw object on a render target
         * @param renderTarget Target to draw object on
         *
         * @note Note that this function will be called automatically
         * by the scene the drawable belongs to
         */
        virtual void draw(priv::RenderTarget &renderTarget) const = 0;

        /**
         * @brief Get the name of this class
         * @return The name of this class
         *
         * Note that this function is only overridden by child classes
         * of Object which also serve as a base class for other classes
         *
         * @see Object::getBaseClassName and Object::getClassName
         */
        std::string getBaseClassName() const override;

        /**
         * @brief Destructor
         */
        ~Drawable() override;

    private:
        Scene* scene_;
        std::string renderLayer_;
        int renderOrder_;
        bool isVisible_;
    };
}

#endif // MIGHTER2D_DRAWABLE_H
