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

#include "Mighter2d/core/physics/GridMover.h"
#include "Mighter2d/utility/Helpers.h"
#include "Mighter2d/core/scene/Scene.h"
#include <string_view>

using namespace std::string_literals;

namespace mighter2d {
    namespace {
        bool isSupportedDirection(const Direction& dir) {
            return dir == Left || dir == UpLeft || dir == Up || dir == UpRight
                || dir == Right ||dir == DownRight || dir == Down || dir == DownLeft;
        }
    }

    GridMover::GridMover(Type type, Grid &grid, GridObject* target) :
        type_{type},
        grid_(grid),
        scene_(&grid.getScene()),
        target_{nullptr},
        maxSpeed_{Vector2f{60.f, 60.f}},
        speedMultiplier_{1.0f},
        targetDirection_{Unknown},
        targetTile_{nullptr},
        prevTile_{nullptr},
        isMoving_{false},
        isMoveFrozen_{false},
        moveRestrict_{MoveRestriction::None},
        targetDestructionId_{-1},
        targetPropertyChangeId_{-1}
    {
        setTarget(target);

        scene_->addUpdatable(this);

        onDestruction([this] {
            scene_->removeUpdatable(this);
        });
    }

    GridMover::GridMover(Grid& grid, GridObject* gameObject) :
        GridMover(Type::Manual, grid, gameObject)
    {}

    GridMover::Ptr GridMover::create(Grid &grid, GridObject *gameObject) {
        return std::make_unique<GridMover>(grid, gameObject);
    }

    Scene &GridMover::getScene() {
        return *scene_;
    }

    Scene &GridMover::getScene() const {
        return *scene_;
    }

    std::string GridMover::getBaseClassName() const {
        return "GridMover";
    }

    std::string GridMover::getClassName() const {
        return "GridMover";
    }

    void GridMover::syncWith(const GridMover &other) {
        if (this != &other && other.isMoving_) {
            isMoving_ = true;
            isMoveFrozen_ = other.isMoveFrozen_;
            targetDirection_ = other.targetDirection_;
            currentDirection_ = other.currentDirection_;
            prevDirection_ = other.prevDirection_;
            maxSpeed_ = other.maxSpeed_;
            speedMultiplier_ = other.speedMultiplier_;
            targetTile_ = &getGrid().getTile(other.targetTile_->getIndex());
            prevTile_ = &getGrid().getTile(other.prevTile_->getIndex());
        }
    }

    void GridMover::setTarget(GridObject* target) {
        if (target_ == target)
            return;
        else if (target) {
            MIGHTER2D_ASSERT(!target->getGridMover(), "A game object can only be controlled by one grid mover at a time, call setTarget(nullptr) on the current grid mover")
            MIGHTER2D_ASSERT(grid_.hasChild(target), "The game object must already be in the grid/grid before adding it to a grid mover")

            if (target_) {
                target_->removeDestructionListener(targetDestructionId_);
                target_->removeEventListener(targetPropertyChangeId_);
                targetDestructionId_ = targetPropertyChangeId_ = -1;
                teleportTargetToDestination();
                target_->setGridMover(nullptr);
            }

            targetDestructionId_ = target->onDestruction([this] {
                setTarget(nullptr);
                targetDestructionId_ = targetPropertyChangeId_ = -1;
            });

            targetPropertyChangeId_ = target->onPropertyChange([this](const Property& property) {
                if (property.getName() == "speed") {
                    maxSpeed_ = property.getValue<Vector2f>();
                }
            });

            if (moveRestrict_ == MoveRestriction::None || moveRestrict_ == MoveRestriction::Diagonal) {
                MIGHTER2D_ASSERT(maxSpeed_.x == maxSpeed_.y, "Cannot have different x and y linear speeds if target can move diagonally")
            }

            prevTile_ = targetTile_ = &grid_.getTile(target->getTransform().getPosition());
            target_ = target;
            target_->setGridMover(this);
        } else { // Detaching the target from the grid mover
            if (target_)
                target_->setGridMover(nullptr);

            target_ = target;
        }

        emitChange(Property{"target", target_});
    }

    GridMover::Type GridMover::getType() const {
        return type_;
    }

    GridObject* GridMover::getTarget() const {
        return target_;
    }

    void GridMover::setSpeed(const Vector2f& speed) {
        if (maxSpeed_.x == std::abs(speed.x) && maxSpeed_.y == std::abs(speed.y))
            return;

        if (moveRestrict_ == MoveRestriction::None || moveRestrict_ == MoveRestriction::Diagonal) {
            MIGHTER2D_ASSERT(maxSpeed_.x == maxSpeed_.y, "Cannot have different x and y linear speeds if target can move diagonally")
        }

        maxSpeed_ = {std::abs(speed.x), std::abs(speed.y)};
        emitChange(Property{"maxLinearSpeed", speed});
    }

