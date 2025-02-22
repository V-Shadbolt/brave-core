/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import { action } from 'typesafe-actions'
import { types } from './rewards_types'
import * as mojom from '../../shared/lib/mojom'

export const isInitialized = () => action(types.IS_INITIALIZED)

export const onSettingSave = (key: string, value: any, persist: boolean = true) => action(types.ON_SETTING_SAVE, {
  key,
  value,
  persist
})

export const onUserType =
  (userType: number) => action(types.ON_USER_TYPE, { userType })

export const getUserType = () => action(types.GET_USER_TYPE)

export const getRewardsParameters = () => action(types.GET_REWARDS_PARAMETERS)

export const onRewardsParameters = (properties: Rewards.RewardsParameters) =>
  action(types.ON_REWARDS_PARAMETERS, {
    properties
  })

export const getAutoContributeProperties = () => action(types.GET_AUTO_CONTRIBUTE_PROPERTIES)

export const onAutoContributeProperties = (properties: any) => action(types.ON_AUTO_CONTRIBUTE_PROPERTIES, {
  properties
})

export const fetchPromotions = () => action(types.FETCH_PROMOTIONS)

export const onPromotions = (properties: Rewards.PromotionResponse) => action(types.ON_PROMOTIONS, {
  properties
})

export const claimPromotion = (promotionId: string) => action(types.CLAIM_PROMOTION, {
  promotionId
})

export const onPromotionClaimStarted = (promotionId: string) => action(types.ON_PROMOTION_CLAIM_STARTED, {
  promotionId
})

export const onPromotionFinish = (properties: Rewards.PromotionFinish) => action(types.ON_PROMOTION_FINISH, {
  properties
})

export const deletePromotion = (promotionId: string) => action(types.DELETE_PROMOTION, {
  promotionId
})

export const onModalResetClose = () => action(types.ON_MODAL_RESET_CLOSE)

export const onModalResetOpen = () => action(types.ON_MODAL_RESET_OPEN)

export const onModalConnectClose = () => action(types.ON_MODAL_CONNECT_CLOSE)

export const onModalConnectOpen = () => action(types.ON_MODAL_CONNECT_OPEN)

export const onClearAlert = (property: string) => action(types.ON_CLEAR_ALERT, {
  property
})

export const onReconcileStamp = (stamp: number) => action(types.ON_RECONCILE_STAMP, {
  stamp
})

export const onContributeList = (list: Rewards.Publisher[]) => action(types.ON_CONTRIBUTE_LIST, {
  list
})

export const onExcludedList = (list: Rewards.ExcludedPublisher[]) => action(types.ON_EXCLUDED_LIST, {
  list
})

export const onBalanceReport = (properties: { month: number, year: number, report: Rewards.BalanceReport }) => action(types.ON_BALANCE_REPORT, {
  month: properties.month,
  year: properties.year,
  report: properties.report
})

export const onExternalWalletProviderList = (list: Rewards.ExternalWalletProvider[]) => action(types.ON_EXTERNAL_WALLET_PROVIDER_LIST, {
  list
})

export const setExternalWalletType = (provider: string) => action(types.SET_EXTERNAL_WALLET_TYPE, {
  provider
})

export const getBalanceReport = (month: number, year: number) => action(types.GET_BALANCE_REPORT, {
  month,
  year
})

export const excludePublisher = (publisherKey: string) => action(types.ON_EXCLUDE_PUBLISHER, {
  publisherKey
})

export const restorePublishers = () => action(types.ON_RESTORE_PUBLISHERS)

export const getContributionAmount = () => action(types.GET_CONTRIBUTION_AMOUNT)

export const onContributionAmount = (amount: number) => action(types.ON_CONTRIBUTION_AMOUNT, {
  amount
})

export const onRecurringTips = (list: Rewards.Publisher[]) => action(types.ON_RECURRING_TIPS, {
  list
})

export const removeRecurringTip = (publisherKey: string) => action(types.REMOVE_RECURRING_TIP, {
  publisherKey
})

export const onCurrentTips = (list: Rewards.Publisher[]) => action(types.ON_CURRENT_TIPS, {
  list
})

export const getTipTable = () => action(types.GET_TIP_TABLE)

export const getContributeList = () => action(types.GET_CONTRIBUTE_LIST)

export const getAdsData = () => action(types.GET_ADS_DATA)

export const onAdsData = (adsData: Rewards.AdsData) => action(types.ON_ADS_DATA, {
  adsData
})

export const getAdsHistory = () => action(types.GET_ADS_HISTORY)

export const onAdsHistory = (adsHistory: Rewards.AdsHistory[]) => action(types.ON_ADS_HISTORY, {
  adsHistory
})

export const toggleAdThumbUp = (adContent: Rewards.AdContent) => action(types.TOGGLE_AD_THUMB_UP, {
  adContent
})

export const onToggleAdThumbUp = (result: Rewards.ToggleLikeAction) => action(types.ON_TOGGLE_AD_THUMB_UP, {
  result
})

export const toggleAdThumbDown = (adContent: Rewards.AdContent) => action(types.TOGGLE_AD_THUMB_DOWN, {
  adContent
})

export const onToggleAdThumbDown = (result: Rewards.ToggleLikeAction) => action(types.ON_TOGGLE_AD_THUMB_DOWN, {
  result
})

export const toggleAdOptIn = (category: string, optAction: number) => action(types.TOGGLE_AD_OPT_IN, {
  category,
  optAction
})

export const onToggleAdOptIn = (result: Rewards.ToggleOptAction) => action(types.ON_TOGGLE_AD_OPT_IN, {
  result
})

export const toggleAdOptOut = (category: string, optAction: number) => action(types.TOGGLE_AD_OPT_OUT, {
  category,
  optAction
})

