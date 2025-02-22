/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <memory>

#include "brave/components/brave_wallet/browser/tx_state_manager.h"

#include "base/run_loop.h"
#include "base/scoped_observation.h"
#include "base/test/bind.h"
#include "base/test/task_environment.h"
#include "base/test/values_test_util.h"
#include "base/time/time.h"
#include "base/values.h"
#include "brave/components/brave_wallet/browser/brave_wallet_prefs.h"
#include "brave/components/brave_wallet/browser/brave_wallet_utils.h"
#include "brave/components/brave_wallet/browser/eth_tx_meta.h"
#include "brave/components/brave_wallet/browser/eth_tx_state_manager.h"
#include "brave/components/brave_wallet/browser/pref_names.h"
#include "brave/components/brave_wallet/common/brave_wallet.mojom.h"
#include "brave/components/brave_wallet/common/test_utils.h"
#include "components/prefs/pref_service.h"
#include "components/sync_preferences/testing_pref_service_syncable.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

using base::test::ParseJson;
using base::test::ParseJsonDict;
using testing::_;

namespace brave_wallet {

class MockTxStateManagerObserver : public TxStateManager::Observer {
 public:
  explicit MockTxStateManagerObserver(TxStateManager* tx_state_manager) {
    observation_.Observe(tx_state_manager);
  }

  MOCK_METHOD1(OnTransactionStatusChanged, void(mojom::TransactionInfoPtr));
  MOCK_METHOD1(OnNewUnapprovedTx, void(mojom::TransactionInfoPtr));

 private:
  base::ScopedObservation<TxStateManager, TxStateManager::Observer>
      observation_{this};
};

class TxStateManagerUnitTest : public testing::Test {
 public:
  TxStateManagerUnitTest() {}

 protected:
  void SetUp() override {
    brave_wallet::RegisterProfilePrefs(prefs_.registry());
    brave_wallet::RegisterProfilePrefsForMigration(prefs_.registry());
    // The only different between each coin type's tx state manager in these
    // base functions are their pref paths, so here we just use
    // EthTxStateManager to test common methods in TxStateManager.
    tx_state_manager_ = std::make_unique<EthTxStateManager>(&prefs_);
  }