    const Vector2f& GridMover::getSpeed() const {
        return maxSpeed_;
    }

    void GridMover::setSpeedMultiplier(float multiplier) {
        if (multiplier >= 0.0f && speedMultiplier_ != multiplier) {
            speedMultiplier_ = multiplier;
            emitChange(Property{"speedMultiplier", speedMultiplier_});
        }
    }

    float GridMover::getSpeedMultiplier() const {
        return speedMultiplier_;
    }

    void GridMover::setMovementRestriction(GridMover::MoveRestriction moveRestriction) {
        if (moveRestrict_ == moveRestriction)
            return;

        if (type_ == Type::Target && !(moveRestriction == MoveRestriction::All || moveRestriction == MoveRestriction::None)) {
            MIGHTER2D_PRINT_WARNING("Cannot set movement restriction for a TargetGridMover, movement depends on path finder algorithm")
            return;
        }

        if (moveRestriction == MoveRestriction::None || moveRestriction == MoveRestriction::Diagonal) {
            MIGHTER2D_ASSERT(maxSpeed_.x == maxSpeed_.y, "Cannot have different x and y linear speeds if target can move diagonally")
        }

        moveRestrict_ = moveRestriction;
        emitChange(Property{"movementRestriction", moveRestrict_});
    }

    GridMover::MoveRestriction GridMover::getMovementRestriction() const {
        return moveRestrict_;
    }

    void GridMover::setMovementFreeze(bool freeze) {
        if (isMoveFrozen_ != freeze) {
            isMoveFrozen_ = freeze;

            emitChange(Property{"movementFreeze", isMoveFrozen_});
        }
    }

    bool GridMover::isMovementFrozen() const {
        return isMoveFrozen_;
    }

    Index GridMover::getCurrentTileIndex() const {
        return targetTile_->getIndex();
    }

    Index GridMover::getPrevTileIndex() const {
        return prevTile_->getIndex();
    }

    Grid &GridMover::getGrid() {
        return grid_;
    }

    const Grid &GridMover::getGrid() const {
        return grid_;
    }

    bool GridMover::isTargetMoving() const {
        return isMoving_;
    }

    bool GridMover::requestMove(const Direction &dir) {
        MIGHTER2D_ASSERT(target_, "requestDirectionChange called on a grid mover without a target, call setTarget first")

        if (!isMoveValid(dir))
            return false;

        if (!isTargetMoving() && targetDirection_ == Unknown) {
            targetDirection_ = dir;
            emit("GridMover_directionChange", targetDirection_);
            target_->setDirection(dir);

            return true;
        }

        return false;
    }

    Direction GridMover::getDirection() const {
        return currentDirection_;
    }

    Direction GridMover::getPrevDirection() const {
        return prevDirection_;
    }

    std::pair<bool, GridObject*> GridMover::isBlockedInDirection(const Direction &dir) const {
        MIGHTER2D_ASSERT(dir.x >= -1 && dir.x <= 1, "Invalid x direction, value must be -1, 0, or 1")
        MIGHTER2D_ASSERT(dir.y >= -1 && dir.y <= 1, "Invalid y Direction, value must be -1, 0, or 1")
        MIGHTER2D_ASSERT(!(dir.x == 0 && dir.y == 0), "Invalid direction, at least one value must be -1 or 1")

        auto [row, colm] = targetTile_->getIndex();
        if (grid_.isIndexValid(Index{row + dir.y, colm + dir.x})) {
            const Tile& adjacentTile = grid_.getTile(Index{row + dir.y, colm + dir.x});
            if (!adjacentTile.isCollidable()) {
                GridObject* obstacle = getObstacleInTile(adjacentTile);

                if (obstacle && canCollide(obstacle)) {
                    // Objects in this list generate a collision event with an obstacle without being blocked
                    // from occupying the same tile as the obstacle
                    if (obstacle->getObstacleCollisionFilter().contains(target_->getCollisionGroup()))
                        return {false, nullptr};

                    return {true, obstacle};
                } else
                    return {false, nullptr};
            }
        }

        return {true, nullptr};
    }

