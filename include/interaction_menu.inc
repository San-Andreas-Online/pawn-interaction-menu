//  ____  ___ _____ _   _ _   _ 
// (_)  \/  ||  ___| \ | | | | |
//  _| .  . || |__ |  \| | | | |
// | | |\/| ||  __|| . ` | | | |
// | | |  | || |___| |\  | |_| |
// |_\_|  |_/\____/\_| \_/\___/ 
//
// Version 1.1.0 by
//    _____ ___   ____     _______________    __  ___
//   / ___//   | / __ \   /_  __/ ____/   |  /  |/  /
//   \__ \/ /| |/ / / /    / / / __/ / /| | / /|_/ / 
//  ___/ / ___ / /_/ /    / / / /___/ ___ |/ /  / /  
// /____/_/  |_\____/    /_/ /_____/_/  |_/_/  /_/     

#if defined _INC_sao_interaction_menu
	#endinput
#endif
#define _INC_sao_interaction_menu

#define MAX_ITEM_LEN 256
#define MAX_DESCRIPTION_LEN 70
#define MAX_SCROLL_LEN 10
#define MAX_MENU_ITEMS 35
#define MAX_SCROLL_ITEMS 15
#define DEFAULT_BOX_ALPHA 0xAA

// TextDraw Menu Colors
#define COLOR_MENU_TITLE_BOX 0xB03060DC
#define COLOR_MENU_TITLE_TEXT 0xFFFFFFFF

#define COLOR_MENU_SEPARATOR_BOX 0x000000FF
#define COLOR_MENU_HEADER_TEXT 0xB03060DC
#define COLOR_MENU_BACKGROUND_BOX 0x000000CF
#define COLOR_MENU_ITEM_DESCRIPTION 0xFFFFFFFF
#define COLOR_MENU_FOOTER_ARROW 0xFFFFFFFF

#define COLOR_MENU_ITEM_BOX_HIGHLIGHT 0xFFFFFFFF
#define COLOR_MENU_ITEM_TEXT_HIGHLIGHT 0x000000FF
#define COLOR_MENU_ITEM_BOX 0xFFFFFF00
#define COLOR_MENU_ITEM_TEXT 0xFFFFFFFF
#define COLOR_MENU_LOCKED_ITEM 0x6B6B6BA2

static enum e_MENU_TD
{
	PlayerText:MENU_TITLE,
	PlayerText:MENU_TITLE_BOX,
	PlayerText:MENU_TITLE_BOX_SEPARATOR,
	PlayerText:MENU_HEADER_TEXT,
	PlayerText:MENU_HEADER_ITEM_COUNT,
	PlayerText:MENU_BACKGROUND_BOX,
	PlayerText:MENU_ITEM_FOOTER_ARROW,
	PlayerText:MENU_FOOTER_BOX,
	PlayerText:MENU_FOOTER_TEXT,
};

static enum e_MENU_ITEM
{
	PlayerText:MENU_ITEM_1,
	PlayerText:MENU_ITEM_2,
	PlayerText:MENU_ITEM_3,
	PlayerText:MENU_ITEM_4,
	PlayerText:MENU_ITEM_5,
	PlayerText:MENU_ITEM_6,
	PlayerText:MENU_ITEM_7
}

enum e_CONTROL_BUTTON
{
	PlayerText:CONTROL_BUTTON_UP,
	PlayerText:CONTROL_BUTTON_DOWN,
	PlayerText:CONTROL_BUTTON_LEFT,
	PlayerText:CONTROL_BUTTON_RIGHT,
	PlayerText:CONTROL_BUTTON_SELECT,
	PlayerText:CONTROL_BUTTON_CLOSE
};


	

static
	PlayerText:tdMenuItemBox[MAX_PLAYERS][e_MENU_ITEM],
	PlayerText:tdMenuItemText[MAX_PLAYERS][e_MENU_ITEM],
	PlayerText:tdMenuItemScroll[MAX_PLAYERS][e_MENU_ITEM],
	PlayerText:tdInteractionMenu[MAX_PLAYERS][e_MENU_TD],
	PlayerText:tdAndroidButtons[MAX_PLAYERS][e_CONTROL_BUTTON],
	PlayerText:tdAndroidText[MAX_PLAYERS][e_CONTROL_BUTTON]
;
/**
 * Store menu state differently depending if foreach is included or not.
 * If foreach is included: use Iterators (itIsInMenu, itHasLastMenu)
 * If it is not included: use bool arrays (bIsInMenu, bHasLastMenu) for each player, which works but uses more memory.
 */
#if defined _FOREACH_LOCAL_VERSION

	static Iterator:itIsInMenu<MAX_PLAYERS>;
	static Iterator:itHasLastMenu<MAX_PLAYERS>;

#else 

	static bool:bIsInMenu[MAX_PLAYERS];
	static bool:bHasLastMenu[MAX_PLAYERS];
	
#endif

static
	iMenuId[MAX_PLAYERS],
	iSelectedMenuItemId[MAX_PLAYERS],
	iMenuScrollOffset[MAX_PLAYERS]
;

static
	interactionMenuItems[MAX_PLAYERS][MAX_MENU_ITEMS][MAX_ITEM_LEN],
	interactionMenuScrollItems[MAX_PLAYERS][MAX_MENU_ITEMS][MAX_SCROLL_ITEMS][MAX_SCROLL_LEN],
	interactionMenuScrollIndex[MAX_PLAYERS][MAX_MENU_ITEMS],
	interactionMenuScrollCount[MAX_PLAYERS][MAX_MENU_ITEMS],
	interactionMenuDesc[MAX_PLAYERS][MAX_MENU_ITEMS][MAX_DESCRIPTION_LEN],
	interactionMenuItemCount[MAX_PLAYERS],
   	bool:bIsItemLocked[MAX_PLAYERS][MAX_MENU_ITEMS],
	bool:bIsItemScrollable[MAX_PLAYERS][MAX_MENU_ITEMS],
	bool:bIsItemEx[MAX_PLAYERS][MAX_MENU_ITEMS]
;

static
	boxExColor[MAX_PLAYERS][MAX_MENU_ITEMS],
	boxExHighlight[MAX_PLAYERS][MAX_MENU_ITEMS]
;

static unfreezeTimer[MAX_PLAYERS] = INVALID_TIMER;

static lastActionTick[MAX_PLAYERS],
	actionCount[MAX_PLAYERS]
;

/**
 * <summary>Creates the interaction menu TextDraws for a player</summary>
 * <remarks>This function must be called once per player, usually on player connect.</remarks>
 * <param name="playerid">The player to create the menu for.</param>
 */
