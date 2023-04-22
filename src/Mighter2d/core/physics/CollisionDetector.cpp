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

#include "CollisionDetector.h"
#include <algorithm>

namespace mighter2d::priv {
    bool CollisionDetector::isColliding(const mighter2d::BoundingBox &boundingBoxA, const mighter2d::BoundingBox &boundingBoxB) {
        if(boundingBoxA.getPosition().x < boundingBoxB.getPosition().x + boundingBoxB.getSize().x &&
            boundingBoxA.getPosition().x + boundingBoxA.getSize().x > boundingBoxB.getPosition().x &&
            boundingBoxA.getPosition().y < boundingBoxB.getPosition().y + boundingBoxB.getSize().y &&
            boundingBoxA.getPosition().y + boundingBoxA.getSize().y > boundingBoxB.getPosition().y)
        {
            return true;
        }

        return false;
    }

    float getArea(const BoundingBox& boundingBox) {
        auto [width, height] = boundingBox.getSize();
        return (width + 1) * (height + 1);
    }

    float CollisionDetector::getIoU(const BoundingBox &boundingBoxA, const BoundingBox &boundingBoxB) {
        auto [bbA_x_pos, bbA_y_pos] = boundingBoxA.getPosition();
        auto [bbA_width, bbA_height] = boundingBoxA.getSize();
        auto [bbB_x_pos, bbB_y_pos] = boundingBoxB.getPosition();
        auto [bbB_width, bbB_height] = boundingBoxB.getSize();

        // Calculate the intersection box
        float x_left = std::max(bbA_x_pos, bbB_x_pos);
        float y_top = std::max(bbA_y_pos, bbB_y_pos);
        float x_right = std::min(bbA_x_pos + bbA_width, bbB_x_pos + bbB_width);
        float y_bottom = std::min(bbA_y_pos + bbA_height, bbB_y_pos + bbB_height);

        if (x_right < x_left || y_bottom < y_top)
            return 0.0f;
        else {
            float intersection_area = (x_right - x_left + 1) * (y_bottom - y_top + 1);

            // Calculate the union box
            float union_area = getArea(boundingBoxA) + getArea(boundingBoxB) - intersection_area;

            // IoU = intersection_area / union_area
            return intersection_area / union_area;
        }
    }
}
