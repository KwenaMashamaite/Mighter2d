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

#ifndef MIGHTER2D_RENDERLAYERCONTAINER_H
#define MIGHTER2D_RENDERLAYERCONTAINER_H

#include "Mighter2d/Config.h"
#include "Mighter2d/core/object/Object.h"
#include "Mighter2d/graphics/Drawable.h"
#include <functional>
#include <map>
#include <memory>

namespace mighter2d {
    namespace priv {
        class RenderLayer;
        class RenderTarget;
    }

    /**
     * @brief Stores and manages a scene's render layers
     *
     * A render layer container determines the order in which objects are
     * drawn to the render window. After creating an object (e.g sprite),
     * you specify the RenderLayer in which the object belongs to and the
     * object will be rendered on top or below other objects depending on
     * the index of its render layer in the render layer container.
     *
     * Render layers are added to the container in ascending order with an
     * index of 0 representing the the furthest background. Therefore, the
     * last added render layer will always be the foreground. The layers
     * are drawn from index 0 going upwards. Note that you cannot change
     * the index of a layer (indexes will always be arranged in ascending
     * order), however you can change the layer at a given index. This means
     * that you can specify which layer gets rendered on top of which layer
     * using appropriate member function. For example if you add a mountain
     * sprite to a render layer at index 5 and a tree sprite to a render layer
     * at index 6, then the mountain will be rendered first followed by the
     * tree
     *
     * This class is not directly instantiatable, it is created when you
     * instantiate a Scene object. Each scene instance has its own render
     * layers and a single render layer container accessed with
     * mighter2d::Scene::getRenderLayers
     */
    class MIGHTER2D_API RenderLayerContainer : public Object {
    public:
        /**
         * @brief Copy constructor
         */
        RenderLayerContainer(const RenderLayerContainer&) = delete;

        /**
         * @brief Copy assignment operator
         */
        RenderLayerContainer& operator=(const RenderLayerContainer&) = delete;

        /**
         * @brief Move constructor
         */
        RenderLayerContainer(RenderLayerContainer&&) noexcept = default;

        /**
         * @brief Move assignment operator
         */
        RenderLayerContainer& operator=(RenderLayerContainer&&) noexcept = default;

        /**
         * @internal
         * @brief Add a drawable object to a render layer in the container
         * @param drawable The drawable object to be added
         * @param renderOrder The render order of the object in the render layer
         * @param renderLayer The RenderLayer to add the object belongs to
         *
         * If the render layer is unspecified or the specified layer cannot be
         * found then the drawable will be added to the 'default' layer. The
         * 'default' render layer is created by the Scene when you instantiate
         * it. Note that the 'default' layer may be deleted from the container,
         * however you must make sure that the layer you specify during a call
         * to this function already exists in the container otherwise undefined
         * behavior
         *
         * @note This function is internal fro internal use only
         *
         * @see mighter2d::Scene::getRenderLayers
         */
        void add(Drawable& drawable, int renderOrder = 0u, const std::string& renderLayer = "default");

        /**
         * @brief Get the name of this class
         * @return The name of this class
         */
        std::string getClassName() const override;

        /**
         * @brief Check if a given index is within bounds or not
         * @param index The index to be checked
         * @return True of the container has the layer otherwise false
         *
         * @see findByIndex and removeByIndex
         */
        bool isIndexValid(unsigned int index) const;

        /**
         * @brief Check if the container has a layer with the given name
         * @param name The name of the layer to be checked
         * @return True of the container has the layer otherwise false
         *
         * @see findByTag and removeByTag
         */
        bool hasLayer(const std::string& name) const;

        /**
         * @brief Remove the layer at the given index
         * @param index The index of the layer to be removed
         * @return True if the layer was removed or false if the index is
         *         out of bounds
         * @throws InvalidActionException if the layer with the given index is
         *         the @em default layer
         *
         * @note A scene will always contain at least one render layer which is
         * the @em default layer
         *
         * @see removeByTag
         */
        bool removeByIndex(unsigned int index);

        /**
         * @brief Remove the layer with the given name from the container
         * @param name The name of the container to be removed
         * @return True if the layer was removed or false if the layer does
         *         not exist
         * @throws InvalidActionException if name = "default"
         *
         * @note A scene will always contain at least one render layer which is
         * the @em default layer
         *
         * @see removeByIndex
         */
        bool removeByName(const std::string& name);

        /**
         * @brief Remove all layers from the container
         *
         * @note A scene will always contain at least one render layer which is
         * the @em default layer. Therefore, this function will remove all layers
         * except the @em default layer
         *
         * @see removeByIndex and removeByTag
         */
        void removeAll();

