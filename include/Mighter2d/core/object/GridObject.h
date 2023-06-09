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

#ifndef MIGHTER2D_GRIDOBJECT_H
#define MIGHTER2D_GRIDOBJECT_H

#include "Mighter2d/Config.h"
#include "Mighter2d/core/object/GameObject.h"
#include "Mighter2d/core/object/CollisionExcludeList.h"
#include "Mighter2d/core/grid/Grid.h"
#include "Mighter2d/core/physics/Collidable.h"
#include <memory>

namespace mighter2d {
    class GridMover;

    /**
     * @brief An object that can be placed in a Grid
     */
    class MIGHTER2D_API GridObject : public GameObject, public Collidable {
    public:
        using Ptr = std::unique_ptr<GridObject>;     //!< Unique game object pointer

        /**
         * @brief Constructor
         * @param scene The scene this game object belongs to
         */
        explicit GridObject(Scene& scene);

        /**
         * @brief Copy constructor
         */
        GridObject(const GridObject&);

        /**
         * @brief Copy assignment operator
         */
        GridObject& operator=(const GridObject&);

        /**
         * @brief Move constructor
         */
        GridObject(GridObject&&) noexcept = default;

        /**
         * @brief Move assignment operator
         */
        GridObject& operator=(GridObject&&) noexcept = default;

        /**
         * @brief Create a new GridObject instance
         * @param scene The scene the object belongs to
         * @return The created instance
         */
        static GridObject::Ptr create(Scene& scene);

        /**
         * @brief Create a copy of the game object
         * @return A new game object
         */
        GridObject::Ptr copy() const;

        /**
         * @brief Swap the object with another game object
         * @param other The object to swap with this game object
         */
        void swap(GridObject& other);

        /**
         * @brief Get the name of the class the game object is instantiated from
         * @return The name of the concrete class the game object is instantiated
         *         from
         *
         * Note that this function must be overridden further if this class
         * is extended, otherwise it will return the name of this class
         * instead of your class name
         *
         * @see getBaseClassName
         */
        std::string getClassName() const override;

        /**
         * @brief Get the objects bounding box
         * @return The objects bounding box
         */
        const BoundingBox &getBoundingBox() const override;

        /**
         * @brief Set the direction of the object
         * @param dir The direction to be set
         *
         * By default the direction is Vector2i{0, 0}
         *
         * @see getDirection
         */
        void setDirection(const Vector2i& dir);

        /**
         * @brief Get the objects current direction
         * @return The objects direction
         */
        const Vector2i& getDirection() const;

        /**
         * @brief Set whether or not the game object is an obstacle
         * @param isObstacle True if it is an obstacle, otherwise false
         *
         * When the object is an obstacle and its active state is @a true
         * (see mighter2d::GameObject::setActive), then other game objects cannot
         * overlap with it. A collision event will be raised when another
         * game object attempts to occupy the same cell as the obstacle.
         * However, the two objects will never overlap
         *
         * By default, the game object is not an obstacle
         *
         * @see isObstacle
         */
        void setObstacle(bool isObstacle);

        /**
         * @brief Check if the object is an obstacle or not
         * @return True if the object is an obstacle, otherwise false
         *
         * @see setObstacle
         */
        bool isObstacle() const;

        /**
         * @brief Set the speed of the game object
         * @param speed The new speed
         *
         * By default, the speed is mighter2d::Vector2f{0, 0}
         *
         * @see getSpeed
         */
        void setSpeed(const Vector2f& speed);

        /**
         * @brief Get the speed of the target
         * @return The speed of the target
         *
         * @see setSpeed
         */
        const Vector2f& getSpeed() const;

        /**
         * @brief Check if target is moving or not
         * @return True if target is moving otherwise false
         *
         * @see getTarget
         */
        bool isMoving() const;

        /**
         * @brief Get the index of the grid tile currently occupied by the object
         * @return The index of the grid tile currently occupied by the object
         *
         * Note that when the object is moved by a GridMover, it occupies a tile
         * ahead of time, see mighter2d::GridMover::requestMove()
         *
         * This function returns mighter2d::Index{-1, -1} if the object is not in a Grid
         *
         * @see getPrevTileIndex
         */
        Index getGridIndex() const;

        /**
         * @brief Get access to the grid in which the target is moved in
         * @return The grid in which the target is being moved in
         */
        Grid* getGrid();
        const Grid* getGrid() const;