static CreateIMenu(playerid)
{

	// == ANDROID BUTTONS ==

	tdAndroidButtons[playerid][CONTROL_BUTTON_UP] = CreatePlayerTextDraw(playerid, 142.000, 188.000, "_");
	PlayerTextDrawLetterSize(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_UP], 0.349, 2.198);
	PlayerTextDrawTextSize(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_UP], 180.000, 16.000);
	PlayerTextDrawAlignment(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_UP], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_UP], -1);
	PlayerTextDrawUseBox(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_UP], true);
	PlayerTextDrawBoxColour(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_UP], 150);
	PlayerTextDrawSetShadow(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_UP], 1);
	PlayerTextDrawSetOutline(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_UP], 1);
	PlayerTextDrawBackgroundColour(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_UP], 150);
	PlayerTextDrawFont(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_UP], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_UP], true);

	tdAndroidText[playerid][CONTROL_BUTTON_UP] = CreatePlayerTextDraw(playerid, 148.000, 189.000, "_up_");
	PlayerTextDrawLetterSize(playerid, tdAndroidText[playerid][CONTROL_BUTTON_UP], 0.270, 1.699);
	PlayerTextDrawTextSize(playerid, tdAndroidText[playerid][CONTROL_BUTTON_UP], 180.000, 17.000);
	PlayerTextDrawAlignment(playerid, tdAndroidText[playerid][CONTROL_BUTTON_UP], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdAndroidText[playerid][CONTROL_BUTTON_UP], -1);
	PlayerTextDrawSetShadow(playerid, tdAndroidText[playerid][CONTROL_BUTTON_UP], 0);
	PlayerTextDrawSetOutline(playerid, tdAndroidText[playerid][CONTROL_BUTTON_UP], 0);
	PlayerTextDrawBackgroundColour(playerid, tdAndroidText[playerid][CONTROL_BUTTON_UP], 150);
	PlayerTextDrawFont(playerid, tdAndroidText[playerid][CONTROL_BUTTON_UP], TEXT_DRAW_FONT_2);
	PlayerTextDrawSetProportional(playerid, tdAndroidText[playerid][CONTROL_BUTTON_UP], true);
	PlayerTextDrawSetSelectable(playerid, tdAndroidText[playerid][CONTROL_BUTTON_UP], true);

	tdAndroidButtons[playerid][CONTROL_BUTTON_DOWN] = CreatePlayerTextDraw(playerid, 142.000, 242.000, "_");
	PlayerTextDrawLetterSize(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_DOWN], 0.349, 2.198);
	PlayerTextDrawTextSize(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_DOWN], 180.000, 16.000);
	PlayerTextDrawAlignment(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_DOWN], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_DOWN], -1);
	PlayerTextDrawUseBox(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_DOWN], true);
	PlayerTextDrawBoxColour(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_DOWN], 150);
	PlayerTextDrawSetShadow(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_DOWN], 1);
	PlayerTextDrawSetOutline(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_DOWN], 1);
	PlayerTextDrawBackgroundColour(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_DOWN], 150);
	PlayerTextDrawFont(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_DOWN], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_DOWN], true);

	tdAndroidText[playerid][CONTROL_BUTTON_DOWN] = CreatePlayerTextDraw(playerid, 145.000, 243.000, "_down");
	PlayerTextDrawLetterSize(playerid, tdAndroidText[playerid][CONTROL_BUTTON_DOWN], 0.270, 1.699);
	PlayerTextDrawTextSize(playerid, tdAndroidText[playerid][CONTROL_BUTTON_DOWN], 180.000, 17.000);
	PlayerTextDrawAlignment(playerid, tdAndroidText[playerid][CONTROL_BUTTON_DOWN], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdAndroidText[playerid][CONTROL_BUTTON_DOWN], -1);
	PlayerTextDrawSetShadow(playerid, tdAndroidText[playerid][CONTROL_BUTTON_DOWN], 0);
	PlayerTextDrawSetOutline(playerid, tdAndroidText[playerid][CONTROL_BUTTON_DOWN], 0);
	PlayerTextDrawBackgroundColour(playerid, tdAndroidText[playerid][CONTROL_BUTTON_DOWN], 150);
	PlayerTextDrawFont(playerid, tdAndroidText[playerid][CONTROL_BUTTON_DOWN], TEXT_DRAW_FONT_2);
	PlayerTextDrawSetProportional(playerid, tdAndroidText[playerid][CONTROL_BUTTON_DOWN], true);
	PlayerTextDrawSetSelectable(playerid, tdAndroidText[playerid][CONTROL_BUTTON_DOWN], true);

	tdAndroidButtons[playerid][CONTROL_BUTTON_LEFT] = CreatePlayerTextDraw(playerid, 210.000, 242.000, "_");
	PlayerTextDrawLetterSize(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_LEFT], 0.418, 2.198);
	PlayerTextDrawTextSize(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_LEFT], 182.000, 16.000);
	PlayerTextDrawAlignment(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_LEFT], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_LEFT], -1);
	PlayerTextDrawUseBox(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_LEFT], true);
	PlayerTextDrawBoxColour(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_LEFT], 150);
	PlayerTextDrawSetShadow(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_LEFT], 1);
	PlayerTextDrawSetOutline(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_LEFT], 1);
	PlayerTextDrawBackgroundColour(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_LEFT], 150);
	PlayerTextDrawFont(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_LEFT], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_LEFT], true);

	tdAndroidText[playerid][CONTROL_BUTTON_LEFT] = CreatePlayerTextDraw(playerid, 191.000, 241.000, "<");
	PlayerTextDrawLetterSize(playerid, tdAndroidText[playerid][CONTROL_BUTTON_LEFT], 0.338, 2.299);
	PlayerTextDrawTextSize(playerid, tdAndroidText[playerid][CONTROL_BUTTON_LEFT], 205.000, 17.000);
	PlayerTextDrawAlignment(playerid, tdAndroidText[playerid][CONTROL_BUTTON_LEFT], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdAndroidText[playerid][CONTROL_BUTTON_LEFT], -1);
	PlayerTextDrawSetShadow(playerid, tdAndroidText[playerid][CONTROL_BUTTON_LEFT], 0);
	PlayerTextDrawSetOutline(playerid, tdAndroidText[playerid][CONTROL_BUTTON_LEFT], 0);
	PlayerTextDrawBackgroundColour(playerid, tdAndroidText[playerid][CONTROL_BUTTON_LEFT], 150);
	PlayerTextDrawFont(playerid, tdAndroidText[playerid][CONTROL_BUTTON_LEFT], TEXT_DRAW_FONT_2);
	PlayerTextDrawSetProportional(playerid, tdAndroidText[playerid][CONTROL_BUTTON_LEFT], true);
	PlayerTextDrawSetSelectable(playerid, tdAndroidText[playerid][CONTROL_BUTTON_LEFT], true);

	tdAndroidButtons[playerid][CONTROL_BUTTON_RIGHT] = CreatePlayerTextDraw(playerid, 210.000, 188.000, "_");
	PlayerTextDrawLetterSize(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_RIGHT], 0.418, 2.198);
	PlayerTextDrawTextSize(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_RIGHT], 182.000, 16.000);
	PlayerTextDrawAlignment(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_RIGHT], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_RIGHT], -1);
	PlayerTextDrawUseBox(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_RIGHT], true);
	PlayerTextDrawBoxColour(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_RIGHT], 150);
	PlayerTextDrawSetShadow(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_RIGHT], 1);
	PlayerTextDrawSetOutline(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_RIGHT], 1);
	PlayerTextDrawBackgroundColour(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_RIGHT], 150);
	PlayerTextDrawFont(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_RIGHT], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_RIGHT], true);

	tdAndroidText[playerid][CONTROL_BUTTON_RIGHT] = CreatePlayerTextDraw(playerid, 191.000, 187.000, ">");
	PlayerTextDrawLetterSize(playerid, tdAndroidText[playerid][CONTROL_BUTTON_RIGHT], 0.338, 2.299);
	PlayerTextDrawTextSize(playerid, tdAndroidText[playerid][CONTROL_BUTTON_RIGHT], 205.000, 15.000);
	PlayerTextDrawAlignment(playerid, tdAndroidText[playerid][CONTROL_BUTTON_RIGHT], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdAndroidText[playerid][CONTROL_BUTTON_RIGHT], -1);
	PlayerTextDrawSetShadow(playerid, tdAndroidText[playerid][CONTROL_BUTTON_RIGHT], 0);
	PlayerTextDrawSetOutline(playerid, tdAndroidText[playerid][CONTROL_BUTTON_RIGHT], 0);
	PlayerTextDrawBackgroundColour(playerid, tdAndroidText[playerid][CONTROL_BUTTON_RIGHT], 150);
	PlayerTextDrawFont(playerid, tdAndroidText[playerid][CONTROL_BUTTON_RIGHT], TEXT_DRAW_FONT_2);
	PlayerTextDrawSetProportional(playerid, tdAndroidText[playerid][CONTROL_BUTTON_RIGHT], true);
	PlayerTextDrawSetSelectable(playerid, tdAndroidText[playerid][CONTROL_BUTTON_RIGHT], true);

	tdAndroidButtons[playerid][CONTROL_BUTTON_SELECT] = CreatePlayerTextDraw(playerid, 142.000, 215.000, "_");
	PlayerTextDrawLetterSize(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_SELECT], 0.349, 2.198);
	PlayerTextDrawTextSize(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_SELECT], 180.000, 16.000);
	PlayerTextDrawAlignment(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_SELECT], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_SELECT], -1);
	PlayerTextDrawUseBox(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_SELECT], true);
	PlayerTextDrawBoxColour(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_SELECT], 150);
	PlayerTextDrawSetShadow(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_SELECT], 1);
	PlayerTextDrawSetOutline(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_SELECT], 1);
	PlayerTextDrawBackgroundColour(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_SELECT], 150);
	PlayerTextDrawFont(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_SELECT], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_SELECT], true);
	PlayerTextDrawSetSelectable(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_SELECT], false);

	tdAndroidText[playerid][CONTROL_BUTTON_SELECT] = CreatePlayerTextDraw(playerid, 142.000, 216.000, "__select_"); select select ona
	PlayerTextDrawLetterSize(playerid, tdAndroidText[playerid][CONTROL_BUTTON_SELECT], 0.168, 1.699);
	PlayerTextDrawTextSize(playerid, tdAndroidText[playerid][CONTROL_BUTTON_SELECT], 170.000, 15.000);
	PlayerTextDrawAlignment(playerid, tdAndroidText[playerid][CONTROL_BUTTON_SELECT], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdAndroidText[playerid][CONTROL_BUTTON_SELECT], -1);
	PlayerTextDrawSetShadow(playerid, tdAndroidText[playerid][CONTROL_BUTTON_SELECT], 0);
	PlayerTextDrawSetOutline(playerid, tdAndroidText[playerid][CONTROL_BUTTON_SELECT], 0);
	PlayerTextDrawBackgroundColour(playerid, tdAndroidText[playerid][CONTROL_BUTTON_SELECT], 150);
	PlayerTextDrawFont(playerid, tdAndroidText[playerid][CONTROL_BUTTON_SELECT], TEXT_DRAW_FONT_2);
	PlayerTextDrawSetProportional(playerid, tdAndroidText[playerid][CONTROL_BUTTON_SELECT], true);
	PlayerTextDrawSetSelectable(playerid, tdAndroidText[playerid][CONTROL_BUTTON_SELECT], true);

	tdAndroidButtons[playerid][CONTROL_BUTTON_CLOSE] = CreatePlayerTextDraw(playerid, 210.000, 215.000, "_");
	PlayerTextDrawLetterSize(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_CLOSE], 0.418, 2.198);
	PlayerTextDrawTextSize(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_CLOSE], 182.000, 16.000);
	PlayerTextDrawAlignment(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_CLOSE], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_CLOSE], -1);
	PlayerTextDrawUseBox(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_CLOSE], true);
	PlayerTextDrawBoxColour(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_CLOSE], 150);
	PlayerTextDrawSetShadow(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_CLOSE], 1);
	PlayerTextDrawSetOutline(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_CLOSE], 1);
	PlayerTextDrawBackgroundColour(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_CLOSE], 150);
	PlayerTextDrawFont(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_CLOSE], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_CLOSE], true);
	PlayerTextDrawSetSelectable(playerid, tdAndroidButtons[playerid][CONTROL_BUTTON_CLOSE], false);

	tdAndroidText[playerid][CONTROL_BUTTON_CLOSE] = CreatePlayerTextDraw(playerid, 191.000, 214.000, "X");
	PlayerTextDrawLetterSize(playerid, tdAndroidText[playerid][CONTROL_BUTTON_CLOSE], 0.338, 2.299);
	PlayerTextDrawTextSize(playerid, tdAndroidText[playerid][CONTROL_BUTTON_CLOSE], 205.000, 15.000);
	PlayerTextDrawAlignment(playerid, tdAndroidText[playerid][CONTROL_BUTTON_CLOSE], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdAndroidText[playerid][CONTROL_BUTTON_CLOSE], -1);
	PlayerTextDrawSetShadow(playerid, tdAndroidText[playerid][CONTROL_BUTTON_CLOSE], 0);
	PlayerTextDrawSetOutline(playerid, tdAndroidText[playerid][CONTROL_BUTTON_CLOSE], 0);
	PlayerTextDrawBackgroundColour(playerid, tdAndroidText[playerid][CONTROL_BUTTON_CLOSE], 150);
	PlayerTextDrawFont(playerid, tdAndroidText[playerid][CONTROL_BUTTON_CLOSE], TEXT_DRAW_FONT_2);
	PlayerTextDrawSetProportional(playerid, tdAndroidText[playerid][CONTROL_BUTTON_CLOSE], true);
	PlayerTextDrawSetSelectable(playerid, tdAndroidText[playerid][CONTROL_BUTTON_CLOSE], true);

	// == MENU CONTAINER ==
	tdInteractionMenu[playerid][MENU_TITLE_BOX] = CreatePlayerTextDraw(playerid, 20.000, 146.000, "_");
	PlayerTextDrawLetterSize(playerid, tdInteractionMenu[playerid][MENU_TITLE_BOX], 0.310, 2.799);
	PlayerTextDrawTextSize(playerid, tdInteractionMenu[playerid][MENU_TITLE_BOX], 130.000, -12.000);
	PlayerTextDrawAlignment(playerid, tdInteractionMenu[playerid][MENU_TITLE_BOX], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdInteractionMenu[playerid][MENU_TITLE_BOX], -1);
	PlayerTextDrawUseBox(playerid, tdInteractionMenu[playerid][MENU_TITLE_BOX], true);
	PlayerTextDrawBoxColour(playerid, tdInteractionMenu[playerid][MENU_TITLE_BOX], COLOR_MENU_TITLE_BOX);
	PlayerTextDrawSetShadow(playerid, tdInteractionMenu[playerid][MENU_TITLE_BOX], 1);
	PlayerTextDrawSetOutline(playerid, tdInteractionMenu[playerid][MENU_TITLE_BOX], 1);
	PlayerTextDrawBackgroundColour(playerid, tdInteractionMenu[playerid][MENU_TITLE_BOX], 150);
	PlayerTextDrawFont(playerid, tdInteractionMenu[playerid][MENU_TITLE_BOX], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdInteractionMenu[playerid][MENU_TITLE_BOX], true);

	tdInteractionMenu[playerid][MENU_TITLE] = CreatePlayerTextDraw(playerid, 21.000, 152.000, "MENU_TITLE");
	PlayerTextDrawLetterSize(playerid, tdInteractionMenu[playerid][MENU_TITLE], 0.190, 1.499);
	PlayerTextDrawTextSize(playerid, tdInteractionMenu[playerid][MENU_TITLE], 4.000, 1.000);
	PlayerTextDrawAlignment(playerid, tdInteractionMenu[playerid][MENU_TITLE], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdInteractionMenu[playerid][MENU_TITLE], COLOR_MENU_TITLE_TEXT);
	PlayerTextDrawSetShadow(playerid, tdInteractionMenu[playerid][MENU_TITLE], 0);
	PlayerTextDrawSetOutline(playerid, tdInteractionMenu[playerid][MENU_TITLE], 0);
	PlayerTextDrawBackgroundColour(playerid, tdInteractionMenu[playerid][MENU_TITLE], 150);
	PlayerTextDrawFont(playerid, tdInteractionMenu[playerid][MENU_TITLE], TEXT_DRAW_FONT_2);
	PlayerTextDrawSetProportional(playerid, tdInteractionMenu[playerid][MENU_TITLE], true);

	tdInteractionMenu[playerid][MENU_TITLE_BOX_SEPARATOR] = CreatePlayerTextDraw(playerid, 20.000, 175.000, "_");
	PlayerTextDrawLetterSize(playerid, tdInteractionMenu[playerid][MENU_TITLE_BOX_SEPARATOR], 0.479, 0.899);
	PlayerTextDrawTextSize(playerid, tdInteractionMenu[playerid][MENU_TITLE_BOX_SEPARATOR], 130.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdInteractionMenu[playerid][MENU_TITLE_BOX_SEPARATOR], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdInteractionMenu[playerid][MENU_TITLE_BOX_SEPARATOR], 255);
	PlayerTextDrawUseBox(playerid, tdInteractionMenu[playerid][MENU_TITLE_BOX_SEPARATOR], true);
	PlayerTextDrawBoxColour(playerid, tdInteractionMenu[playerid][MENU_TITLE_BOX_SEPARATOR], COLOR_MENU_SEPARATOR_BOX);
	PlayerTextDrawSetShadow(playerid, tdInteractionMenu[playerid][MENU_TITLE_BOX_SEPARATOR], 1);
	PlayerTextDrawSetOutline(playerid, tdInteractionMenu[playerid][MENU_TITLE_BOX_SEPARATOR], 1);
	PlayerTextDrawBackgroundColour(playerid, tdInteractionMenu[playerid][MENU_TITLE_BOX_SEPARATOR], 150);
	PlayerTextDrawFont(playerid, tdInteractionMenu[playerid][MENU_TITLE_BOX_SEPARATOR], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdInteractionMenu[playerid][MENU_TITLE_BOX_SEPARATOR], true);

	tdInteractionMenu[playerid][MENU_HEADER_TEXT] = CreatePlayerTextDraw(playerid, 20.000, 174.000, "INTERACTION_MENU");
	PlayerTextDrawLetterSize(playerid, tdInteractionMenu[playerid][MENU_HEADER_TEXT], 0.130, 0.899);
	PlayerTextDrawTextSize(playerid, tdInteractionMenu[playerid][MENU_HEADER_TEXT], 148.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdInteractionMenu[playerid][MENU_HEADER_TEXT], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdInteractionMenu[playerid][MENU_HEADER_TEXT], COLOR_MENU_HEADER_TEXT);
	PlayerTextDrawSetShadow(playerid, tdInteractionMenu[playerid][MENU_HEADER_TEXT], 1);
	PlayerTextDrawSetOutline(playerid, tdInteractionMenu[playerid][MENU_HEADER_TEXT], 1);
	PlayerTextDrawBackgroundColour(playerid, tdInteractionMenu[playerid][MENU_HEADER_TEXT], 150);
	PlayerTextDrawFont(playerid, tdInteractionMenu[playerid][MENU_HEADER_TEXT], TEXT_DRAW_FONT_2);
	PlayerTextDrawSetProportional(playerid, tdInteractionMenu[playerid][MENU_HEADER_TEXT], true);

	tdInteractionMenu[playerid][MENU_HEADER_ITEM_COUNT] = CreatePlayerTextDraw(playerid, 123.000, 174.000, "1/10");
	PlayerTextDrawLetterSize(playerid, tdInteractionMenu[playerid][MENU_HEADER_ITEM_COUNT], 0.130, 0.899);
	PlayerTextDrawTextSize(playerid, tdInteractionMenu[playerid][MENU_HEADER_ITEM_COUNT], 148.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdInteractionMenu[playerid][MENU_HEADER_ITEM_COUNT], TEXT_DRAW_ALIGN_CENTER);
	PlayerTextDrawColour(playerid, tdInteractionMenu[playerid][MENU_HEADER_ITEM_COUNT], COLOR_MENU_HEADER_TEXT);
	PlayerTextDrawSetShadow(playerid, tdInteractionMenu[playerid][MENU_HEADER_ITEM_COUNT], 1);
	PlayerTextDrawSetOutline(playerid, tdInteractionMenu[playerid][MENU_HEADER_ITEM_COUNT], 1);
	PlayerTextDrawBackgroundColour(playerid, tdInteractionMenu[playerid][MENU_HEADER_ITEM_COUNT], 150);
	PlayerTextDrawFont(playerid, tdInteractionMenu[playerid][MENU_HEADER_ITEM_COUNT], TEXT_DRAW_FONT_2);
	PlayerTextDrawSetProportional(playerid, tdInteractionMenu[playerid][MENU_HEADER_ITEM_COUNT], true);

	tdInteractionMenu[playerid][MENU_BACKGROUND_BOX] = CreatePlayerTextDraw(playerid, 20.000, 186.000, "_");
	PlayerTextDrawLetterSize(playerid, tdInteractionMenu[playerid][MENU_BACKGROUND_BOX], 0.380, 14.299);
	PlayerTextDrawTextSize(playerid, tdInteractionMenu[playerid][MENU_BACKGROUND_BOX], 130.000, 8.000);
	PlayerTextDrawAlignment(playerid, tdInteractionMenu[playerid][MENU_BACKGROUND_BOX], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdInteractionMenu[playerid][MENU_BACKGROUND_BOX], -1);
	PlayerTextDrawUseBox(playerid, tdInteractionMenu[playerid][MENU_BACKGROUND_BOX], true);
	PlayerTextDrawBoxColour(playerid, tdInteractionMenu[playerid][MENU_BACKGROUND_BOX], COLOR_MENU_BACKGROUND_BOX);
	PlayerTextDrawSetShadow(playerid, tdInteractionMenu[playerid][MENU_BACKGROUND_BOX], 1);
	PlayerTextDrawSetOutline(playerid, tdInteractionMenu[playerid][MENU_BACKGROUND_BOX], 1);
	PlayerTextDrawBackgroundColour(playerid, tdInteractionMenu[playerid][MENU_BACKGROUND_BOX], 150);
	PlayerTextDrawFont(playerid, tdInteractionMenu[playerid][MENU_BACKGROUND_BOX], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdInteractionMenu[playerid][MENU_BACKGROUND_BOX], true);

	tdInteractionMenu[playerid][MENU_FOOTER_TEXT] = CreatePlayerTextDraw(playerid, 20.000, 296.000, "ITEM_DESCRIPTION");
	PlayerTextDrawLetterSize(playerid, tdInteractionMenu[playerid][MENU_FOOTER_TEXT], 0.180, 0.899);
	PlayerTextDrawTextSize(playerid, tdInteractionMenu[playerid][MENU_FOOTER_TEXT], 122.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdInteractionMenu[playerid][MENU_FOOTER_TEXT], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdInteractionMenu[playerid][MENU_FOOTER_TEXT], COLOR_MENU_ITEM_DESCRIPTION);
	PlayerTextDrawSetShadow(playerid, tdInteractionMenu[playerid][MENU_FOOTER_TEXT], 0);
	PlayerTextDrawSetOutline(playerid, tdInteractionMenu[playerid][MENU_FOOTER_TEXT], 0);
	PlayerTextDrawBackgroundColour(playerid, tdInteractionMenu[playerid][MENU_FOOTER_TEXT], 150);
	PlayerTextDrawFont(playerid, tdInteractionMenu[playerid][MENU_FOOTER_TEXT], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdInteractionMenu[playerid][MENU_FOOTER_TEXT], true);

	tdInteractionMenu[playerid][MENU_ITEM_FOOTER_ARROW] = CreatePlayerTextDraw(playerid, 68.000, 288.000, "V");
	PlayerTextDrawLetterSize(playerid, tdInteractionMenu[playerid][MENU_ITEM_FOOTER_ARROW], 0.290, 0.599);
	PlayerTextDrawTextSize(playerid, tdInteractionMenu[playerid][MENU_ITEM_FOOTER_ARROW], 126.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdInteractionMenu[playerid][MENU_ITEM_FOOTER_ARROW], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdInteractionMenu[playerid][MENU_ITEM_FOOTER_ARROW], COLOR_MENU_FOOTER_ARROW);
	PlayerTextDrawSetShadow(playerid, tdInteractionMenu[playerid][MENU_ITEM_FOOTER_ARROW], 0);
	PlayerTextDrawSetOutline(playerid, tdInteractionMenu[playerid][MENU_ITEM_FOOTER_ARROW], -1);
	PlayerTextDrawBackgroundColour(playerid, tdInteractionMenu[playerid][MENU_ITEM_FOOTER_ARROW], 150);
	PlayerTextDrawFont(playerid, tdInteractionMenu[playerid][MENU_ITEM_FOOTER_ARROW], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdInteractionMenu[playerid][MENU_ITEM_FOOTER_ARROW], true);

	tdInteractionMenu[playerid][MENU_FOOTER_BOX] = CreatePlayerTextDraw(playerid, 20.000, 289.000, "_");
	PlayerTextDrawLetterSize(playerid, tdInteractionMenu[playerid][MENU_FOOTER_BOX], 0.479, 0.398);
	PlayerTextDrawTextSize(playerid, tdInteractionMenu[playerid][MENU_FOOTER_BOX], 130.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdInteractionMenu[playerid][MENU_FOOTER_BOX], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdInteractionMenu[playerid][MENU_FOOTER_BOX], -256);
	PlayerTextDrawUseBox(playerid, tdInteractionMenu[playerid][MENU_FOOTER_BOX], true);
	PlayerTextDrawBoxColour(playerid, tdInteractionMenu[playerid][MENU_FOOTER_BOX], COLOR_MENU_SEPARATOR_BOX);
	PlayerTextDrawSetShadow(playerid, tdInteractionMenu[playerid][MENU_FOOTER_BOX], 1);
	PlayerTextDrawSetOutline(playerid, tdInteractionMenu[playerid][MENU_FOOTER_BOX], 1);
	PlayerTextDrawBackgroundColour(playerid, tdInteractionMenu[playerid][MENU_FOOTER_BOX], 150);
	PlayerTextDrawFont(playerid, tdInteractionMenu[playerid][MENU_FOOTER_BOX], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdInteractionMenu[playerid][MENU_FOOTER_BOX], true);

	// == MENU ITEMS ==
	tdMenuItemBox[playerid][MENU_ITEM_1] = CreatePlayerTextDraw(playerid, 20.000, 187.000, "_"); 
	PlayerTextDrawLetterSize(playerid, tdMenuItemBox[playerid][MENU_ITEM_1], 0.479, 1.199);
	PlayerTextDrawTextSize(playerid, tdMenuItemBox[playerid][MENU_ITEM_1], 130.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdMenuItemBox[playerid][MENU_ITEM_1], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdMenuItemBox[playerid][MENU_ITEM_1], COLOR_MENU_ITEM_BOX);
	PlayerTextDrawUseBox(playerid, tdMenuItemBox[playerid][MENU_ITEM_1], true);
	PlayerTextDrawBoxColour(playerid, tdMenuItemBox[playerid][MENU_ITEM_1], COLOR_MENU_ITEM_BOX);
	PlayerTextDrawSetShadow(playerid, tdMenuItemBox[playerid][MENU_ITEM_1], 1);
	PlayerTextDrawSetOutline(playerid, tdMenuItemBox[playerid][MENU_ITEM_1], 1);
	PlayerTextDrawBackgroundColour(playerid, tdMenuItemBox[playerid][MENU_ITEM_1], 150);
	PlayerTextDrawFont(playerid, tdMenuItemBox[playerid][MENU_ITEM_1], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdMenuItemBox[playerid][MENU_ITEM_1], true);

	tdMenuItemText[playerid][MENU_ITEM_1] = CreatePlayerTextDraw(playerid, 20.000, 188.000, "MENU_ITEM_1");
	PlayerTextDrawLetterSize(playerid, tdMenuItemText[playerid][MENU_ITEM_1], 0.180, 0.899);
	PlayerTextDrawTextSize(playerid, tdMenuItemText[playerid][MENU_ITEM_1], 122.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdMenuItemText[playerid][MENU_ITEM_1], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdMenuItemText[playerid][MENU_ITEM_1], COLOR_MENU_ITEM_TEXT);
	PlayerTextDrawSetShadow(playerid, tdMenuItemText[playerid][MENU_ITEM_1], 0);
	PlayerTextDrawSetOutline(playerid, tdMenuItemText[playerid][MENU_ITEM_1], 0);
	PlayerTextDrawBackgroundColour(playerid, tdMenuItemText[playerid][MENU_ITEM_1], 150);
	PlayerTextDrawFont(playerid, tdMenuItemText[playerid][MENU_ITEM_1], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdMenuItemText[playerid][MENU_ITEM_1], true);

	tdMenuItemBox[playerid][MENU_ITEM_2] = CreatePlayerTextDraw(playerid, 20.000, 202.000, "_");
	PlayerTextDrawLetterSize(playerid, tdMenuItemBox[playerid][MENU_ITEM_2], 0.479, 1.199);
	PlayerTextDrawTextSize(playerid, tdMenuItemBox[playerid][MENU_ITEM_2], 130.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdMenuItemBox[playerid][MENU_ITEM_2], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdMenuItemBox[playerid][MENU_ITEM_2], COLOR_MENU_ITEM_BOX);
	PlayerTextDrawUseBox(playerid, tdMenuItemBox[playerid][MENU_ITEM_2], true);
	PlayerTextDrawBoxColour(playerid, tdMenuItemBox[playerid][MENU_ITEM_2], COLOR_MENU_ITEM_BOX);
	PlayerTextDrawSetShadow(playerid, tdMenuItemBox[playerid][MENU_ITEM_2], 1);
	PlayerTextDrawSetOutline(playerid, tdMenuItemBox[playerid][MENU_ITEM_2], 1);
	PlayerTextDrawBackgroundColour(playerid, tdMenuItemBox[playerid][MENU_ITEM_2], 150);
	PlayerTextDrawFont(playerid, tdMenuItemBox[playerid][MENU_ITEM_2], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdMenuItemBox[playerid][MENU_ITEM_2], true);

	tdMenuItemText[playerid][MENU_ITEM_2] = CreatePlayerTextDraw(playerid, 20.000, 203.000, "MENU_ITEM_2");
	PlayerTextDrawLetterSize(playerid, tdMenuItemText[playerid][MENU_ITEM_2], 0.180, 0.899);
	PlayerTextDrawTextSize(playerid, tdMenuItemText[playerid][MENU_ITEM_2], 122.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdMenuItemText[playerid][MENU_ITEM_2], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdMenuItemText[playerid][MENU_ITEM_2], COLOR_MENU_ITEM_TEXT);
	PlayerTextDrawSetShadow(playerid, tdMenuItemText[playerid][MENU_ITEM_2], 0);
	PlayerTextDrawSetOutline(playerid, tdMenuItemText[playerid][MENU_ITEM_2], 0);
	PlayerTextDrawBackgroundColour(playerid, tdMenuItemText[playerid][MENU_ITEM_2], 150);
	PlayerTextDrawFont(playerid, tdMenuItemText[playerid][MENU_ITEM_2], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdMenuItemText[playerid][MENU_ITEM_2], true);

	tdMenuItemBox[playerid][MENU_ITEM_3] = CreatePlayerTextDraw(playerid, 20.000, 217.000, "_");
	PlayerTextDrawLetterSize(playerid, tdMenuItemBox[playerid][MENU_ITEM_3], 0.479, 1.199);
	PlayerTextDrawTextSize(playerid, tdMenuItemBox[playerid][MENU_ITEM_3], 130.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdMenuItemBox[playerid][MENU_ITEM_3], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdMenuItemBox[playerid][MENU_ITEM_3], COLOR_MENU_ITEM_BOX);
	PlayerTextDrawUseBox(playerid, tdMenuItemBox[playerid][MENU_ITEM_3], true);
	PlayerTextDrawBoxColour(playerid, tdMenuItemBox[playerid][MENU_ITEM_3], COLOR_MENU_ITEM_BOX);
	PlayerTextDrawSetShadow(playerid, tdMenuItemBox[playerid][MENU_ITEM_3], 1);
	PlayerTextDrawSetOutline(playerid, tdMenuItemBox[playerid][MENU_ITEM_3], 1);
	PlayerTextDrawBackgroundColour(playerid, tdMenuItemBox[playerid][MENU_ITEM_3], 150);
	PlayerTextDrawFont(playerid, tdMenuItemBox[playerid][MENU_ITEM_3], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdMenuItemBox[playerid][MENU_ITEM_3], true);

	tdMenuItemText[playerid][MENU_ITEM_3] = CreatePlayerTextDraw(playerid, 20.000, 218.000, "MENU_ITEM_3");
	PlayerTextDrawLetterSize(playerid, tdMenuItemText[playerid][MENU_ITEM_3], 0.180, 0.899);
	PlayerTextDrawTextSize(playerid, tdMenuItemText[playerid][MENU_ITEM_3], 122.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdMenuItemText[playerid][MENU_ITEM_3], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdMenuItemText[playerid][MENU_ITEM_3], COLOR_MENU_ITEM_TEXT);
	PlayerTextDrawSetShadow(playerid, tdMenuItemText[playerid][MENU_ITEM_3], 0);
	PlayerTextDrawSetOutline(playerid, tdMenuItemText[playerid][MENU_ITEM_3], 0);
	PlayerTextDrawBackgroundColour(playerid, tdMenuItemText[playerid][MENU_ITEM_3], 150);
	PlayerTextDrawFont(playerid, tdMenuItemText[playerid][MENU_ITEM_3], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdMenuItemText[playerid][MENU_ITEM_3], true);

	tdMenuItemBox[playerid][MENU_ITEM_4] = CreatePlayerTextDraw(playerid, 20.000, 231.000, "_");
	PlayerTextDrawLetterSize(playerid, tdMenuItemBox[playerid][MENU_ITEM_4], 0.479, 1.199);
	PlayerTextDrawTextSize(playerid, tdMenuItemBox[playerid][MENU_ITEM_4], 130.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdMenuItemBox[playerid][MENU_ITEM_4], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdMenuItemBox[playerid][MENU_ITEM_4], COLOR_MENU_ITEM_BOX);
	PlayerTextDrawUseBox(playerid, tdMenuItemBox[playerid][MENU_ITEM_4], true);
	PlayerTextDrawBoxColour(playerid, tdMenuItemBox[playerid][MENU_ITEM_4], COLOR_MENU_ITEM_BOX);
	PlayerTextDrawSetShadow(playerid, tdMenuItemBox[playerid][MENU_ITEM_4], 1);
	PlayerTextDrawSetOutline(playerid, tdMenuItemBox[playerid][MENU_ITEM_4], 1);
	PlayerTextDrawBackgroundColour(playerid, tdMenuItemBox[playerid][MENU_ITEM_4], 150);
	PlayerTextDrawFont(playerid, tdMenuItemBox[playerid][MENU_ITEM_4], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdMenuItemBox[playerid][MENU_ITEM_4], true);

	tdMenuItemText[playerid][MENU_ITEM_4] = CreatePlayerTextDraw(playerid, 20.000, 232.000, "MENU_ITEM_4");
	PlayerTextDrawLetterSize(playerid, tdMenuItemText[playerid][MENU_ITEM_4], 0.180, 0.899);
	PlayerTextDrawTextSize(playerid, tdMenuItemText[playerid][MENU_ITEM_4], 122.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdMenuItemText[playerid][MENU_ITEM_4], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdMenuItemText[playerid][MENU_ITEM_4], COLOR_MENU_ITEM_TEXT);
	PlayerTextDrawSetShadow(playerid, tdMenuItemText[playerid][MENU_ITEM_4], 0);
	PlayerTextDrawSetOutline(playerid, tdMenuItemText[playerid][MENU_ITEM_4], 0);
	PlayerTextDrawBackgroundColour(playerid, tdMenuItemText[playerid][MENU_ITEM_4], 150);
	PlayerTextDrawFont(playerid, tdMenuItemText[playerid][MENU_ITEM_4], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdMenuItemText[playerid][MENU_ITEM_4], true);

	tdMenuItemBox[playerid][MENU_ITEM_5] = CreatePlayerTextDraw(playerid, 20.000, 245.000, "_");
	PlayerTextDrawLetterSize(playerid, tdMenuItemBox[playerid][MENU_ITEM_5], 0.479, 1.199);
	PlayerTextDrawTextSize(playerid, tdMenuItemBox[playerid][MENU_ITEM_5], 130.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdMenuItemBox[playerid][MENU_ITEM_5], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdMenuItemBox[playerid][MENU_ITEM_5], COLOR_MENU_ITEM_BOX);
	PlayerTextDrawUseBox(playerid, tdMenuItemBox[playerid][MENU_ITEM_5], true);
	PlayerTextDrawBoxColour(playerid, tdMenuItemBox[playerid][MENU_ITEM_5], COLOR_MENU_ITEM_BOX);
	PlayerTextDrawSetShadow(playerid, tdMenuItemBox[playerid][MENU_ITEM_5], 1);
	PlayerTextDrawSetOutline(playerid, tdMenuItemBox[playerid][MENU_ITEM_5], 1);
	PlayerTextDrawBackgroundColour(playerid, tdMenuItemBox[playerid][MENU_ITEM_5], 150);
	PlayerTextDrawFont(playerid, tdMenuItemBox[playerid][MENU_ITEM_5], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdMenuItemBox[playerid][MENU_ITEM_5], true);

	tdMenuItemText[playerid][MENU_ITEM_5] = CreatePlayerTextDraw(playerid, 20.000, 246.000, "MENU_ITEM_5");
	PlayerTextDrawLetterSize(playerid, tdMenuItemText[playerid][MENU_ITEM_5], 0.180, 0.899);
	PlayerTextDrawTextSize(playerid, tdMenuItemText[playerid][MENU_ITEM_5], 122.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdMenuItemText[playerid][MENU_ITEM_5], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdMenuItemText[playerid][MENU_ITEM_5], COLOR_MENU_ITEM_TEXT);
	PlayerTextDrawSetShadow(playerid, tdMenuItemText[playerid][MENU_ITEM_5], 0);
	PlayerTextDrawSetOutline(playerid, tdMenuItemText[playerid][MENU_ITEM_5], 0);
	PlayerTextDrawBackgroundColour(playerid, tdMenuItemText[playerid][MENU_ITEM_5], 150);
	PlayerTextDrawFont(playerid, tdMenuItemText[playerid][MENU_ITEM_5], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdMenuItemText[playerid][MENU_ITEM_5], true);

	tdMenuItemBox[playerid][MENU_ITEM_6] = CreatePlayerTextDraw(playerid, 20.000, 259.000, "_");
	PlayerTextDrawLetterSize(playerid, tdMenuItemBox[playerid][MENU_ITEM_6], 0.479, 1.199);
	PlayerTextDrawTextSize(playerid, tdMenuItemBox[playerid][MENU_ITEM_6], 130.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdMenuItemBox[playerid][MENU_ITEM_6], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdMenuItemBox[playerid][MENU_ITEM_6], COLOR_MENU_ITEM_BOX);
	PlayerTextDrawUseBox(playerid, tdMenuItemBox[playerid][MENU_ITEM_6], true);
	PlayerTextDrawBoxColour(playerid, tdMenuItemBox[playerid][MENU_ITEM_6], COLOR_MENU_ITEM_BOX);
	PlayerTextDrawSetShadow(playerid, tdMenuItemBox[playerid][MENU_ITEM_6], 1);
	PlayerTextDrawSetOutline(playerid, tdMenuItemBox[playerid][MENU_ITEM_6], 1);
	PlayerTextDrawBackgroundColour(playerid, tdMenuItemBox[playerid][MENU_ITEM_6], 150);
	PlayerTextDrawFont(playerid, tdMenuItemBox[playerid][MENU_ITEM_6], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdMenuItemBox[playerid][MENU_ITEM_6], true);

	tdMenuItemText[playerid][MENU_ITEM_6] = CreatePlayerTextDraw(playerid, 20.000, 260.000, "MENU_ITEM_6");
	PlayerTextDrawLetterSize(playerid, tdMenuItemText[playerid][MENU_ITEM_6], 0.180, 0.899);
	PlayerTextDrawTextSize(playerid, tdMenuItemText[playerid][MENU_ITEM_6], 122.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdMenuItemText[playerid][MENU_ITEM_6], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdMenuItemText[playerid][MENU_ITEM_6], COLOR_MENU_ITEM_TEXT);
	PlayerTextDrawSetShadow(playerid, tdMenuItemText[playerid][MENU_ITEM_6], 0);
	PlayerTextDrawSetOutline(playerid, tdMenuItemText[playerid][MENU_ITEM_6], 0);
	PlayerTextDrawBackgroundColour(playerid, tdMenuItemText[playerid][MENU_ITEM_6], 150);
	PlayerTextDrawFont(playerid, tdMenuItemText[playerid][MENU_ITEM_6], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdMenuItemText[playerid][MENU_ITEM_6], true);

	tdMenuItemBox[playerid][MENU_ITEM_7] = CreatePlayerTextDraw(playerid, 20.000, 274.000, "_");
	PlayerTextDrawLetterSize(playerid, tdMenuItemBox[playerid][MENU_ITEM_7], 0.479, 1.199);
	PlayerTextDrawTextSize(playerid, tdMenuItemBox[playerid][MENU_ITEM_7], 130.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdMenuItemBox[playerid][MENU_ITEM_7], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdMenuItemBox[playerid][MENU_ITEM_7], COLOR_MENU_ITEM_BOX);
	PlayerTextDrawUseBox(playerid, tdMenuItemBox[playerid][MENU_ITEM_7], true);
	PlayerTextDrawBoxColour(playerid, tdMenuItemBox[playerid][MENU_ITEM_7], COLOR_MENU_ITEM_BOX);
	PlayerTextDrawSetShadow(playerid, tdMenuItemBox[playerid][MENU_ITEM_7], 1);
	PlayerTextDrawSetOutline(playerid, tdMenuItemBox[playerid][MENU_ITEM_7], 1);
	PlayerTextDrawBackgroundColour(playerid, tdMenuItemBox[playerid][MENU_ITEM_7], 150);
	PlayerTextDrawFont(playerid, tdMenuItemBox[playerid][MENU_ITEM_7], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdMenuItemBox[playerid][MENU_ITEM_7], true);

	tdMenuItemText[playerid][MENU_ITEM_7] = CreatePlayerTextDraw(playerid, 20.000, 275.000, "MENU_ITEM_7");
	PlayerTextDrawLetterSize(playerid, tdMenuItemText[playerid][MENU_ITEM_7], 0.180, 0.899);
	PlayerTextDrawTextSize(playerid, tdMenuItemText[playerid][MENU_ITEM_7], 122.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdMenuItemText[playerid][MENU_ITEM_7], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, tdMenuItemText[playerid][MENU_ITEM_7], COLOR_MENU_ITEM_TEXT);
	PlayerTextDrawSetShadow(playerid, tdMenuItemText[playerid][MENU_ITEM_7], 0);
	PlayerTextDrawSetOutline(playerid, tdMenuItemText[playerid][MENU_ITEM_7], 0);
	PlayerTextDrawBackgroundColour(playerid, tdMenuItemText[playerid][MENU_ITEM_7], 150);
	PlayerTextDrawFont(playerid, tdMenuItemText[playerid][MENU_ITEM_7], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdMenuItemText[playerid][MENU_ITEM_7], true);

	tdMenuItemScroll[playerid][MENU_ITEM_1] = CreatePlayerTextDraw(playerid, 128.000, 188.000, "<Scroll 1>");
	PlayerTextDrawLetterSize(playerid, tdMenuItemScroll[playerid][MENU_ITEM_1], 0.180, 0.898);
	PlayerTextDrawTextSize(playerid, tdMenuItemScroll[playerid][MENU_ITEM_1], 122.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdMenuItemScroll[playerid][MENU_ITEM_1], TEXT_DRAW_ALIGN_RIGHT);
	PlayerTextDrawColour(playerid, tdMenuItemScroll[playerid][MENU_ITEM_1], 255);
	PlayerTextDrawSetShadow(playerid, tdMenuItemScroll[playerid][MENU_ITEM_1], 0);
	PlayerTextDrawSetOutline(playerid, tdMenuItemScroll[playerid][MENU_ITEM_1], 0);
	PlayerTextDrawBackgroundColour(playerid, tdMenuItemScroll[playerid][MENU_ITEM_1], 150);
	PlayerTextDrawFont(playerid, tdMenuItemScroll[playerid][MENU_ITEM_1], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdMenuItemScroll[playerid][MENU_ITEM_1], true);

	// == MENU ITEM SCROLLABLUE VALUES ==
	tdMenuItemScroll[playerid][MENU_ITEM_2] = CreatePlayerTextDraw(playerid, 128.000, 203.000, "<Scroll 2>");
	PlayerTextDrawLetterSize(playerid, tdMenuItemScroll[playerid][MENU_ITEM_2], 0.180, 0.898);
	PlayerTextDrawTextSize(playerid, tdMenuItemScroll[playerid][MENU_ITEM_2], 122.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdMenuItemScroll[playerid][MENU_ITEM_2], TEXT_DRAW_ALIGN_RIGHT);
	PlayerTextDrawColour(playerid, tdMenuItemScroll[playerid][MENU_ITEM_2], -1);
	PlayerTextDrawSetShadow(playerid, tdMenuItemScroll[playerid][MENU_ITEM_2], 0);
	PlayerTextDrawSetOutline(playerid, tdMenuItemScroll[playerid][MENU_ITEM_2], 0);
	PlayerTextDrawBackgroundColour(playerid, tdMenuItemScroll[playerid][MENU_ITEM_2], 150);
	PlayerTextDrawFont(playerid, tdMenuItemScroll[playerid][MENU_ITEM_2], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdMenuItemScroll[playerid][MENU_ITEM_2], true);

	tdMenuItemScroll[playerid][MENU_ITEM_3] = CreatePlayerTextDraw(playerid, 128.000, 218.000, "<Scroll 3>");
	PlayerTextDrawLetterSize(playerid, tdMenuItemScroll[playerid][MENU_ITEM_3], 0.180, 0.898);
	PlayerTextDrawTextSize(playerid, tdMenuItemScroll[playerid][MENU_ITEM_3], 122.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdMenuItemScroll[playerid][MENU_ITEM_3], TEXT_DRAW_ALIGN_RIGHT);
	PlayerTextDrawColour(playerid, tdMenuItemScroll[playerid][MENU_ITEM_3], -1);
	PlayerTextDrawSetShadow(playerid, tdMenuItemScroll[playerid][MENU_ITEM_3], 0);
	PlayerTextDrawSetOutline(playerid, tdMenuItemScroll[playerid][MENU_ITEM_3], 0);
	PlayerTextDrawBackgroundColour(playerid, tdMenuItemScroll[playerid][MENU_ITEM_3], 150);
	PlayerTextDrawFont(playerid, tdMenuItemScroll[playerid][MENU_ITEM_3], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdMenuItemScroll[playerid][MENU_ITEM_3], true);

	tdMenuItemScroll[playerid][MENU_ITEM_4] = CreatePlayerTextDraw(playerid, 128.000, 232.000, "<Scroll 4>");
	PlayerTextDrawLetterSize(playerid, tdMenuItemScroll[playerid][MENU_ITEM_4], 0.180, 0.898);
	PlayerTextDrawTextSize(playerid, tdMenuItemScroll[playerid][MENU_ITEM_4], 122.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdMenuItemScroll[playerid][MENU_ITEM_4], TEXT_DRAW_ALIGN_RIGHT);
	PlayerTextDrawColour(playerid, tdMenuItemScroll[playerid][MENU_ITEM_4], -1);
	PlayerTextDrawSetShadow(playerid, tdMenuItemScroll[playerid][MENU_ITEM_4], 0);
	PlayerTextDrawSetOutline(playerid, tdMenuItemScroll[playerid][MENU_ITEM_4], 0);
	PlayerTextDrawBackgroundColour(playerid, tdMenuItemScroll[playerid][MENU_ITEM_4], 150);
	PlayerTextDrawFont(playerid, tdMenuItemScroll[playerid][MENU_ITEM_4], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdMenuItemScroll[playerid][MENU_ITEM_4], true);

	tdMenuItemScroll[playerid][MENU_ITEM_5] = CreatePlayerTextDraw(playerid, 128.000, 246.000, "<Scroll 5>");
	PlayerTextDrawLetterSize(playerid, tdMenuItemScroll[playerid][MENU_ITEM_5], 0.180, 0.898);
	PlayerTextDrawTextSize(playerid, tdMenuItemScroll[playerid][MENU_ITEM_5], 122.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdMenuItemScroll[playerid][MENU_ITEM_5], TEXT_DRAW_ALIGN_RIGHT);
	PlayerTextDrawColour(playerid, tdMenuItemScroll[playerid][MENU_ITEM_5], -1);
	PlayerTextDrawSetShadow(playerid, tdMenuItemScroll[playerid][MENU_ITEM_5], 0);
	PlayerTextDrawSetOutline(playerid, tdMenuItemScroll[playerid][MENU_ITEM_5], 0);
	PlayerTextDrawBackgroundColour(playerid, tdMenuItemScroll[playerid][MENU_ITEM_5], 150);
	PlayerTextDrawFont(playerid, tdMenuItemScroll[playerid][MENU_ITEM_5], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdMenuItemScroll[playerid][MENU_ITEM_5], true);

	tdMenuItemScroll[playerid][MENU_ITEM_6] = CreatePlayerTextDraw(playerid, 128.000, 260.000, "<Scroll 6>");
	PlayerTextDrawLetterSize(playerid, tdMenuItemScroll[playerid][MENU_ITEM_6], 0.180, 0.898);
	PlayerTextDrawTextSize(playerid, tdMenuItemScroll[playerid][MENU_ITEM_6], 122.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdMenuItemScroll[playerid][MENU_ITEM_6], TEXT_DRAW_ALIGN_RIGHT);
	PlayerTextDrawColour(playerid, tdMenuItemScroll[playerid][MENU_ITEM_6], -1);
	PlayerTextDrawSetShadow(playerid, tdMenuItemScroll[playerid][MENU_ITEM_6], 0);
	PlayerTextDrawSetOutline(playerid, tdMenuItemScroll[playerid][MENU_ITEM_6], 0);
	PlayerTextDrawBackgroundColour(playerid, tdMenuItemScroll[playerid][MENU_ITEM_6], 150);
	PlayerTextDrawFont(playerid, tdMenuItemScroll[playerid][MENU_ITEM_6], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdMenuItemScroll[playerid][MENU_ITEM_6], true);

	tdMenuItemScroll[playerid][MENU_ITEM_7] = CreatePlayerTextDraw(playerid, 128.000, 275.000, "<Scroll 7>");
	PlayerTextDrawLetterSize(playerid, tdMenuItemScroll[playerid][MENU_ITEM_7], 0.180, 0.898);
	PlayerTextDrawTextSize(playerid, tdMenuItemScroll[playerid][MENU_ITEM_7], 122.000, -2.000);
	PlayerTextDrawAlignment(playerid, tdMenuItemScroll[playerid][MENU_ITEM_7], TEXT_DRAW_ALIGN_RIGHT);
	PlayerTextDrawColour(playerid, tdMenuItemScroll[playerid][MENU_ITEM_7], -1);
	PlayerTextDrawSetShadow(playerid, tdMenuItemScroll[playerid][MENU_ITEM_7], 0);
	PlayerTextDrawSetOutline(playerid, tdMenuItemScroll[playerid][MENU_ITEM_7], 0);
	PlayerTextDrawBackgroundColour(playerid, tdMenuItemScroll[playerid][MENU_ITEM_7], 150);
	PlayerTextDrawFont(playerid, tdMenuItemScroll[playerid][MENU_ITEM_7], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, tdMenuItemScroll[playerid][MENU_ITEM_7], true);
}

