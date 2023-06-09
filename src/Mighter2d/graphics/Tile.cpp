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

#include "Mighter2d/graphics/Tile.h"
#include "Mighter2d/graphics/RenderTarget.h"

namespace mighter2d {
    Tile::Tile(Scene& scene, Vector2u size, Vector2f position) :
        Drawable(scene),
        id_{'\0'},
        index_{-1, -1},
        tile_(scene, {static_cast<float>(size.x), static_cast<float>(size.y)}),
        isCollidable_{false}
    {
        tile_.setFillColour(Colour::White);
        tile_.setPosition(position);
    }

    Tile::Tile(const Tile& other) :
        Drawable(other),
        id_{other.id_},
        index_{other.index_},
        tile_{other.tile_},
        isCollidable_{other.isCollidable_}
    {}

    Tile& Tile::operator=(Tile other) {
        swap(other);
        return *this;
    }

    void Tile::swap(Tile &other) {
        using std::swap;

        swap(id_, other.id_);
        swap(index_, other.index_);
        swap(tile_, other.tile_);
        swap(isCollidable_, other.isCollidable_);
    }

    std::string Tile::getClassName() const {
        return "Tile";
    }

    Vector2u Tile::getSize() const {
        return {static_cast<unsigned int>(tile_.getGlobalBounds().width),
                static_cast<unsigned int>(tile_.getGlobalBounds().height)};
    }

    void Tile::setPosition(float x, float y) {
        if (getPosition() == Vector2f{x, y})
            return;

        tile_.setPosition(x, y);

        emitChange(Property{Property{"position", getPosition()}});
    }

    void Tile::setPosition(Vector2f position) {
        setPosition(position.x, position.y);
    }

    Vector2f Tile::getPosition() const {
        return {tile_.getPosition().x, tile_.getPosition().y};
    }

    Vector2f Tile::getWorldCentre() const {
        return {tile_.getPosition().x + tile_.getSize().x / 2.0f,
                tile_.getPosition().y + tile_.getSize().y / 2.0f};
    }

    Vector2f Tile::getLocalCentre() const {
        return tile_.getSize() / 2.0f;
    }

    void Tile::setSize(unsigned int width, unsigned int height) {
        if (getSize() == Vector2u{width, height})
            return;

        tile_.setSize({static_cast<float>(width), static_cast<float>(height)});

        emitChange(Property{"size", getSize()});
    }

    void Tile::setSize(Vector2u size) {
        setSize(size.x, size.y);
    }

    void Tile::setCollidable(bool collidable) {
        if (isCollidable_ == collidable)
            return;

        isCollidable_ = collidable;

        emitChange(Property{"collidable", isCollidable_});
    }

    void Tile::setId(char id) {
        if (id_ != id) {
            id_ = id;
            emitChange(Property{"id", id_});
        }
    }

    char Tile::getId() const {
        return id_;
    }

    void Tile::draw(priv::RenderTarget &renderTarget) const {
        renderTarget.draw(tile_);
    }

    bool Tile::isCollidable() const {
        return isCollidable_;
    }

    bool Tile::contains(Vector2f point) const {
        return ((point.x >= getPosition().x && point.x <= getPosition().x + getSize().x)
                && (point.y >= getPosition().y && point.y <= getPosition().y + getSize().y));
    }

    void Tile::setIndex(Index index) {
        if (index_ != index) {
            index_ = index;
            emitChange(Property{"index", index_});
        }
    }

    Index Tile::getIndex() const {
        return index_;
    }

    void Tile::setFillColour(const Colour &colour) {
        if (tile_.getFillColour() != colour) {
            tile_.setFillColour(colour);
            emitChange(Property{"fillColour", tile_.getFillColour()});
        }
    }

    Colour Tile::getFillColour() const {
        return tile_.getFillColour();
    }

    Tile::~Tile() {
        emitDestruction();
    }
}
