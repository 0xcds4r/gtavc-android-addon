#pragma once

#include "CCommon.h"

#define MORE_LANGUAGES
#define BUTTON_ICONS

struct CFontDetails
{
	uint32_t color;
	float scaleX;
	float scaleY;
	float slant;
	float slantRefX;
	float slantRefY;
	uint8_t justify;
	uint8_t centre;
	uint8_t rightJustify;
	uint8_t background;
	uint8_t backgroundOnlyText;
	uint8_t proportional;
	uint8_t bIsShadow;
	uint8_t bFlash;
	uint8_t bBold;
	float alphaFade;
	uint32_t backgroundColor;
	float wrapX;
	float centreSize;
	float rightJustifyWrap;
	uint16_t style;
	uint8_t bFontHalfTexture;
	uint32_t bank;
	uint16_t dropShadowPosition;
	uint32_t dropColor;
	uint8_t bFlashState;
	int nFlashTimer;
	uint8_t anonymous_23;
	uint32_t anonymous_25;
};

struct CFontRenderState
{
	uint32_t anonymous_0;
	float fTextPosX;
	float fTextPosY;
	float scaleX;
	float scaleY;
	uint32_t color;
	float fExtraSpace;
	float slant;
	float slantRefX;
	float slantRefY;
	uint8_t bIsShadow;
	uint8_t bFontHalfTexture;
	uint8_t proportional;
	uint8_t anonymous_14;
	uint16_t style;
};

class CSprite2d;

enum {
	FONT_BANK,
	FONT_STANDARD,
	FONT_HEADING,
#ifdef MORE_LANGUAGES
	FONT_JAPANESE,
#endif
	MAX_FONTS = FONT_HEADING
};

enum {
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT,
};

#ifdef MORE_LANGUAGES
enum
{
	FONT_LANGSET_EFIGS,
	FONT_LANGSET_RUSSIAN,
	FONT_LANGSET_POLISH,
	FONT_LANGSET_JAPANESE,
	LANGSET_MAX
};

#define FONT_LOCALE(style) (CFont::IsJapanese() ? FONT_JAPANESE : style)
#else
#define FONT_LOCALE(style) (style)
#endif

#ifdef BUTTON_ICONS
enum
{
	BUTTON_NONE = -1,
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_CROSS,
	BUTTON_CIRCLE,
	BUTTON_SQUARE,
	BUTTON_TRIANGLE,
	BUTTON_L1,
	BUTTON_L2,
	BUTTON_L3,
	BUTTON_R1,
	BUTTON_R2,
	BUTTON_R3,
	BUTTON_RSTICK_UP,
	BUTTON_RSTICK_DOWN,
	BUTTON_RSTICK_LEFT,
	BUTTON_RSTICK_RIGHT,
	MAX_BUTTON_ICONS
};
#endif // BUTTON_ICONS

class CRect {
public:
    float left;     // x min
	float bottom;   // y max
	float right;    // x max
	float top;      // y min
};

class CSprite2d;
class CFont
{
#ifdef MORE_LANGUAGES
	static uint16_t Size[LANGSET_MAX][MAX_FONTS][210];
	static uint8_t LanguageSet;
	static uint32_t Slot;
#else
	static uint16_t Size[MAX_FONTS][210];
#endif
	static uint16_t NewLine;
public:
	static CSprite2d Sprite[MAX_FONTS];
	static CFontDetails Details;
	static CFontRenderState RenderState;

#ifdef BUTTON_ICONS
	static uint32_t ButtonsSlot;
	static CSprite2d ButtonSprite[MAX_BUTTON_ICONS];
	static int PS2Symbol;
	
	static void LoadButtons(const char *txdPath);
	static void DrawButton(float x, float y);
#endif // BUTTON_ICONS


	static void Initialise(void) {

    };

	static void Shutdown(void) {
        
    };

	static void InitPerFrame(void) {

    };

	static void PrintChar(float x, float y, uint16_t c) {
        
    };

	static void PrintString(float x, float y, uint16_t *s) {

    };

#ifdef XBOX_SUBTITLES
	static void PrintStringFromBottom(float x, float y, uint16_t *str);
	static void PrintOutlinedString(float x, float y, uint16_t *str, float outlineStrength, bool fromBottom, CRGBA outlineColor);
#endif
	static int GetNumberLines(float xstart, float ystart, uint16_t *s) {

    };

	static void GetTextRect(CRect *rect, float xstart, float ystart, uint16_t *s) {

    };
//#ifdef MORE_LANGUAGES
//	static bool PrintString(float x, float y, wchar *start, wchar* &end, float spwidth, float japX);
//#else
	static void PrintString(float x, float y, uint32_t, uint16_t *start, uint16_t *end, float spwidth) {

    };
//#endif
	static void PrintStringFromBottom(float x, float y, uint16_t *str) {

    };