  base::test::TaskEnvironment task_environment_;
  sync_preferences::TestingPrefServiceSyncable prefs_;
  std::unique_ptr<TxStateManager> tx_state_manager_;
};

TEST_F(TxStateManagerUnitTest, TxOperations) {
  prefs_.ClearPref(kBraveWalletTransactions);

  EthTxMeta meta;
  meta.set_id("001");
  meta.set_chain_id(mojom::kMainnetChainId);
  EXPECT_FALSE(prefs_.HasPrefPath(kBraveWalletTransactions));
  // Add
  tx_state_manager_->AddOrUpdateTx(meta);
  EXPECT_TRUE(prefs_.HasPrefPath(kBraveWalletTransactions));
  {
    const auto& dict = prefs_.GetDict(kBraveWalletTransactions);
    EXPECT_EQ(dict.size(), 1u);
    const auto* ethereum_dict = dict.FindDict("ethereum");
    ASSERT_TRUE(ethereum_dict);
    EXPECT_EQ(ethereum_dict->size(), 1u);
    const auto* network_dict = ethereum_dict->FindDict("mainnet");
    ASSERT_TRUE(network_dict);
    EXPECT_EQ(network_dict->size(), 1u);
    const base::Value::Dict* value = network_dict->FindDict("001");
    ASSERT_TRUE(value);
    auto meta_from_value = tx_state_manager_->ValueToTxMeta(*value);
    ASSERT_NE(meta_from_value, nullptr);
    EXPECT_EQ(*static_cast<EthTxMeta*>(meta_from_value.get()), meta);
  }

  meta.set_tx_hash("0xabcd");
  // Update
  tx_state_manager_->AddOrUpdateTx(meta);
  {
    const auto& dict = prefs_.GetDict(kBraveWalletTransactions);
    EXPECT_EQ(dict.size(), 1u);
    const auto* ethereum_dict = dict.FindDict("ethereum");
    ASSERT_TRUE(ethereum_dict);
    EXPECT_EQ(ethereum_dict->size(), 1u);
    const auto* network_dict = ethereum_dict->FindDict("mainnet");
    ASSERT_TRUE(network_dict);
    EXPECT_EQ(network_dict->size(), 1u);
    const base::Value::Dict* value = network_dict->FindDict("001");
    ASSERT_TRUE(value);
    auto meta_from_value = tx_state_manager_->ValueToTxMeta(*value);
    ASSERT_NE(meta_from_value, nullptr);
    EXPECT_EQ(meta_from_value->tx_hash(), meta.tx_hash());
  }

  meta.set_id("002");
  meta.set_tx_hash("0xabff");
  // Add another one
  tx_state_manager_->AddOrUpdateTx(meta);
  {
    const auto& dict = prefs_.GetDict(kBraveWalletTransactions);
    EXPECT_EQ(dict.size(), 1u);
    const auto* ethereum_dict = dict.FindDict("ethereum");
    ASSERT_TRUE(ethereum_dict);
    EXPECT_EQ(ethereum_dict->size(), 1u);
    const auto* network_dict = ethereum_dict->FindDict("mainnet");
    ASSERT_TRUE(network_dict);
    EXPECT_EQ(network_dict->size(), 2u);
  }

  // Get
  {
    auto meta_fetched = tx_state_manager_->GetTx(mojom::kMainnetChainId, "001");
    ASSERT_NE(meta_fetched, nullptr);
    ASSERT_EQ(tx_state_manager_->GetTx(mojom::kMainnetChainId, "003"), nullptr);
    ASSERT_EQ(tx_state_manager_->GetTx(mojom::kGoerliChainId, "001"), nullptr);
    EXPECT_EQ(meta_fetched->id(), "001");
    EXPECT_EQ(meta_fetched->tx_hash(), "0xabcd");

    auto meta_fetched2 =
        tx_state_manager_->GetTx(mojom::kMainnetChainId, "002");
    ASSERT_NE(meta_fetched2, nullptr);
    EXPECT_EQ(meta_fetched2->id(), "002");
    EXPECT_EQ(meta_fetched2->tx_hash(), "0xabff");

    auto meta_fetched3 = tx_state_manager_->GetTx(mojom::kMainnetChainId, "");
    EXPECT_EQ(meta_fetched3, nullptr);
  }

  // Delete
  tx_state_manager_->DeleteTx(mojom::kMainnetChainId, "001");
  {
    const auto& dict = prefs_.GetDict(kBraveWalletTransactions);
    EXPECT_EQ(dict.size(), 1u);
    const auto* ethereum_dict = dict.FindDict("ethereum");
    ASSERT_TRUE(ethereum_dict);
    EXPECT_EQ(ethereum_dict->size(), 1u);
    const auto* network_dict = ethereum_dict->FindDict("mainnet");
    ASSERT_TRUE(network_dict);
    EXPECT_EQ(network_dict->size(), 1u);
  }

  // Purge
  tx_state_manager_->WipeTxs();
  EXPECT_TRUE(prefs_.HasPrefPath(kBraveWalletTransactions));
  EXPECT_FALSE(
      prefs_.HasPrefPath(std::string(kBraveWalletTransactions) + ".ethereum"));
}

TEST_F(TxStateManagerUnitTest, GetTransactionsByStatus) {
  prefs_.ClearPref(kBraveWalletTransactions);

  std::string addr1 = "0x3535353535353535353535353535353535353535";
  std::string addr2 = "0x2f015c60e0be116b1f0cd534704db9c92118fb6a";

  for (size_t i = 0; i < 20; ++i) {
    EthTxMeta meta;
    meta.set_from("0x3333333333333333333333333333333333333333");
    meta.set_id(base::NumberToString(i));
    if (i % 2 == 0) {
      if (i % 4 == 0) {
        meta.set_from(addr1);
      }
      if (i % 6 == 0) {
        meta.set_chain_id(mojom::kMainnetChainId);
      } else {
        meta.set_chain_id(mojom::kGoerliChainId);
      }
      meta.set_status(mojom::TransactionStatus::Confirmed);
    } else {
      if (i % 5 == 0) {
        meta.set_from(addr2);
      }
      if (i % 7 == 0) {
        meta.set_chain_id(mojom::kMainnetChainId);
      } else {
        meta.set_chain_id(mojom::kGoerliChainId);
      }
      meta.set_status(mojom::TransactionStatus::Submitted);
    }
    tx_state_manager_->AddOrUpdateTx(meta);
  }

  EXPECT_EQ(
      tx_state_manager_
          ->GetTransactionsByStatus(
              absl::nullopt, mojom::TransactionStatus::Approved, absl::nullopt)
          .size(),
      0u);
  EXPECT_EQ(
      tx_state_manager_
          ->GetTransactionsByStatus(
              absl::nullopt, mojom::TransactionStatus::Confirmed, absl::nullopt)
          .size(),
      10u);
  EXPECT_EQ(tx_state_manager_
                ->GetTransactionsByStatus(mojom::kMainnetChainId,
                                          mojom::TransactionStatus::Confirmed,
                                          absl::nullopt)
                .size(),
            4u);
  EXPECT_EQ(tx_state_manager_
                ->GetTransactionsByStatus(mojom::kGoerliChainId,
                                          mojom::TransactionStatus::Confirmed,
                                          absl::nullopt)
                .size(),
            6u);
  EXPECT_EQ(
      tx_state_manager_
          ->GetTransactionsByStatus(
              absl::nullopt, mojom::TransactionStatus::Submitted, absl::nullopt)
          .size(),
      10u);
  EXPECT_EQ(tx_state_manager_
                ->GetTransactionsByStatus(mojom::kMainnetChainId,
                                          mojom::TransactionStatus::Submitted,
                                          absl::nullopt)
                .size(),
            1u);
  EXPECT_EQ(tx_state_manager_
                ->GetTransactionsByStatus(mojom::kGoerliChainId,
                                          mojom::TransactionStatus::Submitted,
                                          absl::nullopt)
                .size(),
            9u);

  EXPECT_EQ(tx_state_manager_
                ->GetTransactionsByStatus(
                    absl::nullopt, mojom::TransactionStatus::Approved, addr1)
                .size(),
            0u);

  EXPECT_EQ(
      tx_state_manager_
          ->GetTransactionsByStatus(absl::nullopt, absl::nullopt, absl::nullopt)
          .size(),
      20u);
  EXPECT_EQ(tx_state_manager_
                ->GetTransactionsByStatus(absl::nullopt, absl::nullopt, addr1)
                .size(),
            5u);
  EXPECT_EQ(tx_state_manager_
                ->GetTransactionsByStatus(mojom::kMainnetChainId, absl::nullopt,
                                          addr1)
                .size(),
            2u);
  EXPECT_EQ(
      tx_state_manager_
          ->GetTransactionsByStatus(mojom::kGoerliChainId, absl::nullopt, addr1)
          .size(),
      3u);
  EXPECT_EQ(tx_state_manager_
                ->GetTransactionsByStatus(absl::nullopt, absl::nullopt, addr2)
                .size(),
            2u);
  EXPECT_EQ(tx_state_manager_
                ->GetTransactionsByStatus(mojom::kMainnetChainId, absl::nullopt,
                                          addr2)
                .size(),
            0u);
  EXPECT_EQ(
      tx_state_manager_
          ->GetTransactionsByStatus(mojom::kGoerliChainId, absl::nullopt, addr2)
          .size(),
      2u);

  auto confirmed_addr1 = tx_state_manager_->GetTransactionsByStatus(
      absl::nullopt, mojom::TransactionStatus::Confirmed, addr1);
  EXPECT_EQ(confirmed_addr1.size(), 5u);
  for (const auto& meta : confirmed_addr1) {
    unsigned id;
    ASSERT_TRUE(base::StringToUint(meta->id(), &id));
    EXPECT_EQ(id % 4, 0u);
  }

  auto submitted_addr2 = tx_state_manager_->GetTransactionsByStatus(
      absl::nullopt, mojom::TransactionStatus::Submitted, addr2);
  EXPECT_EQ(submitted_addr2.size(), 2u);
  for (const auto& meta : submitted_addr2) {
    unsigned id;
    ASSERT_TRUE(base::StringToUint(meta->id(), &id));
    EXPECT_EQ(id % 5, 0u);
  }
}

TEST_F(TxStateManagerUnitTest, MultiChainId) {
  prefs_.ClearPref(kBraveWalletTransactions);

  EthTxMeta meta;
  meta.set_id("001");
  meta.set_chain_id(mojom::kMainnetChainId);
  tx_state_manager_->AddOrUpdateTx(meta);

  EXPECT_EQ(tx_state_manager_->GetTx(mojom::kGoerliChainId, "001"), nullptr);
  meta.set_chain_id(mojom::kGoerliChainId);
  tx_state_manager_->AddOrUpdateTx(meta);

  EXPECT_EQ(tx_state_manager_->GetTx(mojom::kLocalhostChainId, "001"), nullptr);
  meta.set_chain_id(mojom::kLocalhostChainId);
  tx_state_manager_->AddOrUpdateTx(meta);

  const auto& dict = prefs_.GetDict(kBraveWalletTransactions);
  EXPECT_EQ(dict.size(), 1u);
  const auto* ethereum_dict = dict.FindDict("ethereum");
  ASSERT_TRUE(ethereum_dict);
  EXPECT_EQ(ethereum_dict->size(), 3u);
  const auto* mainnet_dict = ethereum_dict->FindDict("mainnet");
  ASSERT_TRUE(mainnet_dict);
  EXPECT_EQ(mainnet_dict->size(), 1u);
  EXPECT_TRUE(mainnet_dict->FindDict("001"));
  const auto* goerli_dict = ethereum_dict->FindDict("goerli");
  ASSERT_TRUE(goerli_dict);
  EXPECT_EQ(goerli_dict->size(), 1u);
  EXPECT_TRUE(goerli_dict->FindDict("001"));
  auto localhost_url_spec =
      brave_wallet::GetNetworkURL(&prefs_, mojom::kLocalhostChainId,
                                  mojom::CoinType::ETH)
          .spec();
  const auto* localhost_dict = ethereum_dict->FindDict(localhost_url_spec);
  ASSERT_TRUE(localhost_dict);
  EXPECT_EQ(localhost_dict->size(), 1u);
  EXPECT_TRUE(localhost_dict->FindDict("001"));
}

TEST_F(TxStateManagerUnitTest, RetireOldTxMeta) {
  prefs_.ClearPref(kBraveWalletTransactions);

  for (size_t i = 0; i < 20; ++i) {
    EthTxMeta meta;
    meta.set_id(base::NumberToString(i));
    meta.set_chain_id(mojom::kMainnetChainId);
    if (i % 2 == 0) {
      meta.set_status(mojom::TransactionStatus::Confirmed);
      meta.set_confirmed_time(base::Time::Now());
    } else {
      meta.set_status(mojom::TransactionStatus::Rejected);
      meta.set_created_time(base::Time::Now());
    }
    tx_state_manager_->AddOrUpdateTx(meta);
  }

  EXPECT_TRUE(tx_state_manager_->GetTx(mojom::kMainnetChainId, "0"));
  EthTxMeta meta21;
  meta21.set_id("20");
  meta21.set_chain_id(mojom::kMainnetChainId);
  meta21.set_status(mojom::TransactionStatus::Confirmed);
  meta21.set_confirmed_time(base::Time::Now());
  tx_state_manager_->AddOrUpdateTx(meta21);
  EXPECT_FALSE(tx_state_manager_->GetTx(mojom::kMainnetChainId, "0"));

  EXPECT_TRUE(tx_state_manager_->GetTx(mojom::kMainnetChainId, "1"));
  EthTxMeta meta22;
  meta22.set_id("21");
  meta22.set_chain_id(mojom::kMainnetChainId);
  meta22.set_status(mojom::TransactionStatus::Rejected);
  meta22.set_created_time(base::Time::Now());
  tx_state_manager_->AddOrUpdateTx(meta22);
  EXPECT_FALSE(tx_state_manager_->GetTx(mojom::kMainnetChainId, "1"));

  // Other status doesn't matter
  EXPECT_TRUE(tx_state_manager_->GetTx(mojom::kMainnetChainId, "2"));
  EXPECT_TRUE(tx_state_manager_->GetTx(mojom::kMainnetChainId, "3"));
  EthTxMeta meta23;
  meta23.set_id("22");
  meta23.set_chain_id(mojom::kMainnetChainId);
  meta23.set_status(mojom::TransactionStatus::Submitted);
  meta23.set_created_time(base::Time::Now());
  tx_state_manager_->AddOrUpdateTx(meta23);
  EXPECT_TRUE(tx_state_manager_->GetTx(mojom::kMainnetChainId, "2"));
  EXPECT_TRUE(tx_state_manager_->GetTx(mojom::kMainnetChainId, "3"));

  // Other chain id doesn't matter
  EthTxMeta meta24;
  meta24.set_id("23");
  meta23.set_chain_id(mojom::kGoerliChainId);
  meta24.set_status(mojom::TransactionStatus::Confirmed);
  meta24.set_created_time(base::Time::Now());
  tx_state_manager_->AddOrUpdateTx(meta24);
  EXPECT_TRUE(tx_state_manager_->GetTx(mojom::kMainnetChainId, "2"));
  EXPECT_TRUE(tx_state_manager_->GetTx(mojom::kMainnetChainId, "3"));
}

TEST_F(TxStateManagerUnitTest, Observer) {
  prefs_.ClearPref(kBraveWalletTransactions);
  MockTxStateManagerObserver observer(tx_state_manager_.get());

  EthTxMeta meta;
  meta.set_id("001");
  // Add
  EXPECT_CALL(observer,
              OnNewUnapprovedTx(EqualsMojo(meta.ToTransactionInfo())));
  EXPECT_CALL(observer, OnTransactionStatusChanged(_)).Times(0);
  tx_state_manager_->AddOrUpdateTx(meta);
  EXPECT_TRUE(testing::Mock::VerifyAndClearExpectations(&observer));

  // Modify
  meta.set_status(mojom::TransactionStatus::Approved);
  EXPECT_CALL(observer, OnNewUnapprovedTx(_)).Times(0);
  EXPECT_CALL(observer,
              OnTransactionStatusChanged(EqualsMojo(meta.ToTransactionInfo())))
      .Times(1);
  tx_state_manager_->AddOrUpdateTx(meta);
  EXPECT_TRUE(testing::Mock::VerifyAndClearExpectations(&observer));
}

TEST_F(TxStateManagerUnitTest,
       MigrateSolanaTransactionsForV0TransactionsSupport) {
  ASSERT_FALSE(
      prefs_.GetBoolean(kBraveWalletSolanaTransactionsV0SupportMigrated));
  base::Value txs_value = ParseJson(R"(
    {
      "solana": {
        "devnet": {
          "tx_id1": {
            "tx": {
              "message": {
                "fee_payer": "3Lu176FQzbQJCc8iL9PnmALbpMPhZeknoturApnXRDJw",
                "instructions": [
                  {
                    "accounts": [],
                    "data": "SGVsbG8sIGZyb20gdGhlIFNvbGFuYSBXYWxsZXQgQWRhcHRlciBleGFtcGxlIGFwcCE=",
                    "program_id": "MemoSq4gqABAXKb96qnH8TysNcWxMyWCqXgDLGmfcHr"
                  }
                ],
                "last_valid_block_height": "0",
                "recent_blockhash":
                    "GZH3GWCMxU9aZbai9L8pA3aTWsBVaCwYxiWfhtnMhUbb"
              }
            }
          }
        },
        "mainnet": {
          "tx_id2": {
            "tx": {
              "message": {
                "fee_payer": "3Lu176FQzbQJCc8iL9PnmALbpMPhZeknoturApnXRDJw",
                "instructions": [
                  {
                    "accounts": [
                      {
                        "is_signer": true,
                        "is_writable": true,
                        "pubkey": "3Lu176FQzbQJCc8iL9PnmALbpMPhZeknoturApnXRDJw"
                      },
                      {
                        "is_signer": true,
                        "is_writable": true,
                        "pubkey": "3Lu176FQzbQJCc8iL9PnmALbpMPhZeknoturApnXRDJw"
                      }
                    ],
                    "data": "AgAAAGQAAAAAAAAA",
                    "decoded_data": {
                      "account_params": [
                        {
                          "localized_name": "From Account",
                          "name": "from_account"
                        },
                        {
                          "localized_name": "To Account",
                          "name": "to_account"
                        }
                      ],
                      "params": [
                        {
                          "localized_name": "Lamports",
                          "name": "lamports",
                          "type": 2,
                          "value": "100"
                        }
                      ],
                      "sys_ins_type": "2"
                    },
                    "program_id": "11111111111111111111111111111111"
                  }
                ],
                "last_valid_block_height": "0",
                "recent_blockhash":
                    "AARGss1frfvBSKqYXLHuv3i4kzbQrHhabubcF2KFTE2S"
              }
            }
          },
          "tx_id3": {
            "tx": {
              "message": {
                "fee_payer": "3Lu176FQzbQJCc8iL9PnmALbpMPhZeknoturApnXRDJw",
                "instructions": [
                  {
                    "accounts": [],
                    "data": "SGVsbG8sIGZyb20gdGhlIFNvbGFuYSBXYWxsZXQgQWRhcHRlciBleGFtcGxlIGFwcCE=",
                    "program_id": "MemoSq4gqABAXKb96qnH8TysNcWxMyWCqXgDLGmfcHr"
                  }
                ],
                "last_valid_block_height": "0",
                "recent_blockhash":
                    "GZH3GWCMxU9aZbai9L8pA3aTWsBVaCwYxiWfhtnMhUbb"
              }
            }
          }
        }
      }
    })");

  prefs_.Set(kBraveWalletTransactions, txs_value);
  TxStateManager::MigrateSolanaTransactionsForV0TransactionsSupport(&prefs_);
  base::Value::Dict msg1 = ParseJsonDict(R"({
      "version": 0,
      "fee_payer": "3Lu176FQzbQJCc8iL9PnmALbpMPhZeknoturApnXRDJw",
      "message_header": {
        "num_readonly_signed_accounts": "0",
        "num_readonly_unsigned_accounts": "1",
        "num_required_signatures": "1"
      },
      "static_account_keys": [
        "3Lu176FQzbQJCc8iL9PnmALbpMPhZeknoturApnXRDJw",
        "MemoSq4gqABAXKb96qnH8TysNcWxMyWCqXgDLGmfcHr"
      ],
      "address_table_lookups": [],
      "instructions": [
        {
          "accounts": [],
          "data": "SGVsbG8sIGZyb20gdGhlIFNvbGFuYSBXYWxsZXQgQWRhcHRlciBleGFtcGxlIGFwcCE=",
          "program_id": "MemoSq4gqABAXKb96qnH8TysNcWxMyWCqXgDLGmfcHr"
        }
      ],
      "last_valid_block_height": "0",
      "recent_blockhash": "GZH3GWCMxU9aZbai9L8pA3aTWsBVaCwYxiWfhtnMhUbb"
  })");
  base::Value::Dict msg2 = ParseJsonDict(R"({
      "version": 0,
      "fee_payer": "3Lu176FQzbQJCc8iL9PnmALbpMPhZeknoturApnXRDJw",
      "message_header": {
        "num_readonly_signed_accounts": "0",
        "num_readonly_unsigned_accounts": "1",
        "num_required_signatures": "1"
      },
      "static_account_keys": [
        "3Lu176FQzbQJCc8iL9PnmALbpMPhZeknoturApnXRDJw",
        "11111111111111111111111111111111"
      ],
      "address_table_lookups": [],
      "instructions": [
        {
          "accounts": [
            {
              "is_signer": true,
              "is_writable": true,
              "pubkey": "3Lu176FQzbQJCc8iL9PnmALbpMPhZeknoturApnXRDJw"
            },
            {
              "is_signer": true,
              "is_writable": true,
              "pubkey": "3Lu176FQzbQJCc8iL9PnmALbpMPhZeknoturApnXRDJw"
            }
          ],
          "data": "AgAAAGQAAAAAAAAA",
          "decoded_data": {
            "account_params": [
              {
                "localized_name": "From Account",
                "name": "from_account"
              },
              {
                "localized_name": "To Account",
                "name": "to_account"
              }
            ],
            "params": [
              {
                "localized_name": "Lamports",
                "name": "lamports",
                "type": 2,
                "value": "100"
              }
            ],
            "sys_ins_type": "2"
          },
          "program_id": "11111111111111111111111111111111"
        }
      ],
      "last_valid_block_height": "0",
      "recent_blockhash": "AARGss1frfvBSKqYXLHuv3i4kzbQrHhabubcF2KFTE2S"
  })");
  EXPECT_EQ(*prefs_.GetDict(kBraveWalletTransactions)
                 .FindDictByDottedPath("solana.devnet.tx_id1.tx.message"),
            msg1);
  EXPECT_EQ(*prefs_.GetDict(kBraveWalletTransactions)
                 .FindDictByDottedPath("solana.mainnet.tx_id2.tx.message"),
            msg2);
  EXPECT_EQ(*prefs_.GetDict(kBraveWalletTransactions)
                 .FindDictByDottedPath("solana.mainnet.tx_id3.tx.message"),
            msg1);

  EXPECT_TRUE(
      prefs_.GetBoolean(kBraveWalletSolanaTransactionsV0SupportMigrated));
}

}  // namespace brave_wallet
