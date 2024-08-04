/*
 * SPDX-FileCopyrightText: 2024 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <aidl/vendor/lineage/accessories/Accessories.h>

#define LOG_TAG "Accessories"

#include <aidl/vendor/lineage/accessories/Accessory.h>
#include <android-base/logging.h>

using std::lock_guard;
using std::shared_ptr;
using std::vector;

namespace aidl {
namespace vendor {
namespace lineage {
namespace accessories {

::ndk::ScopedAStatus Accessories::getAccessories(vector<shared_ptr<IAccessory>>* _aidl_return) {
    lock_guard lock(mAccessoriesLock);

    for (const auto& accessory : mAccessories) {
        _aidl_return->push_back(accessory);
    }

    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus Accessories::setCallback(const shared_ptr<IAccessoriesCallback>& callback) {
    mCallback = callback;

    // Send to the callback all the accessories that are already present
    if (mCallback != nullptr) {
        lock_guard lock(mAccessoriesLock);

        for (const auto& accessory : mAccessories) {
            if (accessory == nullptr) {
                continue;
            }

            mCallback->onAccessoryAdded(accessory);
        }
    }

    return ::ndk::ScopedAStatus::ok();
}

binder_status_t Accessories::dump(int fd, const char** args, uint32_t numArgs) {
    lock_guard lock(mAccessoriesLock);

    dprintf(fd, "Accessories:\n");
    for (const auto& accessory : mAccessories) {
        if (accessory == nullptr) {
            continue;
        }

        accessory->dump(fd, args, numArgs);
    }

    return STATUS_OK;
}

void Accessories::addAccessory(const shared_ptr<Accessory>& accessory) {
    lock_guard lock(mAccessoriesLock);

    mAccessories.push_back(accessory);

    if (mCallback != nullptr) {
        mCallback->onAccessoryAdded(accessory);
    }
}

void Accessories::removeAccessory(const shared_ptr<Accessory>& accessory) {
    lock_guard lock(mAccessoriesLock);

    mAccessories.erase(std::remove(mAccessories.begin(), mAccessories.end(), accessory),
                       mAccessories.end());

    if (mCallback != nullptr) {
        mCallback->onAccessoryRemoved(accessory);
    }
}

}  // namespace accessories
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
