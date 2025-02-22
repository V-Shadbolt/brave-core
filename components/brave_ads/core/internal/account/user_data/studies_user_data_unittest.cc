/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/account/user_data/studies_user_data.h"

#include <string>

#include "base/metrics/field_trial.h"
#include "base/test/values_test_util.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- brave_unit_tests --filter=BraveAds*

namespace brave_ads {

TEST(BraveAdsStudiesUserDataTest, BuildStudiesUserDataIfNoFieldTrials) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(base::test::ParseJsonDict(R"({"studies":[]})"),
            BuildStudiesUserData());
}

TEST(BraveAdsStudiesUserDataTest, BuildStudiesUserData) {
  // Arrange
  const scoped_refptr<base::FieldTrial> field_trial_1 =
      base::FieldTrialList::CreateFieldTrial("BraveAds.FooStudy", "GroupA");
  field_trial_1->group_name();

  const scoped_refptr<base::FieldTrial> field_trial_2 =
      base::FieldTrialList::CreateFieldTrial("BraveAds.BarStudy", "GroupB");
  field_trial_2->group_name();

  const scoped_refptr<base::FieldTrial> field_trial_3 =
      base::FieldTrialList::CreateFieldTrial("FooBarStudy", "GroupC");
  field_trial_3->group_name();

  ASSERT_EQ(3U, base::FieldTrialList::GetFieldTrialCount());

  // Act

  // Assert
  EXPECT_EQ(
      base::test::ParseJsonDict(
          R"({"studies":[{"group":"GroupB","name":"BraveAds.BarStudy"},{"group":"GroupA","name":"BraveAds.FooStudy"}]})"),
      BuildStudiesUserData());
}

}  // namespace brave_ads