/**
 * <summary>Clears out all the menu textdraws for a player</summary>
 * <param name="playerid">The player whose menu textdraws we want to clean up.</param>
 */
static DestroyIMenu(playerid)
{
	for (new i = 0; i < sizeof(tdInteractionMenu[]); i++)
	{
		PlayerTextDrawDestroy(playerid, tdInteractionMenu[playerid][e_MENU_TD:i]);
		tdInteractionMenu[playerid][e_MENU_TD:i] = INVALID_PLAYER_TEXT_DRAW;
	}

	for (new i = 0; i < sizeof(tdMenuItemBox[]); i++)
	{
		PlayerTextDrawDestroy(playerid, tdMenuItemBox[playerid][e_MENU_ITEM:i]);
		tdMenuItemBox[playerid][e_MENU_ITEM:i] = INVALID_PLAYER_TEXT_DRAW;
	
	
		PlayerTextDrawDestroy(playerid, tdMenuItemText[playerid][e_MENU_ITEM:i]);
		tdMenuItemText[playerid][e_MENU_ITEM:i] = INVALID_PLAYER_TEXT_DRAW;
	
	
		PlayerTextDrawDestroy(playerid, tdMenuItemScroll[playerid][e_MENU_ITEM:i]);
		tdMenuItemScroll[playerid][e_MENU_ITEM:i] = INVALID_PLAYER_TEXT_DRAW;
	}
}

