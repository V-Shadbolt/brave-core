// Copyright (c) 2023 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "brave/browser/ui/commands/accelerator_service.h"

#include "base/test/scoped_feature_list.h"
#include "brave/components/commands/browser/accelerator_pref_manager.h"
#include "brave/components/commands/common/accelerator_parsing.h"
#include "brave/components/commands/common/features.h"
#include "chrome/app/chrome_command_ids.h"
#include "chrome/test/base/testing_profile.h"
#include "components/sync_preferences/testing_pref_service_syncable.h"
#include "content/public/test/browser_task_environment.h"
#include "testing/gtest/include/gtest/gtest.h"

class AcceleratorServiceUnitTest : public testing::Test {
 public:
  AcceleratorServiceUnitTest() {
    features_.InitAndEnableFeature(commands::features::kBraveCommands);
  }

  ~AcceleratorServiceUnitTest() override = default;

  void SetUp() override {
    commands::AcceleratorPrefManager::RegisterProfilePrefs(
        profile().GetTestingPrefService()->registry());
  }

  TestingProfile& profile() { return profile_; }

 private:
  content::BrowserTaskEnvironment task_environment_;
  TestingProfile profile_;
  base::test::ScopedFeatureList features_;
};

TEST_F(AcceleratorServiceUnitTest, CanOverrideExistingShortcut) {
  commands::AcceleratorService service(
      profile().GetPrefs(),
      {{IDC_NEW_TAB, {commands::FromCodesString("Control+KeyT")}}});

  auto accelerators = service.GetAcceleratorsForTesting();
  ASSERT_EQ(1u, accelerators[IDC_NEW_TAB].size());
  EXPECT_EQ("Control+KeyT",
            commands::ToCodesString(accelerators[IDC_NEW_TAB][0]));

  service.AssignAcceleratorToCommand(IDC_NEW_WINDOW, "Control+KeyT");
  accelerators = service.GetAcceleratorsForTesting();
  EXPECT_EQ(0u, accelerators[IDC_NEW_TAB].size());
  ASSERT_EQ(1u, accelerators[IDC_NEW_WINDOW].size());
  EXPECT_EQ("Control+KeyT",
            commands::ToCodesString(accelerators[IDC_NEW_WINDOW][0]));
}

TEST_F(AcceleratorServiceUnitTest, AcceleratorsArePersisted) {
  commands::AcceleratorService service(profile().GetPrefs(), {});

  auto accelerators = service.GetAcceleratorsForTesting();
  service.AssignAcceleratorToCommand(IDC_NEW_TAB, "Control+KeyT");

  commands::AcceleratorService service2(profile().GetPrefs(), {});
  accelerators = service2.GetAcceleratorsForTesting();

  ASSERT_EQ(1u, accelerators[IDC_NEW_TAB].size());
  EXPECT_EQ("Control+KeyT",
            commands::ToCodesString(accelerators[IDC_NEW_TAB][0]));
}

TEST_F(AcceleratorServiceUnitTest, AcceleratorsCanBeRemoved) {
  commands::AcceleratorService service(
      profile().GetPrefs(),
      {{IDC_NEW_TAB, {commands::FromCodesString("Control+KeyT")}}});
  service.AssignAcceleratorToCommand(IDC_NEW_TAB, "Control+KeyK");
  auto accelerators = service.GetAcceleratorsForTesting();
  EXPECT_EQ(2u, accelerators[IDC_NEW_TAB].size());

  // Unassigning a non-existent command should be a no-op.
  service.UnassignAcceleratorFromCommand(IDC_NEW_TAB, "Control+KeyA");
  accelerators = service.GetAcceleratorsForTesting();
  EXPECT_EQ(2u, accelerators[IDC_NEW_TAB].size());

  service.UnassignAcceleratorFromCommand(IDC_NEW_TAB, "Control+KeyT");
  accelerators = service.GetAcceleratorsForTesting();
  EXPECT_EQ(1u, accelerators[IDC_NEW_TAB].size());

  service.UnassignAcceleratorFromCommand(IDC_NEW_TAB, "Control+KeyU");
  accelerators = service.GetAcceleratorsForTesting();
  EXPECT_EQ(1u, accelerators[IDC_NEW_TAB].size());
}