        /**
         * @brief Get the game objects obstacle collision filter
         * @return The game objects obstacle collision filter
         *
         * By default, any object that @e cannot collide with an obstacle game
         * object will pass through it without generating a collision event
         * whereas if they @e can collide, the two objects will never overlap.
         * Sometimes you may want an obstacle object to exhibit the default
         * behavior for some objects but allow others to pass through it and
         * still generate a collision event. This list helps you achieve that.
         * The collision groups of game objects added to this list will pass
         * over an obstacle game object but generate a collision event
         *
         * @note This function is useful only if the game object is an obstacle
         * (see setObstacle()).
         *
         * @see getCollisionExcludeList
         */
        CollisionExcludeList& getObstacleCollisionFilter();
        const CollisionExcludeList& getObstacleCollisionFilter() const;

        /**
         * @brief Get the objects grid mover
         * @return The objects grid mover or a nullptr if the object is not
         *         being controlled by a grid mover
         *
         * @see mighter2d::GridMover
         */
        GridMover* getGridMover();
        const GridMover* getGridMover() const;

        /**
         * @brief Add an event listener to a grid enter event
         * @param callback The function to be executed when the game object
         *                 is added to a Grid
         * @param oneTime True to execute the callback one-time or false to
         *                execute it every time the event is triggered
         * @return The event listeners unique identification number
         *
         * The callback is passed this game object on invocation
         *
         * @see onGridExit
         */
        int onGridEnter(const Callback<GridObject*>& callback, bool oneTime = false);

        /**
         * @brief Add an event listener to a grid exit event
         * @param callback The function to be executed when the game object
         *                 is removed from a Grid
         * @param oneTime True to execute the callback one-time or false to
         *                execute it every time the event is triggered
         * @return The event listeners unique identification number
         *
         * The callback is passed this game object on invocation
         *
         * @see onGridEnter
         */
        int onGridExit(const Callback<GridObject*>& callback, bool oneTime = false);

        /**
         * @brief Add an event listener to a move begin event
         * @param callback The function to be executed when the game object
         *                 starts moving
         * @param oneTime True to execute the callback one-time or false to
         *                execute it every time the event is triggered
         * @return The event listeners unique identification number
         *
         * This event is emitted when the game object starts moving from its
         * current tile to one of its adjacent tile. The callback is passed
         * the index of the tile that the game object is currently moving to
         *
         * @note When controlled by a grid mover, the game object will always
         * move one tile at a time, regardless of how fast it's moving
         *
         * The callback is passed this game object on invocation
         *
         * @see onGridMoveEnd
         */
        int onGridMoveBegin(const Callback<GridObject*>& callback, bool oneTime = false);

        /**
         * @brief Add an event listener to a pre move event
         * @param callback The function to be executed before the object is
         *                 moved in the grid
         * @param oneTime True to execute the callback one-time or false to
         *                execute it every time the event is triggered
         * @return The event listeners unique identification number
         *
         * This event is fired every time the object is about to move in the
         * grid. The difference between onGridMoveBegin() and onGridPreMove()
         * is that the former is called once per tile movement and the latter
         * is called every time the objects position is about to be updated
         * while in transit to its target tile. In other words, onGridMoveBegin()
         * is called at the beginning of the tile movement and onGridPreMove()
         * is called until the object completes its tile movement.
         *
         * You can add as many event listeners as you want to this event.
         * Furthermore, the callback is passed this game object on invocation
         *
         * @note This event is only fired if the object does not have a
         * RigidBody attached to it. see GameObject::attachRigidBody
         *
         * @see onGridMove
         */
        int onGridPreMove(const Callback<GridObject*>& callback, bool oneTime = false);

        /**
         * @brief Add an event listener to a grid move event
         * @param callback The function to be executed after the object moves
         *                  in the grid
         * @param oneTime True to execute the callback one-time or false to
         *                execute it every time the event is triggered
         * @return The event listeners unique identification number
         *
         * This event is fired every time after the object moves in the grid.
         * The difference between onGridMoveEnd() and onGridPostMove() is that
         * the former is called once at the end of the tile movement and the
         * latter is called every time after the objects position is updated
         * while in transit to its target tile.
         *
         * You can add as many event listeners as you want to this event.
         * Furthermore, the callback is passed this game object on invocation
         *
         * @note This event is only fired if the object does not have a
         * RigidBody attached to it. see GameObject::attachRigidBody
         *
         * @see onGridPreMove, onGridMoveBegin, onGridMoveEnd
         */
        int onGridPostMove(const Callback<GridObject*>& callback, bool oneTime = false);

