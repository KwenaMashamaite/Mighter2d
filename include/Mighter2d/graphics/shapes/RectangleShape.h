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

#ifndef MIGHTER2D_RECTANGLESHAPE_H
#define MIGHTER2D_RECTANGLESHAPE_H

#include "Mighter2d/Config.h"
#include "Mighter2d/graphics/shapes/Shape.h"
#include <memory>

namespace mighter2d {
    /**
     * @brief A 2D shape having four sides and four corners (90 degree angles)
     */
    class MIGHTER2D_API RectangleShape : public Shape {
    public:
        using Ptr = std::unique_ptr<RectangleShape>; //!< Shared shape pointer

        /**
         * @brief Default constructor
         * @param size The size of the rectangle
         * @param scene The scene the shape belongs to
         */
        explicit RectangleShape(Scene& scene, const Vector2f& size = Vector2f(0, 0));

        /**
         * @brief Copy constructor
         */
        RectangleShape(const RectangleShape&);

        /**
         * @brief Copy assignment operator
         */
        RectangleShape& operator=(const RectangleShape&);

        /**
         * @brief Move constructor
         */
        RectangleShape(RectangleShape&&) noexcept;

        /**
         * @brief Move assignment operator
         */
        RectangleShape& operator=(RectangleShape&&) noexcept;

        /**
         * @brief Create a new rectangle shape
         * @param size The size of the rectangle
         * @param scene The scene the shape belongs to
         * @return The created rectangle shape
         */
        static RectangleShape::Ptr create(Scene& scene, const Vector2f& size = Vector2f(0, 0));

        /**
         * @brief Create a copy of this shape
         * @return A copy of this shape
         *
         * @see clone
         */
        RectangleShape::Ptr copy() const;

        /**
         * @brief Make a copy of this shape
         * @return A copy of this shape
         *
         * You should use this function if you don't care about the type
         * of the shape, otherwise use the shapes copy function
         *
         * @see copy
         */
        Shape::Ptr clone() const override;

        /**
         * @brief Get the name of this class
         * @return The name of this class
         */
        std::string getClassName() const override;

        /**
         * @brief Set the size of the rectangle
         * @param size The new size of the rectangle
         */
        void setSize(const Vector2f& size);

        /**
         * @brief Get the size of th rectangle
         * @return The size of the rectangle
         */
        Vector2f getSize() const;

        /**
         * @brief Destructor
         */
        ~RectangleShape() override;

    private:
        struct RecShapeImpl;
        std::unique_ptr<RecShapeImpl> pimpl_;
    };
}

#endif //MIGHTER2D_RECTANGLESHAPE_H
