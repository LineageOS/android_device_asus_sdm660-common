/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "TouchscreenGestureService"

#include "TouchscreenGesture.h"
#include <android-base/logging.h>
#include <fstream>

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

const std::map<int32_t, TouchscreenGesture::GestureInfo> TouchscreenGesture::kGestureInfoMap = {
    {0, {249, "Letter C", "/proc/touchpanel/letter_c_enable"}},
    {1, {250, "Letter e", "/proc/touchpanel/letter_e_enable"}},
    {2, {251, "Letter S", "/proc/touchpanel/letter_s_enable"}},
    {3, {252, "Letter V", "/proc/touchpanel/letter_v_enable"}},
    {4, {253, "Letter W", "/proc/touchpanel/letter_w_enable"}},
    {5, {254, "Letter Z", "/proc/touchpanel/letter_z_enable"}},
    {6, {255, "One finger swipe up", "/proc/touchpanel/up_swipe_enable"}},
};

ndk::ScopedAStatus TouchscreenGesture::getSupportedGestures(std::vector<Gesture>* _aidl_return) {
    std::vector<Gesture> gestures;

    for (const auto& entry : kGestureInfoMap) {
        gestures.push_back({entry.first, entry.second.name, entry.second.keycode});
    }
    *_aidl_return = gestures;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus TouchscreenGesture::setGestureEnabled(const Gesture& gesture, bool enabled) {
    const auto entry = kGestureInfoMap.find(gesture.id);
    if (entry == kGestureInfoMap.end()) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    std::ofstream file(entry->second.path);
    file << (enabled ? "1" : "0");
    LOG(DEBUG) << "Wrote file " << entry->second.path << " fail " << file.fail();
    if (file.fail()) return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);

    return ndk::ScopedAStatus::ok();
}

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
