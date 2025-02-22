/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/resources/behavioral/conversions/conversions_resource.h"

#include <memory>
#include <string>
#include <utility>

#include "base/files/file.h"
#include "brave/components/brave_ads/core/internal/common/unittest/unittest_base.h"
#include "brave/components/brave_ads/core/internal/common/unittest/unittest_file_util.h"
#include "brave/components/brave_ads/core/internal/resources/resources_unittest_constants.h"

// npm run test -- brave_unit_tests --filter=BraveAds*

namespace brave_ads {

using testing::_;
using testing::Invoke;

namespace {
constexpr char kResourceId[] = "nnqccijfhvzwyrxpxwjrpmynaiazctqb";
}  // namespace

class BraveAdsConversionsResourceTest : public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUp();

    resource_ = std::make_unique<ConversionsResource>();
  }

  bool LoadResource() {
    resource_->Load();
    task_environment_.RunUntilIdle();
    return resource_->IsInitialized();
  }

  std::unique_ptr<ConversionsResource> resource_;
};

TEST_F(BraveAdsConversionsResourceTest, LoadResource) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(LoadResource());
}

TEST_F(BraveAdsConversionsResourceTest, DoNotLoadInvalidResource) {
  // Arrange
  CopyFileFromTestPathToTempPath(kInvalidResourceId, kResourceId);

  // Act

  // Assert
  EXPECT_FALSE(LoadResource());
}

TEST_F(BraveAdsConversionsResourceTest, DoNotLoadMissingResource) {
  // Arrange
  EXPECT_CALL(ads_client_mock_, LoadFileResource(kResourceId, _, _))
      .WillOnce(Invoke([](const std::string& /*id*/, const int /*version*/,
                          LoadFileCallback callback) {
        const base::FilePath path =
            GetFileResourcePath().AppendASCII(kMissingResourceId);

        base::File file(
            path, base::File::Flags::FLAG_OPEN | base::File::Flags::FLAG_READ);
        std::move(callback).Run(std::move(file));
      }));

  // Act

  // Assert
  EXPECT_FALSE(LoadResource());
}

TEST_F(BraveAdsConversionsResourceTest, IsNotInitialized) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(resource_->IsInitialized());
}

}  // namespace brave_ads
