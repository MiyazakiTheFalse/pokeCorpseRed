#include "global.h"
#include "gflib.h"
#include "event_data.h"
#include "menu.h"
#include "text_window.h"
#include "strings.h"
#include "constants/vars.h"

#define MAX_MONEY 999999

EWRAM_DATA static u8 sMoneyBoxWindowId = 0;

#define SOUL_DENSITY_MAX 100
#define SOUL_DENSITY_GAIN_DIVISOR 2000

static void TryIncreaseLocalSoulDensityFromSoulGain(u32 *moneyPtr, u32 oldValue, u32 newValue)
{
    u16 density;
    u16 gain;

    if (moneyPtr != &gSaveBlock1Ptr->money || newValue <= oldValue)
        return;

    density = VarGet(VAR_SOUL_DENSITY_LOCAL);
    gain = 1 + (newValue - oldValue) / SOUL_DENSITY_GAIN_DIVISOR;
    if (gain > 12)
        gain = 12;

    density = min((u16)SOUL_DENSITY_MAX, (u16)(density + gain));
    VarSet(VAR_SOUL_DENSITY_LOCAL, density);
}

u32 GetMoney(u32 *moneyPtr)
{
    return *moneyPtr ^ gSaveBlock2Ptr->encryptionKey;
}

void SetMoney(u32 *moneyPtr, u32 newValue)
{
    *moneyPtr = gSaveBlock2Ptr->encryptionKey ^ newValue;
}

bool8 IsEnoughMoney(u32 *moneyPtr, u32 cost)
{
    if (GetMoney(moneyPtr) >= cost)
        return TRUE;
    else
        return FALSE;
}

void AddMoney(u32 *moneyPtr, u32 toAdd)
{
    u32 oldValue = GetMoney(moneyPtr);
    u32 toSet = oldValue;

    // can't have more money than MAX
    if (toSet + toAdd > MAX_MONEY)
    {
        toSet = MAX_MONEY;
    }
    else
    {
        toSet += toAdd;
        // check overflow, can't have less money after you receive more
        if (toSet < oldValue)
            toSet = MAX_MONEY;
    }

    SetMoney(moneyPtr, toSet);
    TryIncreaseLocalSoulDensityFromSoulGain(moneyPtr, oldValue, toSet);
}

void RemoveMoney(u32 *moneyPtr, u32 toSub)
{
    u32 toSet = GetMoney(moneyPtr);

    // can't subtract more than you already have
    if (toSet < toSub)
        toSet = 0;
    else
        toSet -= toSub;

    SetMoney(moneyPtr, toSet);
}

bool8 IsEnoughForCostInVar0x8005(void)
{
    return IsEnoughMoney(&gSaveBlock1Ptr->money, gSpecialVar_0x8005);
}

void SubtractMoneyFromVar0x8005(void)
{
    RemoveMoney(&gSaveBlock1Ptr->money, gSpecialVar_0x8005);
}

void PrintMoneyAmountInMoneyBox(u8 windowId, int amount, u8 speed)
{
    u8 *txtPtr;
    s32 strLength;

    ConvertIntToDecimalStringN(gStringVar1, amount, STR_CONV_MODE_LEFT_ALIGN, 6);

    strLength = 6 - StringLength(gStringVar1);
    txtPtr = gStringVar4;

    while (strLength-- != 0)
        *(txtPtr++) = 0;

    StringExpandPlaceholders(txtPtr, gText_PokedollarVar1);
    AddTextPrinterParameterized(windowId, FONT_SMALL, gStringVar4, 64 - GetStringWidth(FONT_SMALL, gStringVar4, 0), 0xC, speed, NULL);
}

void PrintMoneyAmount(u8 windowId, u8 x, u8 y, int amount, u8 speed)
{
    u8 *txtPtr;
    s32 strLength;

    ConvertIntToDecimalStringN(gStringVar1, amount, STR_CONV_MODE_LEFT_ALIGN, 6);

    strLength = 6 - StringLength(gStringVar1);
    txtPtr = gStringVar4;

    while (strLength-- != 0)
        *(txtPtr++) = 0;

    StringExpandPlaceholders(txtPtr, gText_PokedollarVar1);
    AddTextPrinterParameterized(windowId, FONT_SMALL, gStringVar4, x, y, speed, NULL);
}

void PrintMoneyAmountInMoneyBoxWithBorder(u8 windowId, u16 tileStart, u8 paletteNum, int amount)
{
    DrawStdFrameWithCustomTileAndPalette(windowId, FALSE, tileStart, paletteNum);
    AddTextPrinterParameterized(windowId, FONT_NORMAL, gText_TrainerCardMoney, 0, 0, 0xFF, 0);
    PrintMoneyAmountInMoneyBox(windowId, amount, 0);
}

void ChangeAmountInMoneyBox(int amount)
{
    PrintMoneyAmountInMoneyBox(sMoneyBoxWindowId, amount, 0);
}

void DrawMoneyBox(int amount, u8 x, u8 y)
{
    struct WindowTemplate template;

    template = SetWindowTemplateFields(0, x + 1, y + 1, 8, 3, 15, 8);
    sMoneyBoxWindowId = AddWindow(&template);
    FillWindowPixelBuffer(sMoneyBoxWindowId, 0);
    PutWindowTilemap(sMoneyBoxWindowId);
    LoadStdWindowGfx(sMoneyBoxWindowId, 0x21D, BG_PLTT_ID(13));
    PrintMoneyAmountInMoneyBoxWithBorder(sMoneyBoxWindowId, 0x21D, 13, amount);
}

void HideMoneyBox(void)
{
    ClearStdWindowAndFrameToTransparent(sMoneyBoxWindowId, FALSE);
    CopyWindowToVram(sMoneyBoxWindowId, COPYWIN_GFX);
    RemoveWindow(sMoneyBoxWindowId);
}
