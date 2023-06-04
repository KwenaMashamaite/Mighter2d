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

#include "Mighter2d/core/engine/EngineSettings.h"

namespace mighter2d {
    template <class T>
    void addOrUpdateValue(PrefContainer& settings, const std::string& preference, PrefType  prefType , T&& defaultValue, const std::string& description)
    {
        if (settings.hasPref(preference) && settings.getPref(preference).hasValue())
            return;
        else if (!settings.hasPref(preference))
            settings.addPref({preference, prefType, std::forward<T>(defaultValue), description});
        else
            settings.getPref(preference).setValue<T>(std::forward<T>(defaultValue));
    }

    EngineSettings::EngineSettings() {
        setDefaultSettings();
    }

    void EngineSettings::loadFromFile(const std::string &filename) {
        settings_.load(filename);

        // If the load operation does not load a setting, we set it to its default value
        setDefaultSettings();
    }

    void EngineSettings::setWindowTitle(const std::string &title) {
        settings_.getPref("WINDOW_TITLE").setValue<std::string>(title);
    }

    std::string EngineSettings::getWindowTitle() const {
        return settings_.getPref("WINDOW_TITLE").getValue<std::string>();
    }

    void EngineSettings::setWindowIcon(const std::string &iconFilename) {
        settings_.getPref("WINDOW_ICON").setValue<std::string>(iconFilename);
    }

    std::string EngineSettings::getWindowIcon() const {
        return settings_.getPref("WINDOW_ICON").getValue<std::string>();
    }

    void EngineSettings::setWindowWidth(int width) {
        settings_.getPref("WINDOW_WIDTH").setValue<int>(width);
    }

    int EngineSettings::getWindowWidth() const {
        return settings_.getPref("WINDOW_WIDTH").getValue<int>();
    }

    void EngineSettings::setWindowHeight(int height) {
        settings_.getPref("WINDOW_HEIGHT").setValue<int>(height);
    }

    int EngineSettings::getWindowHeight() const {
        return settings_.getPref("WINDOW_HEIGHT").getValue<int>();
    }

    void EngineSettings::setWindowFPSLimit(int fpsLimit) {
        settings_.getPref("WINDOW_FPS_LIMIT").setValue<int>(fpsLimit);
    }

    int EngineSettings::getWindowFPSLimit() const {
        return settings_.getPref("WINDOW_FPS_LIMIT").getValue<int>();
    }

    void EngineSettings::setWindowFullScreenEnable(bool fullscreen) {
        settings_.getPref("WINDOW_FULLSCREEN").setValue<bool>(fullscreen);
    }

    bool EngineSettings::getWindowFullscreenEnabled() const {
        return settings_.getPref("WINDOW_FULLSCREEN").getValue<bool>();
    }

    void EngineSettings::setWindowVerticalSyncEnable(bool enable) {
        settings_.getPref("WINDOW_V_SYNC").setValue<bool>(enable);
    }

    bool EngineSettings::getWindowVerticalSyncEnabled() const {
        return settings_.getPref("WINDOW_V_SYNC").getValue<bool>();
    }

    void EngineSettings::setFontsDirectory(const std::string &dir) {
        settings_.getPref("FONTS_DIR").setValue<std::string>(dir);
    }

    std::string EngineSettings::getFontsDirectory() const {
        return settings_.getPref("FONTS_DIR").getValue<std::string>();
    }

    void EngineSettings::setTexturesDirectory(const std::string &dir) {
        settings_.getPref("TEXTURES_DIR").setValue<std::string>(dir);
    }

    std::string EngineSettings::getTexturesDirectory() const {
        return settings_.getPref("TEXTURES_DIR").getValue<std::string>();
    }

    void EngineSettings::setSoundEffectsDirectory(const std::string &dir) {
        settings_.getPref("SOUND_EFFECTS_DIR").setValue<std::string>(dir);
    }

    std::string EngineSettings::getSoundEffectsDirectory() const {
        return settings_.getPref("SOUND_EFFECTS_DIR").getValue<std::string>();
    }

    void EngineSettings::setMusicDirectory(const std::string &dir) {
        settings_.getPref("MUSIC_DIR").setValue<std::string>(dir);
    }

    std::string EngineSettings::getMusicDirectory() const {
        return settings_.getPref("MUSIC_DIR").getValue<std::string>();
    }

    void EngineSettings::setDefaultSettings() {
        addOrUpdateValue(settings_, "WINDOW_TITLE", PrefType::String, std::string("Untitled"), "The title of the render window");
        addOrUpdateValue(settings_, "WINDOW_ICON", PrefType::String, std::string("OS"), "The icon of the render window");
        addOrUpdateValue(settings_, "WINDOW_WIDTH", PrefType::Int, 600, "The width of the render window");
        addOrUpdateValue(settings_, "WINDOW_HEIGHT", PrefType::Int, 600, "The height of the render window");
        addOrUpdateValue(settings_, "WINDOW_FPS_LIMIT", PrefType::Int, 60, "The frames per second limit of the render window");
        addOrUpdateValue(settings_, "WINDOW_FULLSCREEN", PrefType::Bool, false, "Indicates whether or not the render window should be created in full screen mode");
        addOrUpdateValue(settings_, "WINDOW_V_SYNC", PrefType::Bool, false, "Indicates whether or not vertical synchronization should be enabled");
        addOrUpdateValue(settings_, "FONTS_DIR", PrefType::String, std::string(""), "The directory in which fonts can be found");
        addOrUpdateValue(settings_, "TEXTURES_DIR", PrefType::String, std::string(""), "The directory in which textures/images can be found");
        addOrUpdateValue(settings_, "SOUND_EFFECTS_DIR", PrefType::String, std::string(""), "The directory in which sound effects can be found");
        addOrUpdateValue(settings_, "MUSIC_DIR", PrefType::String, std::string(""), "The directory in which music can be found");
    }
}