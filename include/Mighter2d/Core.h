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

#ifndef MIGHTER2D_CORE_H
#define MIGHTER2D_CORE_H

////////////////////////////////////////////////////////////////////////////////
// Includes all the core headers
////////////////////////////////////////////////////////////////////////////////

#include "Mighter2d/common/Vector2.h"
#include "Mighter2d/common/Rect.h"
#include "Mighter2d/common/Transform.h"
#include "Mighter2d/common/ITransformable.h"
#include "Mighter2d/common/ITransformable.h"
#include "Mighter2d/common/Property.h"
#include "Mighter2d/common/PropertyContainer.h"
#include "Mighter2d/core/animation/Animation.h"
#include "Mighter2d/core/animation/Animator.h"
#include "Mighter2d/core/audio/SoundEffect.h"
#include "Mighter2d/core/audio/Music.h"
#include "Mighter2d/core/object/GameObject.h"
#include "Mighter2d/core/event/Event.h"
#include "Mighter2d/core/event/EventEmitter.h"
#include "Mighter2d/core/event/GlobalEventEmitter.h"
#include "Mighter2d/core/exceptions/Exceptions.h"
#include "Mighter2d/core/input/InputManager.h"
#include "Mighter2d/core/input/Mouse.h"
#include "Mighter2d/core/input/Keyboard.h"
#include "Mighter2d/core/input/Joystick.h"
#include "Mighter2d/core/engine/Engine.h"
#include "Mighter2d/core/physics/grid/path/BFS.h"
#include "Mighter2d/core/physics/grid/path/DFS.h"
#include "Mighter2d/core/physics/GridMover.h"
#include "Mighter2d/core/physics/KeyboardGridMover.h"
#include "Mighter2d/core/physics/RandomGridMover.h"
#include "Mighter2d/core/physics/TargetGridMover.h"
#include "Mighter2d/core/physics/CyclicGridMover.h"
#include "Mighter2d/core/resources/ResourceLoader.h"
#include "Mighter2d/core/scene/Scene.h"
#include "Mighter2d/core/scene/DrawableContainer.h"
#include "Mighter2d/core/scene/GameObjectContainer.h"
#include "Mighter2d/core/grid/Index.h"
#include "Mighter2d/core/grid/Grid.h"
#include "Mighter2d/core/time/Clock.h"
#include "Mighter2d/core/time/Time.h"
#include "Mighter2d/core/time/Timer.h"
#include "Mighter2d/core/time/TimerManager.h"

#endif //MIGHTER2D_CORE_H
