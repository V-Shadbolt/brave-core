/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/category_content_info.h"

namespace brave_ads {

bool operator==(const CategoryContentInfo& lhs,
                const CategoryContentInfo& rhs) {
  return lhs.category == rhs.category &&
         lhs.user_reaction_type == rhs.user_reaction_type;
}

bool operator!=(const CategoryContentInfo& lhs,
                const CategoryContentInfo& rhs) {
  return !(lhs == rhs);
}

}  // namespace brave_ads
