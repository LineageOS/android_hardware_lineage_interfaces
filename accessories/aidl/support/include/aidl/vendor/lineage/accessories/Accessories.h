/*
 * SPDX-FileCopyrightText: 2024 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/accessories/Accessory.h>
#include <aidl/vendor/lineage/accessories/BnAccessories.h>
#include <mutex>
#include <vector>

namespace aidl {
namespace vendor {
namespace lineage {
namespace accessories {

/**
 * Base IAccessories class with helper methods. Thread-safe!
 */
class Accessories : public BnAccessories {
  public:
    Accessories() = default;
    Accessories(const Accessories&) = delete;

    ::ndk::ScopedAStatus getAccessories(
            ::std::vector<::std::shared_ptr<IAccessory>>* _aidl_return) override;
    ::ndk::ScopedAStatus setCallback(
            const ::std::shared_ptr<IAccessoriesCallback>& callback) override;

    binder_status_t dump(int fd, const char** args, uint32_t numArgs) override;

  protected:
    /**
     * Add an accessory to the list of accessories. Will also take care of calling the callback.
     *
     * @param accessory The accessory to add
     */
    void addAccessory(const ::std::shared_ptr<Accessory>& accessory);

    /**
     * Remove an accessory from the list of accessories. Will also take care of calling the
     * callback.
     *
     * @param accessory The accessory to remove
     */
    void removeAccessory(const ::std::shared_ptr<Accessory>& accessory);

  private:
    ::std::vector<::std::shared_ptr<Accessory>> mAccessories;
    ::std::mutex mAccessoriesLock;

    ::std::shared_ptr<IAccessoriesCallback> mCallback;
};

}  // namespace accessories
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
