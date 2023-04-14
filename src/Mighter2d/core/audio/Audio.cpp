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

#include "Mighter2d/core/audio/Audio.h"

namespace mighter2d::audio {
    Audio::Audio() :
        isMuted_(false),
        volumeBeforeMute_(100.0f)
    {}

    std::string Audio::getClassType() const {
        return "Audio";
    }

    void Audio::adjustVolume(float offset) {
        if (auto currentVolume = getVolume(); currentVolume + offset > 100.0f)
            setVolume(100.0f);
        else if (currentVolume + offset < 0.0f)
            setVolume(0.0f);
        else
            setVolume(currentVolume + offset);
    }

    void Audio::setMute(bool mute) {
        if (mute && !isMuted_) {
            isMuted_ = true;
            volumeBeforeMute_ = getVolume();
            setVolume(0.0f);
            emitChange(Property{"mute", true});
        } else if (!mute && isMuted_) {
            isMuted_ = false;
            setVolume(volumeBeforeMute_);
            emitChange(Property{"mute", false});
        }
    }

    bool Audio::isMuted() const {
        return isMuted_;
    }

    void Audio::restart() {
        seek(Time::Zero);
    }

    bool Audio::isPlaying() const {
        return getStatus() == Status::Playing;
    }

    bool Audio::isPaused() const {
        return getStatus() == Status::Paused;
    }

    bool Audio::isStopped() const {
        return getStatus() == Status::Stopped;
    }

    Audio::~Audio() {
        emitDestruction();
    }
}
