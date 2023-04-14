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

#include "Mighter2d/core/grid/Grid.h"
#include "Mighter2d/core/grid/GridParser.h"
#include "Mighter2d/core/resources/ResourceManager.h"
#include "Mighter2d/core/object/GridObject.h"
#include "Mighter2d/graphics/RenderTarget.h"
#include <algorithm>

namespace mighter2d {
    bool isInTile(GridObject* child, const Tile& tile) {
        if (child) {
            return tile.contains(child->getTransform().getPosition());
        }

        return false;
    }

    Grid::Grid(unsigned int tileWidth, unsigned int tileHeight, Scene& scene) :
        scene_{scene},
        tileSpacing_{1u},
        invalidTile_(scene, {0, 0}, {-1, -1}),
        backgroundTile_(scene)
    {
        invalidTile_.setIndex({-1, -1});
        mapPos_ = {0, 0};
        numOfRows_ = numOfColms_ = 0u;
        if (tileWidth <= 0)
            tileWidth = 8;

        if (tileHeight <= 0)
            tileHeight = 8;

        tileSize_ = Vector2u{tileWidth, tileHeight};

        backgroundTile_.setFillColour(renderer_.getGridLineColour());
        renderer_.onPropertyChange([this](const Property& property){
            onRenderChange(property);
        });
    }

    Scene &Grid::getScene() {
        return scene_;
    }

    const Scene &Grid::getScene() const {
        return scene_;
    }

    unsigned int Grid::getRowCount() const {
        return numOfRows_;
    }

    unsigned int Grid::getColumnCount() const {
        return numOfColms_;
    }

    GridRenderer &Grid::getRenderer() {
        return renderer_;
    }

    const GridRenderer &Grid::getRenderer() const {
        return renderer_;
    }

    const Tile& Grid::getTile(const Vector2f &position) const {
        for (auto& tileRows : tiledMap_) {
            for (auto& tile : tileRows) {
                if (tile.contains(position))
                    return tile;
            }
        }

        return invalidTile_;
    }

    const Tile &Grid::getTileAbove(const Tile &tile) const {
        return getTileAbove(tile.getIndex());
    }

    const Tile &Grid::getTileBelow(const Tile &tile) const {
        return getTileBelow(tile.getIndex());
    }

    const Tile &Grid::getTileLeftOf(const Tile &tile) const {
        return getTileLeftOf(tile.getIndex());
    }

    const Tile& Grid::getTileRightOf(const Tile &tile) const {
        return getTileRightOf(tile.getIndex());
    }

    bool Grid::isIndexValid(const Index &index) const {
        auto [row, colm] = index;
        return !(row >= static_cast<int>(numOfRows_) || row < 0 || colm >= static_cast<int>(numOfColms_) || colm < 0);
    }

    void Grid::construct(const Vector2u& size, char id) {
        for (auto i = 0u; i < size.x; ++i) {
            auto innerVector = std::vector<char>(size.y, id);
            mapData_.push_back(std::move(innerVector));
        }

        computeDimensions();
        createTiledMap();
    }

    void Grid::loadFromFile(const std::string &filename, const char& separator) {
        mapData_ = GridParser::parse(filename, separator);
        computeDimensions();
        createTiledMap();
    }

    void Grid::loadFromVector(Map map) {
        mapData_ = std::move(map);
        computeDimensions();
        createTiledMap();
    }

    void Grid::computeDimensions() {
        numOfRows_ = static_cast<unsigned int>(mapData_.size());
        numOfColms_ = static_cast<unsigned int>(mapData_[0].size());
        mapSizeInPixels_.x = numOfColms_ * tileSize_.y + (numOfColms_ + 1) * tileSpacing_;
        mapSizeInPixels_.y = numOfRows_ * tileSize_.x + (numOfRows_ + 1) * tileSpacing_;
        backgroundTile_.setSize({static_cast<float>(mapSizeInPixels_.x), static_cast<float>(mapSizeInPixels_.y)});
    }

    void Grid::setCollidable(Tile &tile, bool collidable, bool attachCollider) {
        if (tile.isCollidable() == collidable)
            return;

        tile.setCollidable(collidable);

        if (collidable)
            tile.setFillColour(renderer_.getCollidableTileColour());
        else
            tile.setFillColour(renderer_.getTileColour());
    }

    void Grid::setPosition(int x, int y) {
        mapPos_.x = static_cast<float>(x);
        mapPos_.y = static_cast<float>(y);
        backgroundTile_.setPosition(mapPos_);

        for (auto i = 0u; i < tiledMap_.size(); i++) {
            for (auto j = 0u; j < mapData_[i].size(); j++) {
                if (i == 0 && j == 0)
                    tiledMap_[i][j].setPosition(mapPos_.x + tileSpacing_, mapPos_.y + tileSpacing_);
                else if (j == 0)
                    tiledMap_[i][j].setPosition( {mapPos_.x + tileSpacing_, tiledMap_[i - 1][j].getPosition().y + tileSize_.y + tileSpacing_});
                else
                    tiledMap_[i][j].setPosition( {tiledMap_[i][j - 1].getPosition().x + tileSize_.x + tileSpacing_, tiledMap_[i][j - 1].getPosition().y});
            }
        }
    }