/**
 * <summary>Shows the menu to the player</summary>
 * <remarks>
 * This one has to be called first, to convert string and to be able to open another
 * menu, otherwise the new menu won't open.
 * </remarks>
 * <param name="playerid">Who we’re showing the menu to.</param>
 * <param name="menuid">The menuid to be shown for the player</param>
 * <param name="tdMenuItems">The items string for the menu</param>
 * <param name="title">The menu title to be shown.</param>
 * <param name="desc">Optional description for each item</param>
 * <param name="color">Optional color for the shown menu</param>
 */
stock ShowPlayerInteractionMenu(playerid, menuid, const tdMenuItems[], const title[] = "", const desc[] = "", color = COLOR_MENU_TITLE_BOX)
{
	new sConvertTitle[32],
		sConvertDesc[2048],
		sConvertItems[2048]
	;
	strcat(sConvertTitle, title);
	strcat(sConvertDesc, desc);
	strcat(sConvertItems, tdMenuItems);
	ConvertEncoding(sConvertTitle);
	ConvertEncoding(sConvertDesc);
	ConvertEncoding(sConvertItems);

	if (unfreezeTimer[playerid]) unfreezeTimer[playerid] = KillTimer(unfreezeTimer[playerid]);
	SetTimerEx("DelayShowPlayerInteractionMenu", 150, false, "iisssi", playerid, menuid, sConvertItems, sConvertTitle, sConvertDesc, color);
}