	static float GetCharacterWidth(uint16_t c) {

    };

	static float GetCharacterSize(uint16_t c) {

    };

	static float GetStringWidth(uint16_t *s, bool spaces = false) {

    };

#ifdef MORE_LANGUAGES
	static float GetStringWidth_Jap(uint16_t* s) {

    };
#endif
	static uint16_t *GetNextSpace(uint16_t *s) {

    };
//#ifdef MORE_LANGUAGES
//	static uint16 *ParseToken(wchar *s, bool japShit = false);
//#else
	static uint16_t *ParseToken(uint16_t *s) {

    };
    
	static uint16_t *ParseToken(uint16_t *s, uint32_t &color, bool &flash, bool &bold) {

    };

//#endif
	static void DrawFonts(void) {

    };

	static void RenderFontBuffer(void) {

    };

	static uint16_t character_code(uint8_t c) {

    };

	static void SetScale(float x, float y) {
        // CFont::SetScale(float,float)	.text	001D2C58	00000076	00000020	FFFFFFE0	R	.	.	.	.	T	.
        ((int(*)(float, float))(VC(0x1D2C58 + 1)))(x, y);
    };

	static void SetSlantRefPoint(float x, float y) {

    };

	static void SetSlant(float s) {

    };

	static void SetJustifyOn(void) {

    };

	static void SetJustifyOff(void) {
        // CFont::SetJustifyOff(void)	.text	001D2D88	0000000E			R	.	.	.	.	T	.
        ((int(*)())(VC(0x1D2D88 + 1)))();
    };

	static void SetRightJustifyOn(void) {

    };

	static void SetRightJustifyOff(void) {

    };

	static void SetCentreOn(void) {

    };

	static void SetCentreOff(void) {
        // CFont::SetCentreOff(void)	.text	001D2DB4	0000000C			R	.	.	.	.	T	.
        ((int(*)())(VC(0x1D2DB4 + 1)))();
    };

	static void SetWrapx(float x) {
        // CFont::SetWrapx(float)	.text	001D2DC4	0000000A			R	.	.	.	.	T	.
        ((int(*)(float))(VC(0x1D2DC4 + 1)))(x);
    };

	static void SetCentreSize(float s) {
        // CFont::SetCentreSize(float)	.text	001D2DD4	0000000A			R	.	.	.	.	T	.
        ((int(*)(float))(VC(0x1D2DD4 + 1)))(s);
    };

	static void SetBackgroundOn(void) {

    };

	static void SetBackgroundOff(void) {
        // CFont::SetBackgroundOff(void)	.text	001D2DF4	0000000C			R	.	.	.	.	T	.
        ((int(*)())(VC(0x1D2DF4 + 1)))();
    };

	static void SetBackGroundOnlyTextOn(void) {

    };

	static void SetBackGroundOnlyTextOff(void) {
        // CFont::SetBackGroundOnlyTextOff(void)	.text	001D2E3C	0000000C			R	.	.	.	.	T	.
        ((int(*)())(VC(0x1D2E3C + 1)))();
    };

	static void SetPropOn(void) {
        // CFont::SetPropOn(void)	.text	001D2E88	0000000C			R	.	.	.	.	T	.
        ((int(*)())(VC(0x1D2E88 + 1)))();
    };

	static void SetPropOff(void) {

    };

	static void SetFontStyle(uint16_t style) {
        // CFont::SetFontStyle(short)	.text	001D2E98	00000024			R	.	.	.	.	T	.
        ((int(*)(uint16_t))(VC(0x1D2E98 + 1)))(style);
    };

	static void SetRightJustifyWrap(float wrap) {

    };

	static void SetAlphaFade(float fade) {

    };

	static void SetDropShadowPosition(uint16_t pos) {
        // CFont::SetDropShadowPosition(short)	.text	001D2F40	0000000C			R	.	.	.	.	T	.
        ((int(*)(uint16_t))(VC(0x1D2F40 + 1)))(pos);
    };

	static void SetBackgroundColor(uint32_t col) {

    };

	static void SetColor(uint32_t col) {

    };

	static void SetDropColor(uint32_t col) {

    };

	static uint16_t FindNewCharacter(uint16_t c) {

    };

	static void FilterOutTokensFromString(uint16_t*) {

    };

#ifdef MORE_LANGUAGES
	static void ReloadFonts(uint8_t set) {

    };

	// japanese stuff
	static bool IsAnsiCharacter(uint16_t* s) {

    };

	static bool IsJapanesePunctuation(uint16_t* str) {

    };

	static bool IsJapanese() { return LanguageSet == FONT_LANGSET_JAPANESE; }
	static bool IsJapaneseFont() { return IsJapanese() && (Details.style == FONT_JAPANESE);  }
#endif
};