TEST_F(AcceleratorServiceUnitTest, AcceleratorsCanBeReset) {
  commands::AcceleratorService service(
      profile().GetPrefs(), {{IDC_NEW_TAB,
                              {commands::FromCodesString("Control+KeyT"),
                               commands::FromCodesString("Control+KeyK"),
                               commands::FromCodesString("Control+KeyU")}}});

  auto accelerators = service.GetAcceleratorsForTesting();
  ASSERT_EQ(3u, accelerators[IDC_NEW_TAB].size());

  // Add one new command
  service.AssignAcceleratorToCommand(IDC_NEW_TAB, "Control+KeyJ");

  // Remove one command
  service.UnassignAcceleratorFromCommand(IDC_NEW_TAB, "Control+KeyK");

  // Reassign one accelerator
  service.AssignAcceleratorToCommand(IDC_NEW_WINDOW, "Control+KeyT");
  accelerators = service.GetAcceleratorsForTesting();

  ASSERT_EQ(2u, accelerators[IDC_NEW_TAB].size());
  ASSERT_EQ(1u, accelerators[IDC_NEW_WINDOW].size());

  service.ResetAcceleratorsForCommand(IDC_NEW_TAB);
  accelerators = service.GetAcceleratorsForTesting();

  ASSERT_EQ(3u, accelerators[IDC_NEW_TAB].size());
  EXPECT_EQ("Control+KeyT",
            commands::ToCodesString(accelerators[IDC_NEW_TAB][0]));
  EXPECT_EQ("Control+KeyK",
            commands::ToCodesString(accelerators[IDC_NEW_TAB][1]));
  EXPECT_EQ("Control+KeyU",
            commands::ToCodesString(accelerators[IDC_NEW_TAB][2]));
  EXPECT_EQ(0u, accelerators[IDC_NEW_WINDOW].size());
}

TEST_F(AcceleratorServiceUnitTest, DefaultAcceleratorsCanBeUpdated) {
  {
    commands::AcceleratorService service(
        profile().GetPrefs(),
        {{IDC_NEW_TAB,
          {commands::FromCodesString("Control+KeyT"),
           commands::FromCodesString("Control+KeyQ")}},
         {IDC_NEW_WINDOW, {commands::FromCodesString("Control+KeyN")}}});
    service.AssignAcceleratorToCommand(IDC_NEW_TAB, "Control+KeyJ");
    service.AssignAcceleratorToCommand(IDC_NEW_WINDOW, "Control+KeyW");
  }

  // In the new commands service, the following changes have been made:
  // 1) Remove |Control+KeyQ| from IDC_NEW_TAB
  // 2) Add |Control+KeyY| to IDC_NEW_TAB
  // 3) Add |Control+KeyW| to IDC_NEW_TAB (replacing the shortcut from
  // IDC_NEW_WINDOW)
  // 4) Remove default accelerators fro IDC_NEW_WINDOW 4) Add
  // 5) New default accelerator to IDC_PIN_TARGET_TAB
  commands::AcceleratorService new_service(
      profile().GetPrefs(),
      {{{IDC_NEW_TAB,
         {commands::FromCodesString("Control+KeyT"),
          commands::FromCodesString("Control+KeyY"),
          commands::FromCodesString("Control+KeyW")}},
        {IDC_PIN_TARGET_TAB, {commands::FromCodesString("Alt+KeyP")}}}});
  auto accelerators = new_service.GetAcceleratorsForTesting();
  ASSERT_EQ(4u, accelerators[IDC_NEW_TAB].size());
  EXPECT_EQ("Control+KeyT",
            commands::ToCodesString(accelerators[IDC_NEW_TAB][0]));
  EXPECT_EQ("Control+KeyJ",
            commands::ToCodesString(accelerators[IDC_NEW_TAB][1]));
  EXPECT_EQ("Control+KeyY",
            commands::ToCodesString(accelerators[IDC_NEW_TAB][2]));
  EXPECT_EQ("Control+KeyW",
            commands::ToCodesString(accelerators[IDC_NEW_TAB][3]));

  EXPECT_EQ(0u, accelerators[IDC_NEW_WINDOW].size());

  ASSERT_EQ(1u, accelerators[IDC_PIN_TARGET_TAB].size());
  EXPECT_EQ("Alt+KeyP",
            commands::ToCodesString(accelerators[IDC_PIN_TARGET_TAB][0]));
}