    Vector2f Grid::getPosition() const {
        return mapPos_;
    }

    void Grid::createTiledMap() {
        for (auto i = 0u; i < mapData_.size(); i++) {
            auto row = std::vector<Tile>{};
            for (auto j = 0u; j < mapData_[i].size(); j++) {
                row.emplace_back(Tile(scene_, tileSize_, {-99, -99}));
                if (i == 0 && j == 0)
                    row.back().setPosition(mapPos_.x + tileSpacing_, mapPos_.y + tileSpacing_);
                else if (j == 0)
                    row.back().setPosition( {mapPos_.x + tileSpacing_, tiledMap_[i - 1][j].getPosition().y + tileSize_.y + tileSpacing_});
                else
                    row.back().setPosition( {row[j - 1].getPosition().x + tileSize_.x + tileSpacing_, row[j - 1].getPosition().y});

                row.back().setId(mapData_[i][j]);
                row.back().setIndex({static_cast<int>(i), static_cast<int>(j)});
                row.back().setVisible(renderer_.isVisible());
                row.back().setFillColour(renderer_.getTileColour());
            }
            tiledMap_.push_back(std::move(row));
        }
    }

    void Grid::draw(priv::RenderTarget &renderTarget) const {
        if (renderer_.isVisible()) {
            renderTarget.draw(backgroundTile_);
            forEachTile([&renderTarget](const Tile &tile) {
                renderTarget.draw(tile);
            });
        }
    }

    void Grid::setCollidableByIndex(const Index &index, bool isCollidable, bool attachCollider) {
        if (isIndexValid(index))
            setCollidable(tiledMap_[index.row][index.colm], isCollidable, attachCollider);
    }

    void Grid::setCollidableByIndex(const std::initializer_list<Index> &locations, bool isCollidable, bool attachCollider) {
        std::for_each(locations.begin(), locations.end(), [=](const Index& index) {
            setCollidableByIndex(index, isCollidable, attachCollider);
        });
    }

    void Grid::setCollidableByIndex(Index startPos, Index endPos, bool isCollidable, bool attachCollider) {
        if (isIndexValid(startPos) && isIndexValid(endPos)){
            for (auto i = startPos.colm; i < endPos.colm; i++)
                setCollidableByIndex({startPos.row, i}, isCollidable, attachCollider);
        }
    }

    void Grid::setCollidableById(char id, bool isCollidable, bool attachCollider) {
        forEachTile_([=](Tile& tile) {
            if (tile.getId() == id)
                setCollidable(tile, isCollidable, attachCollider);
        });
    }

    void Grid::setCollidableByExclusion(char id, bool isCollidable, bool attachCollider) {
        forEachTile_([=](Tile& tile) {
            if (tile.getId() != id)
                setCollidable(tile, isCollidable, attachCollider);
        });
    }

    const Tile& Grid::getTile(const Index &index) const {
        if (isIndexValid(index))
            return tiledMap_[index.row][index.colm];

        return invalidTile_;
    }

    bool Grid::isCollidable(const Index &index) const {
        if (isIndexValid(index))
            return tiledMap_[index.row][index.colm].isCollidable();

        return false;
    }

    bool Grid::addChild(GridObject* child, const Index& index) {
        MIGHTER2D_ASSERT(child, "Child cannot be a nullptr")
        if (isIndexValid(index) && children_.insert(child).second) {
            // Automatically remove the child from the grid when it is destroyed
            destructionIds_[child->getObjectId()] = child->onDestruction([this, child] {
                destructionIds_.erase(child->getObjectId());
                children_.erase(child);
            });

            child->getTransform().setPosition(getTile(index).getWorldCentre());
            child->setGrid(this);

            return true;
        }

        return false;
    }

    bool Grid::hasChild(const GridObject* child) const {
        return std::find(children_.begin(), children_.end(), child) != children_.end();
    }

    GridObject* Grid::getChildWithId(std::size_t id) const {
        for (const auto& child : children_) {
            if (child->getObjectId() == id)
                return child;
        }

        return nullptr;
    }

    void Grid::forEachChild(const Callback<GridObject*>& callback) const {
        std::for_each(children_.begin(), children_.end(), [&callback](auto& child) {
            callback(child);
        });
    }

    void Grid::forEachChildInTile(const Tile& tile, const Callback<GridObject*>& callback) const {
        forEachChild([&](GridObject* child) {
            if (isInTile(child, tile))
                callback(child);
        });
    }

    void Grid::update(Time) {

    }

    bool Grid::removeChildWithId(std::size_t id) {
        for (auto& child : children_) {
            if (child->getObjectId() == id) {
                unsubscribeDestructionListener(child);
                GridObject* copy = child;
                children_.erase(child);
                copy->setGrid(nullptr);

                return true;
            }
        }

        return false;
    }