        /**
         * @brief Add an event listener to an adjacent tile reached event
         * @param callback Function to execute when the target reaches its
         *        target tile
         * @param oneTime True to execute the callback one-time or false to
         *                execute it every time the event is triggered
         * @return The event listeners identification number
         *
         * This event is emitted when the target moves from its current tile
         * to any of its adjacent tiles.
         *
         * @note When controlled by a grid mover, the target will always move
         * one tile at a time, regardless of how fast the target is moving
         *
         * The callback is passed this game object on invocation
         *
         * @see onGridMoveBegin
         */
        int onGridMoveEnd(const Callback<GridObject*>& callback, bool oneTime = false);

        /**
         * @brief Add an event listener to a grid object collision event
         * @param callback Function to be executed when this game object
         *                 collides with another game object in a Grid
         * @param oneTime True to execute the callback one-time or false to
         *                execute it every time the event is triggered
         * @return The event listeners unique identification number
         *
         * This event is triggered when this grid object collides with another
         * grid object or vice versa. A collision is registered between two
         * grid objects when the occupy the same Grid tile
         *
         * The callback is passed this game object and the game object it is
         * in collision with respectively.
         *
         * @see removeCollisionListener
         */
        int onGridObjectCollision(const Callback<GridObject*, GridObject*>& callback, bool oneTime = false);

        /**
         * @brief Add an event listener to a grid border collision event
         * @param callback Function to execute when the collision takes place
         * @param oneTime True to execute the callback one-time or false to
         *                execute it every time the event is triggered
         * @return The event listeners identification number
         *
         * This event is emitted when the target tries to go beyond the bounds
         * of the grid. By default the event is handled internally before it's
         * emitted to the outside. The internal handler prevents the target
         * from leaving the grid. That is, the target will occupy the same tile
         * it occupied before the collision. This behaviour is not removable,
         * however, it may be overridden since the internal handler is called
         * first before alerting external handlers
         *
         * The callback is passed this object on invocation
         *
         * @see unsubscribe
         */
        int onGridBorderCollision(const Callback<GridObject*>& callback, bool oneTime = false);

        /**
         * @brief Add an event listener to a grid tile collision event
         * @param callback Function to execute when the collision takes place
         * @param oneTime True to execute the callback one-time or false to
         *                execute it every time the event is triggered
         * @return The event listeners identification number
         *
         * This event is emitted when the object collides with a grid tile
         * By default, the event is handled internally before its emitted. The
         * internal handler prevents the object from occupying the tile by moving
         * it back to its previous tile. This behaviour is not removable, however,
         * it may be overridden since the internal handler is called first before
         * alerting external handlers
         *
         * The callback is passed this object and the index of the tile it tried to
         * occupy on invocation
         *
         * @see unsubscribe
         */
        int onGridTileCollision(const Callback<GridObject*, Index>& callback, bool oneTime = false);

        /**
         * @internal
         * @brief Set the game object's grid mover
         * @param gridMover The grid mover to be set
         *
         * @warning This function is intended for internal use only and should
         * never be called outside of Mighter2d
         */
        void setGridMover(GridMover* gridMover);

        /**
         * @internal
         * @brief Set the grid
         * @param grid The grid the object is in
         *
         * @warning This function is intended for internal use only and should
         * never be called outside of Mighter2d
         */
        void setGrid(Grid* grid);

        /**
         * @internal
         * @brief Emit a collision event
         * @param property Event arguments
         *
         * @warning This function is intended for internal use only and should
         * never be called outside of Mighter2d
         */
        void emitGridEvent(const Property& property);

        /**
         * @brief Destructor
         */
        ~GridObject() override;

    private:
        Grid* grid_;                         //!< The grid the object is in
        bool isObstacle_;                   //!< A flag indicating whether or not the object is an obstacle
        Vector2i direction_;                //!< The current direction of the object
        Vector2f speed_;                    //!< The speed of the game object
        CollisionExcludeList obstacleColFilter_;     //!< Stores the collision groups of game objects that can collide with an obstacle without being blocked
        GridMover* gridMover_;              //!< The objects grid mover
    };
}

#endif //MIGHTER2D_GRIDOBJECT_H