/**
 * <summary>Gets the text for a specific scroll option in a menu item for a player.</summary>
 * <remarks>
 * Formats the string with the scroll item string
 * </remarks>
 * <param name="output">The string to format</param>
 * <param name="playerid">The player with the open menu</param>
 * <param name="imenuitem">Which menu item we’re checking.</param>
 * <param name="scrollitem">Which scroll option inside that item.</param>
 */
stock GetMenuScrollItemString(output[MAX_ITEM_LEN], playerid, imenuitem, scrollitem)
{
	format(output, sizeof(output), "%s", interactionMenuScrollItems[playerid][imenuitem][scrollitem]);
}

/**
 * <summary>Gets the text for a specific menu item for a player.</summary>
 * <remarks>
 * Formats the string with the item string
 * </remarks>
 * <param name="output">The string to format</param>
 * <param name="playerid">The player with the open menu</param>
 * <param name="imenuitem">Which menu item we’re checking.</param>
 */
stock GetMenuItemString(output[MAX_ITEM_LEN], playerid, imenuitem)
{
	format(output, sizeof(output), "%s", interactionMenuItems[playerid][imenuitem]);
}

/**
 * <summary>Updates the menu highlight and display based on the player selection.</summary>
 * <remarks>
 * This function makes the player see which item is currently selected,
 * updates the item count display, and shows the correct description.
 * It also adjusts colors to highlight the selected item and handle item states like locked or scrollable.
 * </remarks>
 * <param name="playerid">The player to update the menu</param>
 * <param name="itemcount">Total number of items in the current menu.</param>
 */
static UpdateMenuHighlight(playerid, itemcount)
{
	UpdateHeaderItemCount(playerid, itemcount);
	UpdateFooterDescription(playerid);

	for (new i = 0; i < sizeof(tdMenuItemText[]); i++)
	{
		new index = iMenuScrollOffset[playerid] + i;

		if (index < itemcount)
		{
			UpdateMenuItem(playerid, e_MENU_ITEM:i, index);
		}
		else
		{
			HideMenuItem(playerid, e_MENU_ITEM:i);
		}
	}
}

/**
 * <summary>Updates the header item count display</summary>
 * <remarks>
 * Updates the header textdraw to show the current selected item number and total item count.
 * </remarks>
 * <param name="playerid">The player to update the header for</param>
 * <param name="itemcount">Total number of items in the current menu.</param>
 */
static UpdateHeaderItemCount(playerid, itemcount)
{
	new msg[16];
	format(msg, sizeof(msg), "%d/%d", iSelectedMenuItemId[playerid] + 1 + iMenuScrollOffset[playerid], itemcount);
	PlayerTextDrawSetString(playerid, tdInteractionMenu[playerid][MENU_HEADER_ITEM_COUNT], msg);
}

