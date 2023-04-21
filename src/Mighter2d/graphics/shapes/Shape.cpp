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

#include "Mighter2d/graphics/shapes/Shape.h"
#include "Mighter2d/core/scene/Scene.h"
#include "Mighter2d/graphics/shapes/ShapeImpl.h"

namespace mighter2d {
    Shape::Shape(Scene& scene, std::unique_ptr<priv::IShapeImpl> impl, Type type) :
        Drawable(scene),
        scene_(&scene),
        pimpl_{std::move(impl)},
        type_{type}
    {}

    Shape::Shape(const Shape& other) :
        Drawable(other),
        scene_(other.scene_),
        pimpl_{other.pimpl_->clone()},
        type_{other.type_}
    {
    }

    Shape &Shape::operator=(const Shape& rhs) {
        if (this != &rhs) {
            Drawable::operator=(rhs);
            scene_ = rhs.scene_;
            pimpl_ = rhs.pimpl_->clone();
            type_ = rhs.type_;
        }

        return *this;
    }

    Shape::Shape(Shape &&) noexcept = default;
    Shape &Shape::operator=(Shape &&) noexcept = default;

    Scene &Shape::getScene() {
        return *scene_;
    }

    const Scene &Shape::getScene() const {
        return *scene_;
    }

    std::string Shape::getBaseClassName() const {
        return "Shape";
    }

    Shape::Type Shape::getShapeType() const {
        return type_;
    }

    void Shape::setTexture(const std::string &filename) {
        pimpl_->setTexture(filename);
    }

    void Shape::setTexture(const Texture &texture) {
        pimpl_->setTexture(texture);
    }

    Texture *Shape::getTexture() {
        return pimpl_->getTexture();
    }

    const Texture *Shape::getTexture() const {
        return pimpl_->getTexture();
    }

    void Shape::setFillColour(const Colour &colour) {
        if (getFillColour() == colour)
            return;

        pimpl_->setFillColour(colour);
        emitChange(Property{"fillColour", colour});
    }

    Colour Shape::getFillColour() const {
        return pimpl_->getFillColour();
    }

    void Shape::setOutlineColour(const Colour &colour) {
        if (getOutlineColour() == colour)
            return;

        pimpl_->setOutlineColour(colour);
        emitChange(Property{"outlineColour", colour});
    }

    Colour Shape::getOutlineColour() const {
        return pimpl_->getOutlineColour();
    }

    void Shape::setOutlineThickness(float thickness) {
        if(getOutlineThickness() == thickness)
            return;

        pimpl_->setOutlineThickness(thickness);
        emitChange(Property{"outlineThickness", thickness});
    }

    float Shape::getOutlineThickness() const {
        return pimpl_->getOutlineThickness();
    }

    FloatRect Shape::getLocalBounds() const {
        return pimpl_->getLocalBounds();
    }

    FloatRect Shape::getGlobalBounds() const {
        return pimpl_->getGlobalBounds();
    }

    void Shape::setPosition(float x, float y) {
        Vector2f pos = getPosition();

        if (pos.x == x && pos.y == y)
            return;

        pimpl_->setPosition(x, y);
        emitChange(Property{"position", getPosition()});
    }

    void Shape::setPosition(const Vector2f& position) {
        setPosition(position.x, position.y);
    }

    Vector2f Shape::getPosition() const {
        return pimpl_->getPosition();
    }

    void Shape::setRotation(float angle) {
        if (getRotation() == angle)
            return;

        pimpl_->setRotation(angle);
        emitChange(Property{"rotation", angle});
    }

    void Shape::rotate(float angle) {
        setRotation(getRotation() + angle);
    }

    float Shape::getRotation() const {
        return pimpl_->getRotation();
    }

    void Shape::setScale(float factorX, float factorY) {
        Vector2f scale = getScale();
        if (scale.x == factorX && scale.y == factorY)
            return;

        pimpl_->setScale(factorX, factorY);
        emitChange(Property{"scale", getScale()});
    }

    void Shape::setScale(const Vector2f& scale) {
        setScale(scale.x, scale.y);
    }

    void Shape::scale(float factorX, float factorY) {
        setScale(getScale().x * factorX, getScale().y * factorY);
    }

    void Shape::scale(const Vector2f& factor) {
        scale(factor.x, factor.y);
    }

    Vector2f Shape::getScale() const {
        return pimpl_->getScale();
    }

    void Shape::setOrigin(float x, float y) {
        Vector2f origin = getOrigin();

        if (origin.x == x && origin.y == y)
            return;

        pimpl_->setOrigin(x, y);
        emitChange(Property{"origin", getOrigin()});
    }

    void Shape::setOrigin(const Vector2f& origin) {
        setOrigin(origin.x, origin.y);
    }

    Vector2f Shape::getOrigin() const {
        return pimpl_->getOrigin();
    }

    void Shape::move(float offsetX, float offsetY) {
        setPosition(getPosition().x + offsetX, getPosition().y + offsetY);
    }

    void Shape::move(const Vector2f& offset) {
        move(offset.x, offset.y);
    }

    void Shape::draw(priv::RenderTarget &renderTarget) const {
        pimpl_->draw(renderTarget);
    }

    std::shared_ptr<void> Shape::getInternalPtr() const {
        return pimpl_->getInternalPtr();
    }

    Shape::~Shape() {
        emitDestruction();
    }
}
