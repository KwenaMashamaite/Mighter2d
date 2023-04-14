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

#include "Mighter2d/core/scene/GridMoverContainer.h"
#include "Mighter2d/core/physics/KeyboardGridMover.h"
#include "Mighter2d/core/physics/TargetGridMover.h"

namespace mighter2d {
    void GridMoverContainer::update(Time deltaTime) {
        forEach([&deltaTime](GridMover* gridMover) {
            gridMover->update(deltaTime);
        });
    }

    void GridMoverContainer::handleEvent(Event event) {
        forEach([&event](GridMover* gridMover) {
            if (gridMover->getType() == GridMover::Type::KeyboardControlled)
                static_cast<KeyboardGridMover*>(gridMover)->handleEvent(event);
        });
    }

    void GridMoverContainer::render(priv::RenderTarget &window) const {
        forEach([&window](GridMover* gridMover) {
            if (gridMover->getType() == GridMover::Type::Target)
                static_cast<TargetGridMover*>(gridMover)->renderPath(window);
        });
    }
}
