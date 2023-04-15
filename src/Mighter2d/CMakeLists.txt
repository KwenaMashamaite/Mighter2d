# Hide all symbols by default
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

set(MIGHTER2D_SRC
    common/Property.cpp
    common/PropertyContainer.cpp
    common/Preference.cpp
    common/PrefContainer.cpp
    common/Transform.cpp
    core/object/Object.cpp
    core/animation/Animation.cpp
    core/animation/AnimationFrame.cpp
    core/animation/Animator.cpp
    core/audio/Audio.cpp
    core/audio/Music.cpp
    core/audio/SoundEffect.cpp
    core/object/GameObject.cpp
    core/object/GridObject.cpp
    core/object/ExcludeList.cpp
    core/event/EventEmitter.cpp
    core/event/GlobalEventEmitter.cpp
    core/input/Joystick.cpp
    core/input/Keyboard.cpp
    core/input/Mouse.cpp
    core/engine/Engine.cpp
    core/audio/AudioManager.cpp
    core/input/InputManager.cpp
    core/resources/ResourceManager.cpp
    core/resources/ResourceHolder.cpp
    core/resources/ResourceLoader.cpp
    core/physics/path/AdjacencyList.cpp
    core/physics/path/BFS.cpp
    core/physics/path/DFS.cpp
    core/physics/path/IPathFinderStrategy.cpp
    core/physics/GridMover.cpp
    core/physics/TargetGridMover.cpp
    core/physics/KeyboardGridMover.cpp
    core/physics/RandomGridMover.cpp
    core/physics/CyclicGridMover.cpp
    core/scene/Scene.cpp
    core/scene/SceneManager.cpp
    core/scene/RenderLayer.cpp
    core/scene/RenderLayerContainer.cpp
    core/scene/SceneStateObserver.cpp
    core/grid/Index.cpp
    core/grid/Grid.cpp
    core/grid/GridParser.cpp
    core/grid/GridRenderer.cpp
    core/time/Clock.cpp
    core/time/Timer.cpp
    core/time/TimerManager.cpp
    core/time/Time.cpp
    ui/renderers/BoxLayoutRenderer.cpp
    ui/renderers/ButtonRenderer.cpp
    ui/renderers/CheckBoxRenderer.cpp
    ui/renderers/ChildWindowRenderer.cpp
    ui/renderers/EditBoxRenderer.cpp
    ui/renderers/LabelRenderer.cpp
    ui/renderers/ProgressBarRenderer.cpp
    ui/renderers/SliderRenderer.cpp
    ui/renderers/RadioButtonRenderer.cpp
    ui/renderers/PanelRenderer.cpp
    ui/renderers/ScrollablePanelRenderer.cpp
    ui/renderers/TabsRenderer.cpp
    ui/renderers/PictureRenderer.cpp
    ui/renderers/SeparatorRenderer.cpp
    ui/renderers/SpinButtonRenderer.cpp
    ui/renderers/MenuBarRenderer.cpp
    ui/renderers/MessageBoxRenderer.cpp
    ui/widgets/Widget.cpp
    ui/widgets/WidgetContainer.cpp
    ui/widgets/ClickableWidget.cpp
    ui/widgets/BitmapButton.cpp
    ui/widgets/Button.cpp
    ui/widgets/ToggleButton.cpp
    ui/widgets/CheckBox.cpp
    ui/widgets/EditBox.cpp
    ui/widgets/Label.cpp
    ui/widgets/ProgressBar.cpp
    ui/widgets/Slider.cpp
    ui/widgets/RadioButton.cpp
    ui/widgets/Panel.cpp
    ui/widgets/ScrollablePanel.cpp
    ui/widgets/ChildWindow.cpp
    ui/widgets/IBoxLayout.cpp
    ui/widgets/VerticalLayout.cpp
    ui/widgets/HorizontalLayout.cpp
    ui/widgets/Tabs.cpp
    ui/widgets/TabsContainer.cpp
    ui/widgets/Picture.cpp
    ui/widgets/Separator.cpp
    ui/widgets/SpinControl.cpp
    ui/widgets/MenuBar.cpp
    ui/widgets/MessageBox.cpp
    ui/GuiContainer.cpp
    graphics/Texture.cpp
    graphics/Sprite.cpp
    graphics/Colour.cpp
    graphics/Tile.cpp
    graphics/RenderTarget.cpp
    graphics/Window.cpp
    graphics/SpriteSheet.cpp
    graphics/shapes/Shape.cpp
    graphics/shapes/RectangleShape.cpp
    graphics/shapes/CircleShape.cpp
    graphics/shapes/ConvexShape.cpp
    graphics/Drawable.cpp
    graphics/Camera.cpp
    graphics/SpriteImage.cpp
    utility/DiskFileReader.cpp
    utility/Helpers.cpp
    utility/Utils.cpp)