    bool Grid::removeChild(GridObject* child) {
        if (!child)
            return false;

        return removeChildWithId(child->getObjectId());
    }

    void Grid::removeChildIf(const std::function<bool(GridObject*)>& callback) {
        for (auto iter = children_.begin(); iter != children_.end(); ) {
            if (callback(*iter)) {
                GridObject* gameObject = *iter;
                unsubscribeDestructionListener(gameObject);
                iter = children_.erase(iter);
                gameObject->setGrid(nullptr);
            } else
                ++iter;
        }
    }

    void Grid::removeAllChildren() {
        removeChildIf([](GridObject*) {
            return true;
        });
    }

    void Grid::changeTile(GridObject* child, const Index& index) {
        if (hasChild(child) && isIndexValid(index))
            child->getTransform().setPosition(getTile(index).getWorldCentre());
    }

    void Grid::changeTile(GridObject* child, const Tile &tile) {
        changeTile(child, tile.getIndex());
    }

    Vector2u Grid::getTileSize() const {
        return tileSize_;
    }

    void Grid::forEachTile(const Callback<const Tile&>& callback) const {
        std::for_each(tiledMap_.begin(), tiledMap_.end(), [&callback](auto& row) {
            std::for_each(row.begin(), row.end(), callback);
        });
    }

    void Grid::forEachTile_(const Callback<Tile&> &callback) {
        std::for_each(tiledMap_.begin(), tiledMap_.end(), [&callback](auto& row) {
            std::for_each(row.begin(), row.end(), callback);
        });
    }

    void Grid::forEachTileWithId(char id, const Callback<const Tile&>& callback) const {
        forEachTile([&callback, id](const Tile& tile) {
            if (tile.getId() == id)
                callback(tile);
        });
    }

    void Grid::forEachTileExcept(char id, const Callback<const Tile&>& callback) const {
        forEachTile([&callback, id](const Tile& tile) {
            if (tile.getId() != id)
                callback(tile);
        });
    }

    void Grid::forEachTileInRange(Index startPos, Index endPos, const Callback<const Tile&>& callback) const {
        if (isIndexValid(startPos) && isIndexValid(endPos)) {
            std::for_each(tiledMap_[startPos.row].begin() + startPos.colm,
                tiledMap_[startPos.row].begin() + endPos.colm,
                [&callback](const Tile& tile) {
                    callback(tile);
            });
        }
    }

    const Tile& Grid::getTileAbove(const Index &index) const {
        return getTile(Index{index.row - 1, index.colm});
    }

    const Tile& Grid::getTileBelow(const Index &index) const {
        return getTile(Index{index.row + 1, index.colm});
    }

    const Tile& Grid::getTileLeftOf(const Index &index) const {
        return getTile(Index{index.row, index.colm - 1});
    }

    const Tile& Grid::getTileRightOf(const Index &index) const {
        return getTile(Index{index.row, index.colm + 1});
    }

    Vector2u Grid::getSize() const {
        return mapSizeInPixels_;
    }

    unsigned int Grid::getSpaceBetweenTiles() const {
        return tileSpacing_;
    }

    Vector2u Grid::getSizeInTiles() const {
        return {numOfColms_, numOfRows_};
    }

    const Tile& Grid::getTileOccupiedByChild(const GridObject* child) const {
        if (child && hasChild(child))
            return getTile(child->getTransform().getPosition());
        else
            return invalidTile_;
    }

    bool Grid::isTileOccupied(const Tile &tile) const {
        return isTileOccupied(tile.getIndex());
    }

    bool Grid::isTileOccupied(const Index &index) const {
        return std::any_of(children_.begin(), children_.end(), [this, &index] (GridObject* child) {
            return isInTile(child, tiledMap_[index.row][index.colm]);
        });
    }

    void Grid::onRenderChange(const Property &property) {
        if (property.getName() == "visible") {
            auto visible = property.getValue<bool>();
            forEachTile_([visible](Tile& tile) {
                tile.setVisible(visible);
            });

            if (visible)
                backgroundTile_.setFillColour(renderer_.getGridLineColour());
            else
                backgroundTile_.setFillColour(mighter2d::Colour::Transparent);
        } else if (property.getName() == "tileColour") {
            forEachTile_([&property](Tile& tile) {
                if (!tile.isCollidable())
                    tile.setFillColour(property.getValue<Colour>());
            });
        } else if (property.getName() == "collidableTileColour") {
            forEachTile_([&property](Tile& tile) {
                if (tile.isCollidable())
                    tile.setFillColour(property.getValue<Colour>());
            });
        } else if (property.getName() == "gridLineColour")
            backgroundTile_.setFillColour(property.getValue<Colour>());
        else if (property.getName() == "backgroundTexture")
            backgroundTile_.setTexture(property.getValue<std::string>());
    }

    void Grid::unsubscribeDestructionListener(GridObject *child) {
        child->removeEventListener(destructionIds_[child->getObjectId()]);
        destructionIds_.erase(child->getObjectId());
    }

    Grid::~Grid() {
        removeAllChildren();
    }
}
