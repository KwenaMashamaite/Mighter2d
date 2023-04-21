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

#include "Mighter2d/core/object/Object.h"
#include "Mighter2d/utility/Helpers.h"

namespace mighter2d {
    namespace {
        unsigned int objectIdCounter = 0u;
    }

    Object::Object() :
        id_{objectIdCounter++}
    {}

    Object::Object(const Object& other) :
        EventEmitter(other),
        id_{objectIdCounter++},
        tag_{other.tag_}
    {
        removeAllEventListeners("Object_destruction");
    }

    Object &Object::operator=(const Object & other) {
        // We don't want to assign the object id, each must have a unique one
        if (this != &other) {
            tag_ = other.tag_;
            EventEmitter::operator=(other);
            removeAllEventListeners("Object_destruction");
        }

        return *this;
    }

    void Object::setTag(const std::string &tag) {
        if (tag_ != tag) {
            tag_ = tag;
            emitChange(Property{"tag", tag_});
        }
    }

    const std::string &Object::getTag() const {
        return tag_;
    }

    unsigned int Object::getObjectId() const {
        return id_;
    }

    std::string Object::getBaseClassName() const {
        return "Object";
    }

    std::string Object::getClassName() const {
        return "Object";
    }

    int Object::onPropertyChange(const std::string &property, const Callback<Property>& callback, bool oneTime) {
        return utility::addEventListener(*this, "Object_" + property + "Change", callback, oneTime);
    }

    int Object::onPropertyChange(const Callback<Property> &callback, bool oneTime) {
        return utility::addEventListener(*this, "Object_propertyChange", callback, oneTime);
    }

    int Object::onDestruction(const Callback<>& callback) {
        return addOnceEventListener("Object_destruction", callback);
    }

    bool Object::isSameObjectAs(const Object &other) const {
        return id_ == other.id_;
    }

    void Object::emitChange(const Property &property) {
        emit("Object_" + property.getName() + "Change", property);
        emit("Object_propertyChange", property);
    }

    void Object::emitDestruction() {
        emit("Object_destruction");
    }

    Object::~Object() {
        emitDestruction();
    }
}
