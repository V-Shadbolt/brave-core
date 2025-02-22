/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/account/confirmations/redeem_confirmation/url_request_builders/create_opted_in_confirmation_url_request_builder.h"

#include <utility>
#include <vector>

#include "base/check.h"
#include "base/strings/strcat.h"
#include "brave/components/brave_ads/common/interfaces/brave_ads.mojom.h"
#include "brave/components/brave_ads/core/internal/account/confirmations/confirmation_payload_json_writer.h"
#include "brave/components/brave_ads/core/internal/account/confirmations/confirmation_util.h"
#include "brave/components/brave_ads/core/internal/account/confirmations/redeem_confirmation/url_request_builders/create_opted_in_confirmation_url_request_builder_util.h"
#include "brave/components/brave_ads/core/internal/common/url/request_builder/host/url_host_util.h"
#include "url/gurl.h"

namespace brave_ads {

namespace {

std::vector<std::string> BuildHeaders() {
  std::vector<std::string> headers;

  const std::string accept_header = "accept: application/json";
  headers.push_back(accept_header);

  return headers;
}

}  // namespace

CreateOptedInConfirmationUrlRequestBuilder::
    CreateOptedInConfirmationUrlRequestBuilder(ConfirmationInfo confirmation)
    : confirmation_(std::move(confirmation)) {
  CHECK(IsValid(confirmation_));
}

mojom::UrlRequestInfoPtr CreateOptedInConfirmationUrlRequestBuilder::Build() {
  mojom::UrlRequestInfoPtr url_request = mojom::UrlRequestInfo::New();
  url_request->url = BuildUrl();
  url_request->headers = BuildHeaders();
  url_request->content = BuildBody();
  url_request->content_type = "application/json";
  url_request->method = mojom::UrlRequestMethodType::kPost;

  return url_request;
}

///////////////////////////////////////////////////////////////////////////////

GURL CreateOptedInConfirmationUrlRequestBuilder::BuildUrl() const {
  CHECK(confirmation_.opted_in);
  CHECK(confirmation_.opted_in->credential_base64url);

  const std::string url_host = confirmation_.ad_type == AdType::kSearchResultAd
                                   ? GetAnonymousSearchUrlHost()
                                   : GetAnonymousUrlHost();

  const std::string credential_base64_url =
      *confirmation_.opted_in->credential_base64url;

  const std::string spec = base::StrCat(
      {url_host, BuildCreateOptedInConfirmationUrlPath(
                     confirmation_.transaction_id, credential_base64_url)});

  return GURL(spec);
}

std::string CreateOptedInConfirmationUrlRequestBuilder::BuildBody() const {
  return json::writer::WriteConfirmationPayload(confirmation_);
}

}  // namespace brave_ads