    void GridMover::update(Time deltaTime) {
        if (target_ && !isMoveFrozen_) {
            MIGHTER2D_ASSERT(grid_.hasChild(target_), "Target removed from the grid while still controlled by a grid mover")

            if (!isMoving_ && targetDirection_ != Unknown && maxSpeed_ != Vector2f{0.0f, 0.0f}) {
                setTargetTile();

                if (handleGridBorderCollision() || handleSolidTileCollision() || handleObstacleCollision())
                    return;

                prevDirection_ = currentDirection_;
                currentDirection_ = targetDirection_;
                isMoving_ = true;

                // Move target to target tile ahead of time
                Vector2f currentPosition = target_->getTransform().getPosition();
                grid_.changeTile(target_, targetTile_->getIndex());

                // Grid::addChild modifies the position of the target such that it's at
                // the centre of the tile, however we don't want it to teleport, we want it
                // to smoothly move there
                target_->getTransform().setPosition(currentPosition);

                emit("GridMover_moveBegin");
                target_->emitGridEvent(Property{"moveBegin"});
            }
            else if (isMoving_) {
                if (isTargetTileReached(deltaTime)) {
                    snapTargetToTargetTile();
                    onDestinationReached();
                } else {
                    emit("GridMover_preMove");
                    target_->emitGridEvent(Property("preMove"));

                    target_->getTransform().move(maxSpeed_.x * targetDirection_.x * deltaTime.asSeconds() * speedMultiplier_,
                                                 maxSpeed_.y * targetDirection_.y * deltaTime.asSeconds() * speedMultiplier_);

                    emit("GridMover_postMove");
                    target_->emitGridEvent(Property("postMove"));
                }
            }
        }
    }

    void GridMover::teleportTargetToDestination() {
        snapTargetToTargetTile();
    }

    void GridMover::snapTargetToTargetTile() {
        isMoving_ = false;
        targetDirection_ = Unknown;
        target_->getTransform().setPosition(targetTile_->getWorldCentre());
    }

    bool GridMover::isMoveValid(Direction targetDir) const {
        if (!isSupportedDirection(targetDir)) {
            MIGHTER2D_PRINT_WARNING("Direction change ignored: Target can only move in these directions: W, NW, N, NE, E, SE, S, SW")
            return false;
        } else {
            switch (moveRestrict_) {
                case MoveRestriction::None:
                    return true;
                case MoveRestriction::All:
                    return false;
                case MoveRestriction::Vertical:
                    if (targetDir != Up && targetDir != Down)
                        return false;
                    break;
                case MoveRestriction::Horizontal:
                    if (targetDir != Left && targetDir != Right)
                        return false;
                    break;
                case MoveRestriction::Diagonal:
                    if (abs(targetDir.x) != 1 || abs(targetDir.y) != 1)
                        return false;
                    break;
                case MoveRestriction::NonDiagonal:
                    if (abs(targetDir.x) == 1 && abs(targetDir.y) == 1)
                        return false;
                    break;
                default:
                    return false;
            }
        }

        return true;
    }

    bool GridMover::canCollide(GridObject* other) const {
        // Prevent Self collision
        if (other == target_)
            return false;

        // Inactive objects do not collide (collision filtering by inactivity)
        if (!(target_->isActive() && other->isActive()))
            return false;

        // Objects in excluded collision group do not collide (Collision filtering by group)
        if ((target_->getCollisionExcludeList().contains(other->getCollisionGroup())) ||
            (other->getCollisionExcludeList().contains(target_->getCollisionGroup())))
        {
            return false;
        }

        // Objects with different collision id's do not collide (collision filtering by id)
        if (target_->getCollisionId() != other->getCollisionId())
            return false;

        // Satisfies collision requirement
        return true;
    }

    bool GridMover::handleSolidTileCollision() {
        if (target_->isActive() && targetTile_->isCollidable()) {
            const Tile* hitTile = targetTile_;
            targetTile_ = prevTile_;
            targetDirection_ = Unknown;

            emit("GridMover_tileCollision", hitTile->getIndex());
            target_->emitGridEvent(Property{"tileCollision", hitTile->getIndex()});

            return true;
        }

        return false;
    }

    bool GridMover::handleObstacleCollision() {
        GridObject* obstacle = getObstacleInTile(*targetTile_);

        if (obstacle && canCollide(obstacle)) {
            // Objects in this list generate a collision event with an obstacle without being blocked
            // from occupying the same tile as the obstacle
            if (obstacle->getObstacleCollisionFilter().contains(target_->getCollisionGroup()))
                return false;

            targetTile_ = prevTile_;
            targetDirection_ = Unknown;

            emit("GridMover_objectCollision", target_, obstacle);
            target_->emitGridEvent(Property{"objectCollision", obstacle});
            obstacle->emitGridEvent(Property{"objectCollision", target_});

            return true;
        }

        return false;
    }

    GridObject* GridMover::getObstacleInTile(const Tile& tile) const {
        GridObject* obstacle = nullptr;
        grid_.forEachChildInTile(tile, [&obstacle, this](GridObject* child) {
            if (child->isObstacle() && child->isActive() && child != target_) {
                obstacle = child;
                return;
            }
        });

        return obstacle;
    }

