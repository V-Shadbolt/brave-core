// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import styled from 'styled-components'
import * as leo from '@brave/leo/tokens/css'
import {
  AssetIconProps,
  AssetIconFactory,
  WalletButton,
  Row,
  Column
} from '../../../shared/style'
import {
  layoutSmallWidth,
  layoutPanelWidth
} from '../../wallet-page-wrapper/wallet-page-wrapper.style'

export const StyledWrapper = styled.div`
  display: flex;
  flex-direction: column;
  align-items: flex-start;
  justify-content: flex-start;
  width: 100%;
  height: 100%;
`

export const TopRow = styled.div`
  display: flex;
  flex-direction: row;
  align-items: center;
  justify-content: space-between;
  width: 100%;
`

export const BalanceText = styled.span`
  font-family: Poppins;
  font-style: normal;
  font-size: 36px;
  font-weight: 500;
  line-height: 54px;
  color: ${leo.color.text.primary};
`

export const FiatChange = styled.span<{ isDown?: boolean }>`
  font-family: Poppins;
  font-style: normal;
  font-size: 12px;
  font-weight: 400;
  line-height: 18px;
  margin-right: 8px;
  color: ${(p) =>
    p.isDown
      ? leo.color.systemfeedback.errorIcon
      : leo.color.systemfeedback.successIcon
  };
`

export const ButtonRow = styled.div<
  {
    noMargin?: boolean,
    horizontalPadding?: number
  }>`
  display: flex;
  flex-direction: row;
  align-items: center;
  justify-content: flex-start;
  width: 100%;
  margin: ${(p) => p.noMargin ? '0px' : '20px 0px'};
  padding: 0px ${(p) =>
    p.horizontalPadding !== undefined
      ? p.horizontalPadding
      : 0
  }px;
`

export const BalanceRow = styled.div<{ gap?: string }>`
  display: flex;
  flex-direction: row;
  align-items: center;
  justify-content: center;
  height: 36px;
  vertical-align: middle;
  gap: ${p => p.gap || 0};
`

// Construct styled-component using JS object instead of string, for editor
// support with custom AssetIconFactory.
//
// Ref: https://styled-components.com/docs/advanced#style-objects
export const AssetIcon = AssetIconFactory<AssetIconProps>({
  width: '40px',
  height: 'auto'
})

export const SubDivider = styled.div`
  width: 100%;
  height: 2px;
  background-color: ${(p) => p.theme.color.divider01};
  margin-bottom: 12px;
`

export const DividerText = styled.span`
  font-family: Poppins;
  font-size: 15px;
  line-height: 20px;
  letter-spacing: 0.04em;
  font-weight: 600;
  margin-bottom: 10px;
  color: ${(p) => p.theme.color.text03};
`

export const PercentBubble = styled.div<{ isDown?: boolean }>`
  display: flex;
  padding: 4px 8px;
  border-radius: 4px;
  background-color: ${(p) =>
    p.isDown
      ? leo.color.red[10]
      : leo.color.green[10]
  };
  font-family: Poppins;
  font-size: 11px;
  line-height: 16px;
  letter-spacing: 0.02em;
  font-weight: 500;
  color: ${(p) =>
    p.isDown
      ? leo.color.red[50]
      : leo.color.green[50]
  };
`

export const EmptyTransactionContainer = styled.div`
  display: flex;
  flex-direction: row;
  align-items: flex-start;
  justify-content: flex-start;
  width: 100%;
  height: 100px;
`

export const TransactionPlaceholderText = styled.span`
  font-family: Poppins;
  font-size: 13px;
  line-height: 20px;
  letter-spacing: 0.01em;
  font-weight: 600;
  color: ${(p) => p.theme.color.text03};
  margin-left: 10px;
`

export const AssetBalanceDisplay = styled.span`
  font-family: Poppins;
  font-size: 14px;
  line-height: 20px;
  letter-spacing: 0.01em;
  font-weight: 600;
  color: ${(p) => p.theme.color.text02};
`

export const DividerRow = styled.div`
  display: flex;
  align-items: center;
  justify-content: space-between;
  flex-direction: row;
  width: 100%;
`

export const Spacer = styled.div`
  display: flex;
  height: 2px;
  width: 100%;
  margin-top: 10px;
`

export const CoinGeckoText = styled.span`
  font-family: Arial;
  font-size: 10px;
  font-weight: normal;
  color: ${(p) => p.theme.color.text03};
  margin: 15px 0px;
`

export const FilterTokenRow = styled.div<
  {
    horizontalPadding?: number
  }>`
  display: flex;
  flex-direction: row;
  flex-wrap: wrap;
  width: 100%;
  gap: 14px;
  padding: 0px ${(p) =>
    p.horizontalPadding !== undefined
      ? p.horizontalPadding
      : 0
  }px;
`

export const NftMultimedia = styled.iframe<{ visible?: boolean }>`
  display: ${p => p.visible ? 'flex' : 'none'};
  width: 100%;
  min-height: ${p => p.visible ? '500px' : '0px'};
  border: none;
  visibility: ${p => p.visible ? 'visible' : 'hidden'};
  margin-bottom: 30px;
  margin-top: 16px;
`

export const BridgeToAuroraButton = styled(WalletButton) <
  {
    noBottomMargin?: boolean
  }>`
  display: flex;
  flex-direction: row;
  justify-content: center;
  align-items: center;
  padding: 8px 14px;
  height: 40px;
  cursor: pointer;
  outline: none;
  border-radius: 40px;
  font-family: 'Poppins';
  font-style: normal;
  font-weight: 600;
  font-size: 14px;
  line-height: 21px;
  background-color: ${(p) => p.theme.palette.blurple500};
  color: ${(p) => p.theme.palette.white};
  border: none;
  margin-bottom: ${(p) => p.noBottomMargin ? 0 : 32}px;
  margin-right: 10px;
`

export const SelectTimelineWrapper = styled(Row)`
  @media screen and (max-width: ${layoutSmallWidth}px) {
    justify-content: flex-start;
  }
  @media screen and (max-width: ${layoutPanelWidth}px) {
    justify-content: center;
  }
`

export const ControlsRow = styled(Row)`
  box-shadow: 0px -1px 1px rgba(0, 0, 0, 0.02);
  border-radius: 16px;
`

export const BalanceAndButtonsWrapper = styled(Column)`
  @media screen and (max-width: ${layoutSmallWidth}px) {
    flex-direction: row;
    justify-content: space-between;
    align-items: flex-start;
  }
  @media screen and (max-width: ${layoutPanelWidth}px) {
    flex-direction: column;
    justify-content: flex-start;
    align-items: center;
    padding: 24px 0px;
  }
`

export const BalanceAndChangeWrapper = styled(Column)`
  @media screen and (max-width: ${layoutSmallWidth}px) {
    align-items: flex-start;
  }
  @media screen and (max-width: ${layoutPanelWidth}px) {
    flex-direction: column;
    align-items: center;
    justify-content: flex-start;
    margin-bottom: 24px;
  }
`
