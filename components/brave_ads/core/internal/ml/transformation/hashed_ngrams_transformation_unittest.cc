/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/ml/transformation/hashed_ngrams_transformation.h"

#include "brave/components/brave_ads/core/internal/common/unittest/unittest_base.h"
#include "brave/components/brave_ads/core/internal/ml/data/text_data.h"
#include "brave/components/brave_ads/core/internal/ml/data/vector_data.h"

// npm run test -- brave_unit_tests --filter=BraveAds*

namespace brave_ads::ml {

class BraveAdsHashedNGramsTransformationTest : public UnitTestBase {};

TEST_F(BraveAdsHashedNGramsTransformationTest, HashingTest) {
  // Arrange
  constexpr size_t kDefaultBucketCount = 10'000;
  constexpr char kTestString[] = "tiny";
  const std::unique_ptr<Data> text_data =
      std::make_unique<TextData>(kTestString);

  const HashedNGramsTransformation hashed_ngrams;

  // Act
  const std::unique_ptr<Data> hashed_data = hashed_ngrams.Apply(text_data);

  ASSERT_EQ(hashed_data->GetType(), DataType::kVector);

  const VectorData* const hashed_vector_data =
      static_cast<VectorData*>(hashed_data.get());

  // Assert
  // 10000 is the default size
  ASSERT_EQ(kDefaultBucketCount, hashed_vector_data->GetDimensionCount());

  // Hashes for [t, i, n, y, ti, in, ny, tin, iny, tiny] -- 10 in total
  EXPECT_EQ(10U, hashed_vector_data->GetData().size());
}

TEST_F(BraveAdsHashedNGramsTransformationTest, CustomHashingTest) {
  // Arrange
  constexpr size_t kHashBucketCount = 3;
  constexpr char kTestString[] = "tiny";
  const std::unique_ptr<Data> text_data =
      std::make_unique<TextData>(kTestString);

  const HashedNGramsTransformation hashed_ngrams(kHashBucketCount,
                                                 std::vector<int>{1, 2, 3});

  // Act
  const std::unique_ptr<Data> hashed_data = hashed_ngrams.Apply(text_data);

  ASSERT_EQ(DataType::kVector, hashed_data->GetType());

  const VectorData* const hashed_vector_data =
      static_cast<VectorData*>(hashed_data.get());

  // Assert
  ASSERT_EQ(kHashBucketCount, hashed_vector_data->GetDimensionCount());
  EXPECT_EQ(kHashBucketCount, hashed_vector_data->GetData().size());
}

}  // namespace brave_ads::ml