# Optimize single build
if(MIGHTER2D_OPTIMIZE_SINGLE_BUILD)
    list(LENGTH MIGHTER2D_SRC fileCount)
    if(MIGHTER2D_OPTIMIZE_SINGLE_BUILD_THREADS)
        set(threads ${MIGHTER2D_OPTIMIZE_SINGLE_BUILD_THREADS})
    else()
        include(ProcessorCount)
        ProcessorCount(threads)
    endif()

    if(${threads} LESS ${fileCount})
        math(EXPR threads "(${threads} / 2) + 1")
        math(EXPR fileCountMinusOne "${fileCount} - 1")
        math(EXPR threadsMinusOne "${threads} - 1")

        set(NEW_MIGHTER2D_SRC "")
        foreach(i RANGE 0 ${threadsMinusOne})
            set(OutputFile "${CMAKE_CURRENT_SOURCE_DIR}/CombinedSources-${i}.cpp")
            file(WRITE "${OutputFile}" "")
            foreach(j RANGE ${i} ${fileCountMinusOne} ${threads})
                list(GET MIGHTER2D_SRC ${j} inputFile)
                file(READ ${inputFile} CONTENTS)
                file(APPEND "${OutputFile}" "${CONTENTS}")
                list(APPEND NEW_MIGHTER2D_SRC "${OutputFile}")
            endforeach()
        endforeach()

        set(MIGHTER2D_SRC ${NEW_MIGHTER2D_SRC})
    endif()
endif()

# Create library and determine suffixes depending on static/shared configuration
if(MIGHTER2D_SHARED_LIBS)
    add_library(mighter2d SHARED ${MIGHTER2D_SRC})
    set_target_properties(mighter2d PROPERTIES DEFINE_SYMBOL MIGHTER2D_EXPORTS)
    set_target_properties(mighter2d PROPERTIES DEBUG_POSTFIX -d)

    # Set the version of the target
    set_target_properties(mighter2d PROPERTIES VERSION ${MIGHTER2D_VERSION_MAJOR}.${MIGHTER2D_VERSION_MINOR}.${MIGHTER2D_VERSION_PATCH})

    # on Windows/gcc get rid of "lib" prefix for shared libraries,
    # and transform the ".dll.a" suffix into ".a" for import libraries
    if(MIGHTER2D_OS_WINDOWS AND MIGHTER2D_COMPILER_GCC)
        set_target_properties(mighter2d PROPERTIES PREFIX "")
        set_target_properties(mighter2d PROPERTIES IMPORT_SUFFIX ".a")
    endif()

    target_compile_definitions(mighter2d PUBLIC MIGHTER2D_DYNAMIC)

else()
    add_library(mighter2d STATIC ${MIGHTER2D_SRC})
    set_target_properties(mighter2d PROPERTIES DEBUG_POSTFIX -s-d)
    set_target_properties(mighter2d PROPERTIES RELEASE_POSTFIX -s)
    set_target_properties(mighter2d PROPERTIES MINSIZEREL_POSTFIX -s)
    set_target_properties(mighter2d PROPERTIES RELWITHDEBINFO_POSTFIX -s)

    target_compile_definitions(mighter2d PUBLIC MIGHTER2D_STATIC)
endif()

# set the target's folder (for IDEs that support it, e.g. Visual Studio)
set_target_properties(mighter2d PROPERTIES FOLDER "MIGHTER2D")

# set the output directory for MIGHTER2D library
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

mighter2d_set_global_compile_flags(mighter2d)
mighter2d_set_stdlib(mighter2d)

if(MIGHTER2D_OPTIMIZE_SINGLE_BUILD AND MIGHTER2D_COMPILER_MSVC)
    target_compile_options(mighter2d PRIVATE /bigobj)
endif()

# Add <project>/include as public include directory
target_include_directories(mighter2d PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                            PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_include_directories(mighter2d INTERFACE $<INSTALL_INTERFACE:include>)

# Find SFML if not build in same project
if (NOT TARGET sfml-system OR NOT TARGET sfml-window OR NOT TARGET sfml-graphics OR NOT TARGET sfml-audio)
    find_package(SFML 2.5.1 REQUIRED COMPONENTS system window graphics audio)

    if (${SFML_VERSION} VERSION_LESS "2.5.1")
        message(FATAL_ERROR "MIGHTER2D requires SFML 2.5.1 or higher")
    endif()
endif ()

# Find TGUI if not build in same project
if (NOT TARGET tgui)
    find_package(TGUI 0.9.3 REQUIRED)

    if (${TGUI_VERSION} VERSION_LESS "0.9.3")
        message(FATAL_ERROR "MIGHTER2D requires TGUI 0.9.3 or higher")
    endif()
endif()

# Link MIGHTER2D dependencies
target_link_libraries(mighter2d PRIVATE tgui sfml-graphics sfml-window sfml-system sfml-audio)

# For Visual Studio on Windows, export debug symbols (PDB files) to lib directory
if(MIGHTER2D_GENERATE_PDB)
    # PDB files are only generated in Debug and RelWithDebInfo configurations, find out which one
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        set(MIGHTER2D_PDB_POSTFIX "-d")
    else()
        set(MIGHTER2D_PDB_POSTFIX "")
    endif()

    if(MIGHTER2D_SHARED_LIBS)
        # DLLs export debug symbols in the linker PDB (the compiler PDB is an intermediate file)
        set_target_properties(mighter2d PROPERTIES
                PDB_NAME "mighter2d${MIGHTER2D_PDB_POSTFIX}"
                PDB_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
    else()
        # Static libraries have no linker PDBs, thus the compiler PDBs are relevant
        set_target_properties(mighter2d PROPERTIES
                COMPILE_PDB_NAME "mighter2d-s${MIGHTER2D_PDB_POSTFIX}"
                COMPILE_PDB_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
    endif()
endif()

# Install library
install(TARGETS mighter2d EXPORT MIGHTER2DConfigExport
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/${CMAKE_BUILD_TYPE} COMPONENT bin
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT bin
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT devel
        )

# Generate the MIGHTER2DConfig.cmake file
mighter2d_export_target(MIGHTER2DConfigExport)