/**
 * <summary>Updates the footer description based on the selected item</summary>
 * <remarks>
 * Updates the footer textdraw to show the description of the currently selected menu item.
 * If the description is too long, it truncates it and adds ellipsis.
 * </remarks>
 * <param name="playerid">The player to update the footer for</param>
 */
static UpdateFooterDescription(playerid)
{
	new idx = iMenuScrollOffset[playerid] + iSelectedMenuItemId[playerid];
	if (strlen(interactionMenuDesc[playerid][idx]) >= MAX_DESCRIPTION_LEN - 1)
	{
		strdel(interactionMenuDesc[playerid][idx], MAX_DESCRIPTION_LEN - 4, MAX_DESCRIPTION_LEN);
		strcat(interactionMenuDesc[playerid][idx], "...");
	}

	PlayerTextDrawSetString(playerid, tdInteractionMenu[playerid][MENU_FOOTER_TEXT], interactionMenuDesc[playerid][idx]);
}

/**
 * <summary>Updates a single item</summary>
 * <remarks>Updates the item to have locked colors, or the scroll options</remarks>
 * <param name="playerid">The player who's item we're updating.</param>
 * <param name="slot">The visible item in the textdraw. (e_MENU_ITEM constant)</param>
 * <param name="index">The overall item position. (0 to MAX_MENU_ITEMS)</param>
 */
static UpdateMenuItem(playerid, e_MENU_ITEM:slot, index)
{
	PlayerTextDrawSetString(playerid, tdMenuItemText[playerid][slot], interactionMenuItems[playerid][index]);

	if (slot == e_MENU_ITEM:iSelectedMenuItemId[playerid]) HighlightMenuItem(playerid, slot, index);
	else ResetMenuItemColor(playerid, slot, index);

	if (bIsItemLocked[playerid][index]) UpdateLockedItem(playerid, slot);

	if (bIsItemScrollable[playerid][index]) UpdateScrollableItem(playerid, slot, index);
	else PlayerTextDrawSetString(playerid, tdMenuItemScroll[playerid][slot], "");

	// Always show textdraws for active items
	PlayerTextDrawShow(playerid, tdInteractionMenu[playerid][MENU_FOOTER_TEXT]);
	PlayerTextDrawShow(playerid, tdMenuItemBox[playerid][slot]);
	PlayerTextDrawShow(playerid, tdMenuItemText[playerid][slot]);
	PlayerTextDrawShow(playerid, tdMenuItemScroll[playerid][slot]);
}

/**
 * <summary>Hides a single item</summary>
 * <remarks>Hides the item textdraws when there are no more items to show</remarks>
 * <param name="playerid">The player who's item we're updating.</param>
 * <param name="slot">The visible item in the textdraw. (e_MENU_ITEM constant)</param>
 */
static HideMenuItem(playerid, e_MENU_ITEM:slot)
{
	PlayerTextDrawSetString(playerid, tdMenuItemText[playerid][slot], "");
	PlayerTextDrawSetString(playerid, tdMenuItemScroll[playerid][slot], "");
	PlayerTextDrawHide(playerid, tdMenuItemBox[playerid][slot]);
	PlayerTextDrawHide(playerid, tdMenuItemText[playerid][slot]);
	PlayerTextDrawHide(playerid, tdMenuItemScroll[playerid][slot]);
}

/**
 * <summary>Highlights the item color</summary>
 * <remarks>When the item is selected, update it's color to the lit state.</remarks>
 * <param name="playerid">The player who's item we're updating.</param>
 * <param name="slot">The visible item in the textdraw. (e_MENU_ITEM constant)</param>
 * <param name="index">The overall item position. (0 - MAX_MENU_ITEMS)</param>
 */
static HighlightMenuItem(playerid, e_MENU_ITEM:slot, index)
{
	if (bIsItemEx[playerid][index])
	{
		PlayerTextDrawBoxColour(playerid, tdMenuItemBox[playerid][slot], boxExHighlight[playerid][index]);
	}
	else
	{
		PlayerTextDrawBoxColour(playerid, tdMenuItemBox[playerid][slot], COLOR_MENU_ITEM_BOX_HIGHLIGHT);
		PlayerTextDrawColour(playerid, tdMenuItemText[playerid][slot], COLOR_MENU_ITEM_TEXT_HIGHLIGHT);
		PlayerTextDrawColour(playerid, tdMenuItemScroll[playerid][slot], COLOR_MENU_ITEM_TEXT_HIGHLIGHT);
	}
}

/**
 * <summary>Resets the item color</summary>
 * <remarks>When the item is not selected, update it's color to the unlit state.</remarks>
 * <param name="playerid">The player who's item we're updating.</param>
 * <param name="slot">The visible item in the textdraw. (e_MENU_ITEM constant)</param>
 * <param name="index">The overall item position. (0 - MAX_MENU_ITEMS)</param>
 */
static ResetMenuItemColor(playerid, e_MENU_ITEM:slot, index)
{
	if (bIsItemEx[playerid][index])
	{
		PlayerTextDrawBoxColour(playerid, tdMenuItemBox[playerid][slot], boxExColor[playerid][index]);
	}
	else
	{
		PlayerTextDrawBoxColour(playerid, tdMenuItemBox[playerid][e_MENU_ITEM:slot], COLOR_MENU_ITEM_BOX);
		PlayerTextDrawColour(playerid, tdMenuItemText[playerid][e_MENU_ITEM:slot], COLOR_MENU_ITEM_TEXT);
		PlayerTextDrawColour(playerid, tdMenuItemScroll[playerid][e_MENU_ITEM:slot], COLOR_MENU_ITEM_TEXT);
	}
}

/**
 * <summary>Updates the locked item color</summary>
 * <remarks>When the item is locked, update it's color to the locked state.</remarks
 * <param name="playerid">The player who's item we're updating.</param>
 * <param name="slot">The visible item in the textdraw. (e_MENU_ITEM constant)</param>
 */
static UpdateLockedItem(playerid, e_MENU_ITEM:slot)
{
	PlayerTextDrawColour(playerid, tdMenuItemText[playerid][slot], COLOR_MENU_LOCKED_ITEM);
	PlayerTextDrawColour(playerid, tdMenuItemScroll[playerid][slot], COLOR_MENU_LOCKED_ITEM);
}

/**
 * <summary>Updates the scrollable item text</summary>
 * <remarks>When the item is scrollable, update it's text to show the current selected option.</remarks>
 * <param name="playerid">The player who's item we're updating.</param>
 * <param name="slot">The visible item in the textdraw. (e_MENU_ITEM constant)</param>
 * <param name="index">The overall item position. (0 - MAX_MENU_ITEMS)</param>
 */
static UpdateScrollableItem(playerid, e_MENU_ITEM:slot, index)
{
	new str[128], scroll[MAX_ITEM_LEN];
	strins((scroll[0] = EOS, scroll), interactionMenuScrollItems[playerid][index][interactionMenuScrollIndex[playerid][index]], 0, sizeof(scroll));
	format(str, sizeof str, "<%s>", scroll);
	PlayerTextDrawSetString(playerid, tdMenuItemScroll[playerid][slot], str);
}

/**
 * <summary>Initializes all menu variables</summary>
 * <remarks>Resets and initializes all menu variables when openning a new menu</remarks>
 * <param name="playerid">The player who's menu we are initializing</param>
 * <param name="menuid">The menu id to initialize</param>
 */
static InitMenuState(playerid, menuid)
{
	#if defined _FOREACH_LOCAL_VERSION
		if (!Iter_Contains(itIsInMenu, playerid))
		{
			TogglePlayerControllable(playerid, false);
			Iter_Add(itHasLastMenu, playerid);
		}
		else HideInteractionMenu(playerid);
		Iter_Add(itIsInMenu, playerid);

	#else 
		if (!bIsInMenu[playerid])
		{
			TogglePlayerControllable(playerid, false);
			bHasLastMenu[playerid] = true;
		}
		else HideInteractionMenu(playerid);
		bIsInMenu[playerid] = true;

	#endif

	iMenuId[playerid] = menuid;
	iSelectedMenuItemId[playerid] = 0;
	iMenuScrollOffset[playerid] = 0;
	interactionMenuItemCount[playerid] = 0;
}

/**
 * <summary>Applies the menu title and color</summary>
 * <remarks>Applies the styling and updates the textdraws for the player to see</remarks>
 * <param name="playerid">The player who's menu we're updating.</param>
 * <param name="title">The title to update</param>
 * <param name="color">The menu header color.</param>
 */
static ApplyMenuStyling(playerid, const title[], color)
{
	PlayerTextDrawSetString(playerid, tdInteractionMenu[playerid][MENU_TITLE], title);

	PlayerTextDrawBoxColour(playerid,  tdInteractionMenu[playerid][MENU_TITLE_BOX], color);

	PlayerTextDrawColour(playerid, tdInteractionMenu[playerid][MENU_HEADER_TEXT], color);

	PlayerTextDrawColour(playerid, tdInteractionMenu[playerid][MENU_HEADER_ITEM_COUNT], color);
}

/**
 * <summary>Updates all the menu textdraws</summary>
 * <remarks>Shows all menu textdraws for the player to update if changes were made</remarks>
 * <param name="playerid">The player who's menu we're updating.</param>
 */
static UpdateMenuTextdraws(playerid)
{
	for (new i = 0; i < sizeof(tdInteractionMenu[]); i++)
	{
		PlayerTextDrawShow(playerid, tdInteractionMenu[playerid][e_MENU_TD:i]);
	}
	for (new i = 0; i < sizeof(tdMenuItemText[]); i++)
	{
		PlayerTextDrawShow(playerid, tdMenuItemBox[playerid][e_MENU_ITEM:i]);
		PlayerTextDrawShow(playerid, tdMenuItemText[playerid][e_MENU_ITEM:i]);
		PlayerTextDrawShow(playerid, tdMenuItemScroll[playerid][e_MENU_ITEM:i]);
	}
}

/**
 * <summary>Formats all items string</summary>
 * <remarks>Separates each menu item from the big string, into it's own variable</remarks>
 * <param name="playerid">The player who's item we're formating.</param>
 * <param name="MenuItem">The big string with all items</param>
 */
static FormatMenuItems(playerid, const MenuItems[])
{
	new msg[1028];
	format(msg, sizeof msg, "%s\n", MenuItems);

	for(new i; i < MAX_MENU_ITEMS; i++)
	{
		strmid(interactionMenuItems[playerid][i], msg, 0, strfind(msg, "\n", true));

		ItemPropertyCheck(playerid, interactionMenuItems[playerid][i]);

		if (strfind(msg, "\n", true) != -1) interactionMenuItemCount[playerid] ++;
		strdel(msg, 0, strfind(msg, "\n", true) + 1);
	}
}

/**
 * <summary>Formats all descriptions string</summary>
 * <remarks>Separates each menu item description from the big string, into it's own variable</remarks>
 * <param name="playerid">The player who's description we're formating.</param>
 * <param name="MenuDesc">The big string with all descriptions</param>
 */
static FormatMenuDescriptions(playerid, const menu_desc[])
{
	new str[2048];
	format(str, sizeof str, "%s\n", menu_desc);

	for (new i; i < MAX_MENU_ITEMS; i++)
	{
		strmid(interactionMenuDesc[playerid][i], str, 0, strfind(str, "\n", true));
		strdel(str, 0, strfind(str, "\n", true) + 1);

		if (strlen(interactionMenuDesc[playerid][i]) <= 0)
		{
			interactionMenuDesc[playerid][i] = "No description";
		}
	}
}

/**
 * <summary>Checks if the player is in a menu</summary>
 * <remarks>Returns true if the player currently has a menu open</remarks>
 * <param name="playerid">The player who's description we're formating.</param>
 * <returns>True if the player is in a menu</returns>
 */
bool:IsPlayerInMenu(playerid)
{
	#if defined _FOREACH_LOCAL_VERSION
		if (Iter_Contains(itIsInMenu, playerid)) return true;
		else return false;
	#else
		return bIsInMenu[playerid];
	#endif
}

/**
 * <summary>Checks if we can process the player's command</summary>
 * <remarks>Checks if the player can send a command to the menu, to prevent spamming</remarks>
 * <param name="playerid">The player who's description we're formating.</param>
 * <param name="MenuDesc">The big string with all descriptions</param>
 * <returns>True if the command can be processed</returns>
 */
bool:CanProcessMenuInput(playerid)
{
	new tick = GetTickCount();
	if (tick - lastActionTick[playerid] >= 1000)
	{
		lastActionTick[playerid] = tick;
		actionCount[playerid] = 0;
	}
	return true;
}

/**
 * <summary>Handles up / down menu navigation</summary>
 * <remarks>Checks if the player is sending the up / down key</remarks>
 * <param name="playerid">The player who's sending the command</param>
 * <param name="updown">The KEY_DOWN or KEY_UP variable</param>
 * <param name="index">The curent item selected. (0 - MAX_MENU_ITEMS)</param>
 */