    bool GridMover::handleGridBorderCollision() {
        //A tile outside the grid bounds has the index {-1, -1}
        if (targetTile_->getIndex().row < 0 || targetTile_->getIndex().colm < 0) {
            targetTile_ = prevTile_;
            targetDirection_ = Unknown;

            emit("GridMover_borderCollision", targetTile_->getIndex());
            target_->emitGridEvent(Property{"borderCollision"});

            return true;
        }

        return false;
    }

    bool GridMover::isTargetTileReached(Time deltaTime) {
        auto distanceToTile = target_->getTransform().getPosition().distanceTo(targetTile_->getWorldCentre());
        auto distanceMoved = maxSpeed_ * deltaTime.asSeconds() * speedMultiplier_;

        // Horizontally movement
        if (targetDirection_.x != 0 && std::fabs(distanceMoved.x) >= distanceToTile) {
            return true;
        }

        // Vertical movement
        if (targetDirection_.y != 0 && std::fabs(distanceMoved.y) >= distanceToTile)
            return true;

        return false;
    }

    void GridMover::onDestinationReached() {
        // Collide target with occupants of target tile
        grid_.forEachChildInTile(*targetTile_, [this](GridObject* gameObject) {
            if (!canCollide(gameObject))
                return;

            emit("GridMover_objectCollision", target_, gameObject);
            target_->emitGridEvent(Property{"objectCollision", gameObject});
            gameObject->emitGridEvent(Property{"objectCollision", target_});
        });

        emit("GridMover_moveEnd", targetTile_->getIndex());
        target_->emitGridEvent(Property{"moveEnd"});
    }

    void GridMover::setTargetTile() {
        prevTile_ = targetTile_;
        if (targetDirection_ == Left)
            targetTile_ = &grid_.getTileLeftOf(*targetTile_);
        else if (targetDirection_ == UpLeft)
            targetTile_ = &grid_.getTileAbove(grid_.getTileLeftOf(*targetTile_));
        else if (targetDirection_ == Up)
            targetTile_ = &grid_.getTileAbove(*targetTile_);
        else if (targetDirection_ == UpRight)
            targetTile_ = &grid_.getTileAbove(grid_.getTileRightOf(*targetTile_));
        else if (targetDirection_ == Right)
            targetTile_ = &grid_.getTileRightOf(*targetTile_);
        else if (targetDirection_ == DownRight)
            targetTile_ = &grid_.getTileBelow(grid_.getTileRightOf(*targetTile_));
        else if (targetDirection_ == Down)
            targetTile_ = &grid_.getTileBelow(*targetTile_);
        else if (targetDirection_ == DownLeft)
            targetTile_ = &grid_.getTileBelow(grid_.getTileLeftOf(*targetTile_));
    }

    void GridMover::resetTargetTile() {
        if (target_ && !isTargetMoving() && targetTile_->getIndex()
            != grid_.getTileOccupiedByChild(target_).getIndex())
        {
            targetTile_ = &grid_.getTileOccupiedByChild(target_);
            emit("GridMover_targetTileReset", targetTile_);
        }
    }

    int GridMover::onDirectionChange(const Callback<Direction> &callback, bool oneTime) {
        return utility::addEventListener(*this, "GridMover_directionChange", callback, oneTime);
    }

    int GridMover::onMoveBegin(const Callback<Index> &callback, bool oneTime) {
        return utility::addEventListener(*this, "GridMover_moveBegin", callback, oneTime);
    }

    int GridMover::onMoveEnd(const Callback<Index> &callback, bool oneTime) {
        return utility::addEventListener(*this, "GridMover_moveEnd", callback, oneTime);
    }

    int GridMover::onObjectCollision(const Callback<GridObject *, GridObject *> &callback, bool oneTime) {
        return utility::addEventListener(*this, "GridMover_objectCollision", callback, oneTime);
    }

    int GridMover::onBorderCollision(const Callback<> &callback, bool oneTime) {
        return utility::addEventListener(*this, "GridMover_borderCollision", callback, oneTime);
    }

    int GridMover::onTileCollision(const Callback<Index> &callback, bool oneTime) {
        return utility::addEventListener(*this, "GridMover_tileCollision", callback, oneTime);
    }

    int GridMover::onTargetTileReset(const Callback<Index>& callback, bool oneTime) {
        return utility::addEventListener(*this, "GridMover_targetTileReset", callback, oneTime);
    }

    GridMover::~GridMover() {
        emitDestruction();
        
        if (target_) {
            if (targetDestructionId_ != -1)
                target_->removeEventListener(targetDestructionId_);

            if (targetPropertyChangeId_ != -1)
                target_->removeEventListener(targetPropertyChangeId_);

            target_->setGridMover(nullptr);
        }

        target_ = nullptr;
        prevTile_ = nullptr;
        targetTile_ = nullptr;
    }
}