        /**
         * @brief Move a layer one level up
         * @param index The index of the layer to be moved
         *
         * This function has no effect if the index is out of bounds or the
         * layer is already at the highest level
         *
         * @see moveDown(unsigned int)
         */
        void moveUp(unsigned int index);

        /**
         * @brief Move the layer up one level
         * @param name The name of the layer to be moved
         *
         * This function has no effect if the layer does not exist
         *
         * @see moveDown(const std::string&)
         */
        void moveUp(const std::string& name);

        /**
         * @brief Move a layer one level down
         * @param index The index of the layer to be moved
         *
         * This function has no effect if the layer does not exist or the
         * layer is already at the lowest level
         *
         * @see moveUp(unsigned int)
         */
        void moveDown(unsigned int index);

        /**
         * @brief Move a layer one level down
         * @param name The name of the layer to be moved
         *
         * This function has no effect if the layer does not exist or the
         * layer is already at the lowest level
         *
         * @see moveUp(const std::string&)
         */
        void moveDown(const std::string& name);

        /**
         * @brief Move a layer to the highest level (foreground)
         * @param index The index of the layer to be moved
         *
         * This function has no effect if the layer does not exist or the
         * layer is already at the highest level
         *
         * @see sendToBack(unsigned int)
         */
        void sendToFront(unsigned int index);

        /**
         * @brief Move a layer to the highest level (foreground)
         * @param name The name of the level to be moved
         *
         * This function has no effect if the layer does not exist or the
         * layer is already at the highest level
         *
         * @see sendToBack(const std::string&)
         */
        void sendToFront(const std::string& name);

        /**
         * @brief Move a layer to the lowest level (background)
         * @param index The index of the layer ot be moved
         *
         * This function has no effect if the layer does not exist or the
         * layer is already at the lowest level
         *
         * @see sendToFront(unsigned int)
         */
        void sendToBack(unsigned int index);

        /**
         * @brief Move a layer to the lowest level (background)
         * @param name The name of the layer ot be moved
         *
         * This function has no effect if the layer does not exist or the
         * layer is already at the lowest level
         *
         * @see sendToFront(const std::string&)
         */
        void sendToBack(const std::string& name);

        /**
         * @brief Swap the render positions of two layers
         * @param layerOneIndex The index of the first layer
         * @param layerTwoIndex The index of the second layer
         * @return True if the two layers were swapped successfully, or
         *         false if either one of the given layers does not exist
         *         in the container
         *
         * @see swap(const std::string&, const std::string&)
         */
        bool swap(unsigned int layerOneIndex, unsigned int layerTwoIndex);

        /**
         * @brief Swap the positions of two layers
         * @param layerOne The name of the first layer
         * @param layerTwo The name iof the second layer
         *
         * This function has no effect of either of the layers cannot be found
         *
         * @see swap(unsigned int, unsigned int)
         */
        void swap(const std::string& layerOne, const std::string& layerTwo);

        /**
         * @brief Get the number of layer in the container
         * @return The number of layers in the container
         */
        std::size_t getCount() const;

        /**
         * @internal
         * @brief Render all the layers
         * @param window The window to render layers on
         *
         * @warning This function is intended for internal use only and
         * should never be called outside of Mighter2d
         */
        void render(priv::RenderTarget& window) const;

        /**
         * @internal
         * @brief Create a layer
         * @param name The name of the layer to be created
         * @return The created layer
         *
         * Note that the container keeps a pointer to the layer after it is
         * created, therefore you don't need to keep the returned pointer
         * alive after using it
         *
         * @warning The name of the layer must be unique, that is, another
         * layer must not exist in the container with the same name as
         * @a name otherwise it is undefined behaviour
         *
         * @note This function is internal fro internal use only
         *
         * @see removeByTag, removeByIndex and removeAll
         */
        std::shared_ptr<priv::RenderLayer> create(const std::string& name);

        /**
         * @internal
         * @brief Get a layer with the given name
         * @param name The name of the layer to retrieve
         * @return The layer with the given name or a nullptr if the layer
         *         does not exits
         *
         * @note This function is internal fro internal use only
         *
         * @see findByIndex
         */
        std::shared_ptr<priv::RenderLayer> findByName(const std::string& name) const;
        
        /**
         * @brief Destructor
         */
        ~RenderLayerContainer() override;

    private:
        /**
         * @brief Constructor
         */
        RenderLayerContainer() = default;

    private:
        std::map<unsigned int, std::shared_ptr<priv::RenderLayer>> layers_;   //!< Layers container
        std::map<std::string, unsigned int> inverseLayers_; //!< Layers container with keys and values swapped

        //!> Needs access to the constructor
        friend class Scene;
        friend class Grid;
    };
}

#endif //MIGHTER2D_RENDERLAYERCONTAINER_H