HandleMenuNavigation(playerid, updown, index)
{
	if (updown == KEY_UP)
	{
		if (iSelectedMenuItemId[playerid] > 0) 
		{
			iSelectedMenuItemId[playerid]--;
			PlayerPlaySound(playerid, 1084);
		}
		else if (iMenuScrollOffset[playerid] > 0) 
		{
			iMenuScrollOffset[playerid]--;
			PlayerPlaySound(playerid, 1084);
		}

		UpdateMenuHighlight(playerid, interactionMenuItemCount[playerid]);
		actionCount[playerid]++;
	}
	else if (updown == KEY_DOWN)
	{
		if (iSelectedMenuItemId[playerid] < sizeof(tdMenuItemText[]) - 1 
		&& index < interactionMenuItemCount[playerid] - 1)
		{
			iSelectedMenuItemId[playerid]++;
			PlayerPlaySound(playerid, 1084);
		}
		else if (iMenuScrollOffset[playerid] + sizeof(tdMenuItemText[]) < interactionMenuItemCount[playerid])
		{
			iMenuScrollOffset[playerid]++;
			PlayerPlaySound(playerid, 1084);
		}

		UpdateMenuHighlight(playerid, interactionMenuItemCount[playerid]);
		actionCount[playerid]++;
	}
}

/**
 * <summary>Handles closing and selecting on the menu.</summary>
 * <remarks>Checks if the player is sending the close or select key</remarks>
 * <param name="playerid">The player who's sending the command</param>
 * <param name="keys">The pressed key variable</param>
 * <param name="index">The curent item selected. (0 - MAX_MENU_ITEMS)</param>
 */
HandleMenuSelection(playerid, KEY:keys, index)
{
	if (keys & KEY_SPRINT)
	{
		if (!bIsItemLocked[playerid][index])
		{
			HandleTDMenuItem(playerid, iSelectedMenuItemId[playerid], iMenuId[playerid], true);
			HideInteractionMenu(playerid);
		}
		else PlayerPlaySound(playerid, 1085);
		actionCount[playerid]++;
	}
	else if (keys & KEY_SECONDARY_ATTACK)
	{
		HideInteractionMenu(playerid);
		HandleTDMenuItem(playerid, iSelectedMenuItemId[playerid], iMenuId[playerid], false);
		PlayerPlaySound(playerid, 1085);
		actionCount[playerid]++;
	}
}

/**
 * <summary>Handles left / right menu navigation</summary>
 * <remarks>Checks if the player is sending the left / right key</remarks>
 * <param name="playerid">The player who's sending the command</param>
 * <param name="updown">The KEY_LEFT or KEY_RIGHT variable</param>
 * <param name="index">The curent item selected. (0 - MAX_MENU_ITEMS)</param>
 */
HandleMenuScrolling(playerid, leftright, index)
{
	if (!bIsItemScrollable[playerid][index]) return;
	if (bIsItemLocked[playerid][index]) return;

	
	if (interactionMenuScrollCount[playerid][index] <= 0) return;

	if (leftright == KEY_RIGHT)
	{
		interactionMenuScrollIndex[playerid][index]++;
		if (interactionMenuScrollIndex[playerid][index] >= interactionMenuScrollCount[playerid][index]) 
			interactionMenuScrollIndex[playerid][index] = 0;

		HandleScrollItem(playerid, iMenuId[playerid], index, interactionMenuScrollIndex[playerid][index]);
		UpdateMenuHighlight(playerid, interactionMenuItemCount[playerid]);
		actionCount[playerid]++;
	}
	else if (leftright == KEY_LEFT)
	{
		if (interactionMenuScrollIndex[playerid][index] == 0) 
			interactionMenuScrollIndex[playerid][index] = interactionMenuScrollCount[playerid][index] - 1;
		else 
			interactionMenuScrollIndex[playerid][index]--;

		HandleScrollItem(playerid, iMenuId[playerid], index, interactionMenuScrollIndex[playerid][index]);
		UpdateMenuHighlight(playerid, interactionMenuItemCount[playerid]);
		actionCount[playerid]++;
	}
}

/**
 * <summary>Converts the encoding of a string to the real characters used in SA:MP.</summary>
 * <remarks>This function is used to convert strings that may contain characters outside the standard ASCII range
 * to the real characters used in SA:MP, ensuring proper display of text in menus and text draws.</remarks>
 * <param name="string">The string that needs to be converted</param>
 * <returns>Doesn't return anything, acts directly on the string</returns>
 */
static stock ConvertEncoding(string[])
{
	static const scRealChars[256] =
	{
		0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
		16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
		32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
		48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
		64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
		80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
		96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
		112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
		128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
		144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
		160,  94, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
		124, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 175,
		128, 129, 130, 195, 131, 197, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141,
		208, 173, 142, 143, 144, 213, 145, 215, 216, 146, 147, 148, 149, 221, 222, 150,
		151, 152, 153, 227, 154, 229, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
		240, 174, 165, 166, 167, 245, 168, 247, 248, 169, 170, 171, 172, 253, 254, 255
	};
	if (ispacked(string))
	{
		for (new i = 0, len = strlen(string); i != len; ++i)
		{
			string{i} = scRealChars[string{i}];
		}
	}
	else
	{
		for (new i = 0, len = strlen(string), ch; i != len; ++i)
		{
			if (0 <= (ch = string[i]) < 256)
			{
				string[i] = scRealChars[ch];
			}
		}
	}
}

/**
 * <summary>This callback is called when a player changes a scroll option in the menu.</summary>
 * <remarks>
 * This is called when the player changes the scroll item NOT when it's clicked
 * </remarks>
 * <param name="playerid">The player who changed the scroll option.</param>
 * <param name="menuid">The ID of the menu being used.</param>
 * <param name="menuitem">The specific item that was scrolled.</param>
 * <param name="scrollitem">The new scroll choice the player picked.</param>
 */
static HandleScrollItem(playerid, menuid, menuitem, scrollitem)
{
	CallRemoteFunction("OnScrollItemChange", "iiii", playerid, menuid, menuitem, scrollitem);
}

/**
 * <summary>This callback is called when a player clicks a menu item, or exits the menu.</summary>
 * <remarks>
 * if player exits the menu, the selected item is -1 and response is false.
 * </remarks>
 * <param name="playerid">The player who changed the scroll option.</param>
 * <param name="selected">The specific item that was selected</param>
 * <param name="menuid">The ID of the menu being used.</param>
 * <param name="response">whether the player selected the menu or not</param>
 */
static HandleTDMenuItem(playerid, selected, menuid, bool:response)
{
	#if defined _FOREACH_LOCAL_VERSION
		if (Iter_Contains(itHasLastMenu, playerid))
		{
			if (unfreezeTimer[playerid]) KillTimer(unfreezeTimer[playerid]);
			unfreezeTimer[playerid] = SetTimerEx("DelayFromLastMenu", 300, false, "i", playerid);
		}
	#else
		if (bHasLastMenu[playerid])
		{
			if (unfreezeTimer[playerid]) KillTimer(unfreezeTimer[playerid]);
			unfreezeTimer[playerid] = SetTimerEx("DelayFromLastMenu", 300, false, "i", playerid);
		}
	#endif

	new index = iMenuScrollOffset[playerid] + selected;	
	CallRemoteFunction("OnInteractionMenuResponse", "iibi", playerid, menuid, response, response ? index : -1);
}

/**
 * <summary>Closes the interaction menu for a player.</summary>
 * <remarks>
 * Called when a player closed the menu or clicked on an item
 * </remarks>
 * <param name="playerid">The player to close the menu.</param>
 */
HideInteractionMenu(playerid)
{
	#if defined _FOREACH_LOCAL_VERSION
		Iter_Remove(itIsInMenu, playerid);
	#else
		bIsInMenu[playerid] = false;
	#endif

	iSelectedMenuItemId[playerid] = 0;

	for (new i = 0; i < sizeof(tdAndroidButtons[]); i++)
	{
		PlayerTextDrawHide(playerid, tdAndroidButtons[playerid][e_CONTROL_BUTTON:i]);
	}
	for (new i = 0; i < sizeof(tdAndroidButtons[]); i++)
	{
		PlayerTextDrawHide(playerid, tdAndroidText[playerid][e_CONTROL_BUTTON:i]);
	}

	for (new i = 0; i < sizeof(tdMenuItemText[]); i++)
	{
		PlayerTextDrawHide(playerid, tdMenuItemBox[playerid][e_MENU_ITEM:i]);
		PlayerTextDrawHide(playerid, tdMenuItemText[playerid][e_MENU_ITEM:i]);
		PlayerTextDrawHide(playerid, tdMenuItemScroll[playerid][e_MENU_ITEM:i]);
	}
	for (new i = 0; i < sizeof(tdInteractionMenu[]); i++)
	{
		PlayerTextDrawHide(playerid, tdInteractionMenu[playerid][e_MENU_TD:i]);
	}

	for (new i = 0; i < MAX_MENU_ITEMS; i++)
	{
		if (bIsItemLocked[playerid][i]) bIsItemLocked[playerid][i] = false; 
		
		if (bIsItemEx[playerid][i])
		{
			bIsItemEx[playerid][i] = false;
			boxExColor[playerid][i] = COLOR_MENU_ITEM_BOX;
			boxExHighlight[playerid][i] = COLOR_MENU_ITEM_BOX_HIGHLIGHT;
		}

		if (bIsItemScrollable[playerid][i])
		{
			bIsItemScrollable[playerid][i] = false;
			interactionMenuScrollCount[playerid][i] = 0;
			interactionMenuScrollIndex[playerid][i] = 0;
		}
	}
}

/**
 * <summary>Removes a specific item from the player's menu.</summary>
 * <param name="playerid">The player to remove the item from</param>
 * <param name="itemid">The item index to be removed</param>
*/
stock RemoveInteractionMenuItem(playerid, itemid)
{
	interactionMenuItems[playerid][itemid] = "_";
	interactionMenuItemCount[playerid] --;
	UpdateMenuHighlight(playerid, interactionMenuItemCount[playerid]);
}

/**
 * <summary>Adds a new item to the player's menu</summary>
 * <remarks>This updates the menu so the player can see the new item and its description</remarks>
 * <param name="playerid">The player who will see this menu item.</param>
 * <param name="item">The text of the menu item being added.</param>
 * <param name="desc">Description for the menu item; defaults to "No Description" if none is provided.</param>
*/
stock AddInteractionMenuItem(playerid, const item[MAX_ITEM_LEN], const desc[MAX_DESCRIPTION_LEN] = "No Description")
{
	ItemPropertyCheck(playerid, item);

	interactionMenuDesc[playerid][interactionMenuItemCount[playerid]] = desc;
	interactionMenuItemCount[playerid] ++;
	
	UpdateMenuHighlight(playerid, interactionMenuItemCount[playerid]);
}
/**
 * <summary>Adds a new item to the player's interaction menu with a custom box color</summary>
 * <remarks>This updates the menu so the player can see the new item and its description</remarks>
 * <param name="playerid">The player who will see this menu item.</param>
 * <param name="item">The text of the menu item being added.</param>
 * <param name="boxcolor">The color of the item being added</param>
 * <param name="desc">Description for the menu item; defaults to "No Description" if none is provided.</param>
*/
stock AddInteractionMenuItemEx(playerid, const item[MAX_ITEM_LEN], const boxcolor = COLOR_MENU_ITEM_BOX, const desc[MAX_DESCRIPTION_LEN] = "No Description")
{
	ItemPropertyCheck(playerid, item);

	interactionMenuDesc[playerid][interactionMenuItemCount[playerid]] = desc;

	boxExColor[playerid][interactionMenuItemCount[playerid]] = (100 << 24) | (boxcolor & 0xFFFFFF);

	boxExHighlight[playerid][interactionMenuItemCount[playerid]] = (255 << 24) | (boxcolor & 0xFFFFFF);

	bIsItemEx[playerid][interactionMenuItemCount[playerid]] = true;
	interactionMenuItemCount[playerid]++;

	UpdateMenuHighlight(playerid, interactionMenuItemCount[playerid]);
}

stock ShowAndroidButtons(playerid)
{
	for (new i = 0; i < 6; i++)
	{
		PlayerTextDrawShow(playerid, tdAndroidButtons[playerid][e_CONTROL_BUTTON:i]);
	}
	for (new i = 0; i < 6; i++)
	{
		PlayerTextDrawShow(playerid, tdAndroidText[playerid][e_CONTROL_BUTTON:i]);
	}

	SelectTextDraw(playerid, 0x818181DC);
}

static stock LightenColor(color, Float:amount)
{
	new a = (color >> 24) & 0xFF;
	new r = (color >> 16) & 0xFF;
	new g = (color >> 8) & 0xFF;
	new b = color & 0xFF;

	r = min(255, floatround(r + (255 - r) * amount));
	g = min(255, floatround(g + (255 - g) * amount));
	b = min(255, floatround(b + (255 - b) * amount));

	return (a << 24) | (r << 16) | (g << 8) | b;
}

