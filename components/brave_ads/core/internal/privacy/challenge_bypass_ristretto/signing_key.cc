/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/privacy/challenge_bypass_ristretto/signing_key.h"

#include "brave/components/brave_ads/core/internal/privacy/challenge_bypass_ristretto/blinded_token.h"
#include "brave/components/brave_ads/core/internal/privacy/challenge_bypass_ristretto/challenge_bypass_ristretto_util.h"
#include "brave/components/brave_ads/core/internal/privacy/challenge_bypass_ristretto/public_key.h"
#include "brave/components/brave_ads/core/internal/privacy/challenge_bypass_ristretto/signed_token.h"
#include "brave/components/brave_ads/core/internal/privacy/challenge_bypass_ristretto/token_preimage.h"
#include "brave/components/brave_ads/core/internal/privacy/challenge_bypass_ristretto/unblinded_token.h"

namespace brave_ads::privacy::cbr {

namespace {

absl::optional<challenge_bypass_ristretto::SigningKey> Create(
    const std::string& signing_key_base64) {
  if (signing_key_base64.empty()) {
    return absl::nullopt;
  }

  return ValueOrLogError(challenge_bypass_ristretto::SigningKey::decode_base64(
      signing_key_base64));
}

}  // namespace

SigningKey::SigningKey()
    : signing_key_(
          ValueOrLogError(challenge_bypass_ristretto::SigningKey::random())) {}

SigningKey::SigningKey(const std::string& signing_key_base64)
    : signing_key_(Create(signing_key_base64)) {}

SigningKey::SigningKey(
    const challenge_bypass_ristretto::SigningKey& signing_key)
    : signing_key_(signing_key) {}

SigningKey::~SigningKey() = default;

bool SigningKey::operator==(const SigningKey& other) const {
  return EncodeBase64().value_or("") == other.EncodeBase64().value_or("");
}

bool SigningKey::operator!=(const SigningKey& other) const {
  return !(*this == other);
}

SigningKey SigningKey::DecodeBase64(const std::string& signing_key_base64) {
  return SigningKey(signing_key_base64);
}

absl::optional<std::string> SigningKey::EncodeBase64() const {
  if (!signing_key_ || !has_value()) {
    return absl::nullopt;
  }

  return ValueOrLogError(signing_key_->encode_base64());
}

absl::optional<SignedToken> SigningKey::Sign(
    const BlindedToken& blinded_token) const {
  if (!signing_key_ || !has_value() || !blinded_token.has_value()) {
    return absl::nullopt;
  }

  return ValueOrLogError<challenge_bypass_ristretto::SignedToken, SignedToken>(
      signing_key_->sign(blinded_token.get()));
}

absl::optional<UnblindedToken> SigningKey::RederiveUnblindedToken(
    const TokenPreimage& token_preimage) {
  if (!signing_key_ || !has_value() || !token_preimage.has_value()) {
    return absl::nullopt;
  }

  return ValueOrLogError<challenge_bypass_ristretto::UnblindedToken,
                         UnblindedToken>(
      signing_key_->rederive_unblinded_token(token_preimage.get()));
}

absl::optional<PublicKey> SigningKey::GetPublicKey() {
  if (!signing_key_ || !has_value()) {
    return absl::nullopt;
  }

  return PublicKey(signing_key_->public_key());
}

std::ostream& operator<<(std::ostream& os, const SigningKey& signing_key) {
  os << signing_key.EncodeBase64().value_or("");
  return os;
}

}  // namespace brave_ads::privacy::cbr
