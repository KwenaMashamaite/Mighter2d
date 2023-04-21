// ///////////////////////////////////////////////////////////////////////////
//  Mighter2d
//
//  Copyright (c) 2023 Kwena Mashamaite
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
// ///////////////////////////////////////////////////////////////////////////

#ifndef MIGHTER2D_BOUNDINGBOX_H
#define MIGHTER2D_BOUNDINGBOX_H

#include "Mighter2d/Config.h"
#include "Mighter2d/common/Vector2.h"

namespace mighter2d {
    /**
     * @brief A rectangular axis-aligned box that outlines an object
     *
     * A bounding box is primarily used for collision detection
     */
    class MIGHTER2D_API BoundingBox {
    public:
        /**
         * @brief Constructor
         * @param pos The position of the bounding box
         * @param size The size of the bounding box
         */
        explicit BoundingBox(const Vector2f& pos = {0, 0}, const Vector2f& size = {0, 0});

        /**
         * @brief Set the position of the bounding box
         * @param position The position to be set
         *
         * @see getPosition
         */
        void setPosition(const Vector2f& position);

        /**
         * @brief Get the position of the bounding box
         * @return The position of the bounding box
         *
         * @see setPosition
         */
        const Vector2f& getPosition() const;

        /**
         * @brief Set the size of the bounding box
         * @param size The size to be set
         *
         * @see getSize
         */
        void setSize(const Vector2f& size);

        /**
         * @brief Get the size of the bounding box
         * @return The size of the bounding box
         *
         * @see setSize
         */
        const Vector2f& getSize() const;

    private:
        Vector2f pos_;    //!< The position of the bounding box
        Vector2f size_;   //!< The size of the bounding box
    };
}

#endif //MIGHTER2D_BOUNDINGBOX_H