/**
 * <summary>Checks and formats the given menu item text.</summary>
 * <remarks>
 * Checks if the given menu item is locked or scrollable
 * </remarks>
 * <param name="playerid">The target player</param>
 * <param name="item">The menu item text to check</param>
 */
static ItemPropertyCheck(playerid, const item[MAX_ITEM_LEN])
{
	new curentItem = interactionMenuItemCount[playerid];
	interactionMenuItems[playerid][curentItem] = item;

	if (strfind(interactionMenuItems[playerid][curentItem], "[") != -1 && 
		strfind(interactionMenuItems[playerid][curentItem], "]") != -1)
	{
		bIsItemScrollable[playerid][curentItem] = true;

		new index1,
			index2
		;

		index1 = strfind(interactionMenuItems[playerid][curentItem], "[");
		index2 = strfind(interactionMenuItems[playerid][curentItem], "]");

		new scrollItem[MAX_ITEM_LEN];
		strmid(scrollItem, interactionMenuItems[playerid][curentItem], index1, index2 + 1);
		strdel(interactionMenuItems[playerid][curentItem], index1, index2 + 1);
		strdel(scrollItem, 0, 1);
		strdel(scrollItem, strfind(scrollItem, "]"), strfind(scrollItem, "]") + 1);

		if (scrollItem[strlen(scrollItem) - 1] != ',') strcat(scrollItem, ",");

		for (new scroll; scroll < MAX_SCROLL_ITEMS; scroll++)
		{
			if (strfind(scrollItem, ",") == -1) break;
			interactionMenuScrollCount[playerid][curentItem] ++;
			strmid(interactionMenuScrollItems[playerid][curentItem][scroll], scrollItem, 0, strfind(scrollItem, ",", true));
			strdel(scrollItem, 0, strfind(scrollItem, ",", true) + 1);
			new idx = strfind(interactionMenuScrollItems[playerid][curentItem][scroll], " ");
			if (strfind(interactionMenuScrollItems[playerid][curentItem][scroll], " ") != -1) strdel(interactionMenuScrollItems[playerid][curentItem][scroll], idx, idx + 1);
		
		}
		
	}

	new len = strlen(interactionMenuItems[playerid][curentItem]);
	if (len > 1 &&
		interactionMenuItems[playerid][curentItem][0] == '#' &&
		interactionMenuItems[playerid][curentItem][len - 1] == '#')
	{
		strdel(interactionMenuItems[playerid][curentItem], len -1, len);
		strdel(interactionMenuItems[playerid][curentItem], 0, 1);
		strins(interactionMenuItems[playerid][curentItem], "[L] ", 0);
		bIsItemLocked[playerid][curentItem] = true;
	}
}

forward OnInteractionMenuResponse(playerid, menuid, bool:response, menuitem);
forward OnScrollItemChange(playerid, menuid, menuitem, scrollitem);

/**
 * <summary>Handles unfreezing the player after the last menu is closed.</summary>
 * <remarks>
 * This is used as a delay so that the player isn't frozen/unfrozen incorrectly 
 * when another menu is opened immediately after the previous one.
 * It ensures the player is only unfrozen if no new menu is currently being opened.
 * </remarks>
 * <param name="playerid">The player to unfreeze after menu closure.</param>
 */
forward DelayFromLastMenu(playerid);
public DelayFromLastMenu(playerid)
{
	#if defined _FOREACH_LOCAL_VERSION

		if (!Iter_Contains(itHasLastMenu, playerid)) return;
		Iter_Remove(itHasLastMenu, playerid);

	#else

		if (!bHasLastMenu[playerid]) return;
		bHasLastMenu[playerid] = false;
		
	#endif
	
	unfreezeTimer[playerid] = INVALID_TIMER;
	TogglePlayerControllable(playerid, true);
}

/**
 * <summary>Initializes and displays the interaction menu for a player.</summary>
 * <remarks>
 * This function sets up all menu textdraws, loads the items and descriptions, 
 * applies formatting and colors, and shows the menu to the player.  
 * It also ensures the player is frozen while the menu is open, and prevents conflicts 
 * if another menu is already active.  
 * 
 * Item descriptions default to "No description" if missing.
 * </remarks>
 * <param name="playerid">The player who will see the menu.</param>
 * <param name="menuid">The ID of the menu being opened for the player.</param>
 * <param name="tdMenuItems">The string for the menu items, separated by \n.</param>
 * <param name="title">The menu title to display.</param>
 * <param name="description">The description string for the menu items, separated by \n.</param>
 * <param name="color">The primary color used for the menu’s title and header.</param>
 */

forward DelayShowPlayerInteractionMenu(playerid, menuid, const tdMenuItems[], const title[], const description[], color);
public DelayShowPlayerInteractionMenu(playerid, menuid, const tdMenuItems[], const title[], const description[], color)
{
	
	InitMenuState(playerid, menuid);
	ApplyMenuStyling(playerid, title, color);

	UpdateMenuTextdraws(playerid);

	FormatMenuItems(playerid, tdMenuItems);
	FormatMenuDescriptions(playerid, description);
	
	if (interactionMenuItemCount[playerid] > MAX_MENU_ITEMS) interactionMenuItemCount[playerid] = MAX_MENU_ITEMS;

	for(new i; i < interactionMenuItemCount[playerid]; i++)
	{
		if(strlen(interactionMenuItems[playerid][i]) == 0)
		{
			new string[128];
			format(string, sizeof string, "%s(%i) : warning: Item %i in menuid %i is empty!", __file, __line, i, iMenuId[playerid]);
			print(string);
		}
	}

	UpdateMenuHighlight(playerid, interactionMenuItemCount[playerid]);
}

public OnPlayerConnect(playerid) 
{
	CreateIMenu(playerid);

	return CallLocalFunction("IM_OnPlayerConnect", "i", playerid);
}
forward IM_OnPlayerConnect(playerid);
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect IM_OnPlayerConnect

public OnPlayerDisconnect(playerid, reason)
{
	DestroyIMenu(playerid);
	#if defined _FOREACH_LOCAL_VERSION

		Iter_Remove(itIsInMenu, playerid);
		Iter_Remove(itHasLastMenu, playerid);

	#else

		bIsInMenu[playerid] = false;
		bHasLastMenu[playerid] = false;

	#endif
	iSelectedMenuItemId[playerid] = 0;
	iMenuScrollOffset[playerid] = 0;
	iMenuId[playerid] = -1;

	unfreezeTimer[playerid] = INVALID_TIMER;

	return CallLocalFunction("IM_OnPlayerDisconnect", "ii", playerid, reason);
}
forward IM_OnPlayerDisconnect(playerid, reason);
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect IM_OnPlayerDisconnect

/**
 * <summary>
 * Handles player input when inside the interaction menu.
 * </summary>
 *
 * <remarks>
 * While a menu is open, this hook handles navigation, selection, and scrolling
 * through menu items. It also rate-limits input so players cannot trigger
 * actions too quickly.
 *
 * Controls:
 * - KEY_UP / KEY_DOWN: Navigate through menu items.
 * - KEY_SPRINT: Confirm the currently selected item (if not locked).
 * - KEY_SECONDARY_ATTACK: Closes the open menu.
 * - KEY_LEFT / KEY_RIGHT: Cycle through scrollable item options.
 *
 * If the player is not inside a menu, this hook does not affect them.
 * </remarks>
 *
 * <param name="playerid">The ID of the player being updated.</param>
 * <returns>Always returns Y_HOOKS_CONTINUE_RETURN_1.</returns>
 */
public OnPlayerUpdate(playerid)
{   
	if (!IsPlayerInMenu(playerid)) return CallLocalFunction("IM_OnPlayerUpdate", "i", playerid);

	new KEY:keys, updown, leftright;
	GetPlayerKeys(playerid, keys, updown, leftright);

	if (!CanProcessMenuInput(playerid)) return CallLocalFunction("IM_OnPlayerUpdate", "i", playerid);

	if (actionCount[playerid] >= 2) return CallLocalFunction("IM_OnPlayerUpdate", "i", playerid);

	new index = iMenuScrollOffset[playerid] + iSelectedMenuItemId[playerid];

	HandleMenuNavigation(playerid, updown, index);
	HandleMenuSelection(playerid, keys, index);
	HandleMenuScrolling(playerid, leftright, index);

	return CallLocalFunction("IM_OnPlayerUpdate", "i", playerid);
}

forward IM_OnPlayerUpdate(playerid);
#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate IM_OnPlayerUpdate

/*
 * Hides the interaction menu when a player dies.
 */
public OnPlayerDeath(playerid, killerid, WEAPON:reason)
{
	HideInteractionMenu(playerid);

	return CallLocalFunction("IM_OnPlayerDeath", "iii", playerid, killerid, _:reason);
}

forward IM_OnPlayerDeath(playerid, killerid, WEAPON:reason);
#if defined _ALS_OnPlayerDeath
	#undef OnPlayerDeath
#else
	#define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath IM_OnPlayerDeath

/*
 * Handles android button controls
 */
 
public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
	new index = iMenuScrollOffset[playerid] + iSelectedMenuItemId[playerid];
	if (playertextid == tdAndroidText[playerid][CONTROL_BUTTON_UP])
	{
		if (iSelectedMenuItemId[playerid] > 0) 
		{
			iSelectedMenuItemId[playerid]--;
			PlayerPlaySound(playerid, 1084);
		}
		else if (iMenuScrollOffset[playerid] > 0) 
		{
			iMenuScrollOffset[playerid]--;
			PlayerPlaySound(playerid, 1084);
		}

		UpdateMenuHighlight(playerid, interactionMenuItemCount[playerid]);
		actionCount[playerid]++;
	}

	if (playertextid == tdAndroidText[playerid][CONTROL_BUTTON_DOWN])
	{
		if (iSelectedMenuItemId[playerid] < sizeof(tdMenuItemText[]) - 1 
		&& index < interactionMenuItemCount[playerid] - 1)
		{
			iSelectedMenuItemId[playerid]++;
			PlayerPlaySound(playerid, 1084);
		}
		else if (iMenuScrollOffset[playerid] + sizeof(tdMenuItemText[]) < interactionMenuItemCount[playerid])
		{
			iMenuScrollOffset[playerid]++;
			PlayerPlaySound(playerid, 1084);
		}

		UpdateMenuHighlight(playerid, interactionMenuItemCount[playerid]);
		actionCount[playerid]++;
	}


	if (playertextid == tdAndroidText[playerid][CONTROL_BUTTON_RIGHT])
	{
		interactionMenuScrollIndex[playerid][index]++;
		if (interactionMenuScrollIndex[playerid][index] >= interactionMenuScrollCount[playerid][index]) 
			interactionMenuScrollIndex[playerid][index] = 0;

		HandleScrollItem(playerid, iMenuId[playerid], index, interactionMenuScrollIndex[playerid][index]);
		UpdateMenuHighlight(playerid, interactionMenuItemCount[playerid]);
		actionCount[playerid]++;
	}

	if (playertextid == tdAndroidText[playerid][CONTROL_BUTTON_LEFT])
	{
		if (interactionMenuScrollIndex[playerid][index] == 0) 
			interactionMenuScrollIndex[playerid][index] = interactionMenuScrollCount[playerid][index] - 1;
		else 
			interactionMenuScrollIndex[playerid][index]--;

		HandleScrollItem(playerid, iMenuId[playerid], index, interactionMenuScrollIndex[playerid][index]);
		UpdateMenuHighlight(playerid, interactionMenuItemCount[playerid]);
		actionCount[playerid]++;
	}

	if (playertextid == tdAndroidText[playerid][CONTROL_BUTTON_SELECT])
	{
		if (!bIsItemLocked[playerid][index])
		{
			HandleTDMenuItem(playerid, iSelectedMenuItemId[playerid], iMenuId[playerid], true);
			HideInteractionMenu(playerid);
			CancelSelectTextDraw(playerid);
		}
		else PlayerPlaySound(playerid, 1085);
		actionCount[playerid]++;
	}

	if (playertextid == tdAndroidText[playerid][CONTROL_BUTTON_CLOSE])
	{
		HideInteractionMenu(playerid);
		HandleTDMenuItem(playerid, iSelectedMenuItemId[playerid], iMenuId[playerid], false);
		CancelSelectTextDraw(playerid);
		PlayerPlaySound(playerid, 1085);
		actionCount[playerid]++;
	}
	
	return CallLocalFunction("IM_OnPlayerClickPlayerTextDraw", "ii", playerid, playertextid);

}

forward IM_OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid);
#if defined _ALS_OnPlayerClickPlayerTextDraw
	#undef OnPlayerClickPlayerTextDraw
#else
	#define _ALS_OnPlayerClickPlayerTextDraw
#endif
#define OnPlayerClickPlayerTextDraw IM_OnPlayerClickPlayerTextDraw