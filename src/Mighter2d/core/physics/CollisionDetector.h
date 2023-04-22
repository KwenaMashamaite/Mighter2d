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

#ifndef MIGHTER2D_COLLISIONDETECTOR_H
#define MIGHTER2D_COLLISIONDETECTOR_H

#include "Mighter2d/core/physics/BoundingBox.h"

namespace mighter2d::priv {
    /**
     * @brief Checks if a collision exists or not
     */
    class CollisionDetector {
    public:
        /**
         * @brief Check if two bounding boxes are overlapping or not
         * @param boundingBoxA The first bounding box
         * @param boundingBoxB The second bounding box
         * @return True if the two bounding boxes are overlapping, otherwise false
         */
        static bool isColliding(const BoundingBox& boundingBoxA, const BoundingBox& boundingBoxB);

        /**
         * @brief Get the Union over Intersection (IoU) of the two bounding boxes
         * @param boundingBoxA The first bounding box
         * @param boundingBoxB The second bounding box
         * @return The IoU in the range [0, 1]
         *
         * The IoU specifies by how much the two bounding boxes overlap. 0 = no overlap, 0.5 = 50% overlap
         * and 1 = 100% overlap
         */
        static float getIoU(const BoundingBox& boundingBoxA, const BoundingBox& boundingBoxB);
    };
}

#endif