/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/account/confirmations/opted_in_info.h"

namespace brave_ads {

OptedInInfo::OptedInInfo() = default;

OptedInInfo::OptedInInfo(const OptedInInfo& other) {
  *this = other;
}

OptedInInfo& OptedInInfo::operator=(const OptedInInfo& other) {
  if (this != &other) {
    token = other.token;
    blinded_token = other.blinded_token;
    unblinded_token = other.unblinded_token;
    user_data = other.user_data;
    credential_base64url = other.credential_base64url;
  }

  return *this;
}

OptedInInfo::OptedInInfo(OptedInInfo&& other) noexcept = default;

OptedInInfo& OptedInInfo::operator=(OptedInInfo&& other) noexcept = default;

OptedInInfo::~OptedInInfo() = default;

bool operator==(const OptedInInfo& lhs, const OptedInInfo& rhs) {
  return lhs.token == rhs.token && lhs.blinded_token == rhs.blinded_token &&
         lhs.unblinded_token == rhs.unblinded_token &&
         lhs.user_data == rhs.user_data &&
         lhs.credential_base64url == rhs.credential_base64url;
}

bool operator!=(const OptedInInfo& lhs, const OptedInInfo& rhs) {
  return !(lhs == rhs);
}

}  // namespace brave_ads