export const onToggleAdOptOut = (result: Rewards.ToggleOptAction) => action(types.ON_TOGGLE_AD_OPT_OUT, {
  result
})

export const toggleSavedAd = (adContent: Rewards.AdContent) => action(types.TOGGLE_SAVED_AD, {
  adContent
})

export const onToggleSavedAd = (result: Rewards.ToggleSavedAd) => action(types.ON_TOGGLE_SAVED_AD, {
  result
})

export const toggleFlaggedAd = (adContent: Rewards.AdContent) => action(types.TOGGLE_FLAGGED_AD, {
  adContent
})

export const onToggleFlaggedAd = (result: Rewards.ToggleFlaggedAd) => action(types.ON_TOGGLE_FLAGGED_AD, {
  result
})

export const onAdsSettingSave = (key: string, value: any) => action(types.ON_ADS_SETTING_SAVE, {
  key,
  value
})

export const getReconcileStamp = () => action(types.GET_RECONCILE_STAMP)

export const getPendingContributions = () => action(types.GET_PENDING_CONTRIBUTIONS)

export const onPendingContributions = (list: Rewards.PendingContribution[]) =>
  action(types.ON_PENDING_CONTRIBUTIONS, {
    list
  })

export const onStatement = (data: any) => action(types.ON_STATEMENT, { data })

export const getStatement = () => action(types.GET_STATEMENT)

export const onStatementChanged = () => action(types.ON_STATEMENT_CHANGED)

export const onRecurringTipSaved = (success: boolean) => action(types.ON_RECURRING_TIP_SAVED, {
  success
})

export const onRecurringTipRemoved = (success: boolean) => action(types.ON_RECURRING_TIP_REMOVED, {
  success
})

export const onInlineTipSettingChange = (key: string, value: boolean) => action(types.ON_INLINE_TIP_SETTINGS_CHANGE, {
  key,
  value
})

export const onInlineTipsEnabledChange = (enabled: boolean) =>
  action(types.ON_INLINE_TIPS_ENABLED_CHANGE, { enabled })

export const removePendingContribution = (id: number) =>
  action(types.REMOVE_PENDING_CONTRIBUTION, {
    id
  })

export const removeAllPendingContribution = () => action(types.REMOVE_ALL_PENDING_CONTRIBUTION)

export const restorePublisher = (publisherKey: string) => action(types.ON_RESTORE_PUBLISHER, {
  publisherKey
})

export const getExcludedSites = () => action(types.GET_EXCLUDED_SITES)

export const getBalance = () => action(types.GET_BALANCE)

export const onBalance = (result: mojom.FetchBalanceResult) =>
  action(types.ON_BALANCE, {
    result
  })

export const getExternalWalletProviders = () => action(types.GET_EXTERNAL_WALLET_PROVIDERS)

export const getExternalWallet = () => action(types.GET_EXTERNAL_WALLET)

export const onGetExternalWallet = (result: mojom.GetExternalWalletResult) => action(types.ON_GET_EXTERNAL_WALLET, {
  result
})

export const connectExternalWallet = (path: string, query: string) => action(types.CONNECT_EXTERNAL_WALLET, {
  path,
  query
})

export const onConnectExternalWallet = (result: mojom.ConnectExternalWalletResult) => action(types.ON_CONNECT_EXTERNAL_WALLET, {
  result
})

export const hideRedirectModal = () => action(types.HIDE_REDIRECT_MODAL)

export const getMonthlyReport = (month?: number, year?: number) => action(types.GET_MONTHLY_REPORT, {
  month,
  year
})

export const onMonthlyReport = (properties: { result: number, month: number, year: number, report: Rewards.MonthlyReport }) => action(types.ON_MONTHLY_REPORT, {
  result: properties.result,
  month: properties.month,
  year: properties.year,
  report: properties.report
})

export const onReconcileStampReset = () => action(types.ON_RECONCILE_STAMP_RESET)

export const getMonthlyReportIds = () => action(types.GET_MONTHLY_REPORT_IDS)

export const onMonthlyReportIds = (ids: string[]) => action(types.ON_MONTHLY_REPORT_IDS, ids)

export const dismissPromoPrompt = (promo: string) => action(types.DISMISS_PROMO_PROMPT, {
  promo
})

export const getCountryCode = () => action(types.GET_COUNTRY_CODE)

export const onCountryCode = (countryCode: string) => action(types.ON_COUNTRY_CODE, {
  countryCode
})

export const onInitialized = () => action(types.ON_INITIALIZED)

export const completeReset = () => action(types.COMPLETE_RESET)

export const onCompleteReset = (success: boolean) => action(types.ON_COMPLETE_RESET, {
  success
})

export const getOnboardingStatus = () => action(types.GET_ONBOARDING_STATUS)

export const onOnboardingStatus = (showOnboarding: boolean) => action(types.ON_ONBOARDING_STATUS, {
  showOnboarding
})

export const enableRewards = () => action(types.ENABLE_REWARDS)

export const getEnabledInlineTippingPlatforms = () => action(types.GET_ENABLED_INLINE_TIPPING_PLATFORMS)

export const onEnabledInlineTippingPlatforms = (platforms: string[]) => action(types.ON_ENABLED_INLINE_TIPPING_PLATFORMS, {
  platforms
})

export const restartBrowser = () => action(types.RESTART_BROWSER)

export const onPrefChanged = (key: string) => action(types.ON_PREF_CHANGED, {
  key
})

export const getIsUnsupportedRegion = () => action(types.GET_IS_UNSUPPORTED_REGION)

export const onIsUnsupportedRegion = (isUnsupportedRegion: boolean) => action(types.ON_IS_UNSUPPORTED_REGION, {
  isUnsupportedRegion
})
