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

#ifndef MIGHTER2D_ICLASSIFIABLE_H
#define MIGHTER2D_ICLASSIFIABLE_H

#include "Mighter2d/Config.h"

namespace mighter2d {
    /**
     * @brief An interface for classifying classes by their class names
     *
     * This class is meant to help you identify Mighter2d classes without the
     * use of casts. It serves no other purpose. You can also directly
     * inherit from it to make use of it in your project
     */
    class MIGHTER2D_API IClassifiable {
    public:
        /**
         * @brief Get the name of the concrete class
         * @return The name of the concrete class
         *
         * Example:
         *
         * @code
         * class Player : public mighter2d::IClassifiable {
         *    std::string getClassName() const override {return "Player"}
         * }
         * GameObject gObject;
         * std::cout << gObject.getClassName(); // Prints "Player"
         * @endcode
         *
         * @see getBaseClassName
         */
        virtual std::string getClassName() const = 0;

        /**
         * @brief Get the name of the base class
         * @return The name of the base class
         *
         * In contrast to getClassName() which returns the name of the concrete
         * class, this function returns the name of the concrete classes base
         * class.
         *
         * @code
         * auto rectangle = mighter2d::RectangleShape(); // RectangleShape is derived from Shape
         * std::cout << rectangle->getClassName(); // Prints "RectangleShape"
         * std::cout << rectangle->getBaseClassName(); // Prints "Shape"
         * @endcode
         *
         * @see getClassName
         */
        virtual std::string getBaseClassName() const {return "IClassifiable"; }
    };
}

#endif
