# SA-MP Interaction Menu Documentation

## Table of Contents

- [SA-MP Interaction Menu Documentation](#sa-mp-interaction-menu-documentation)
  - [Table of Contents](#table-of-contents)
  - [Installation](#installation)
  - [Defines](#defines)
  - [Function Reference](#function-reference)
    - [⚙️ Functions](#️-functions)
      - [`ShowPlayerInteractionMenu`](#showplayerinteractionmenu)
        - [Example usage of ShowPlayerInteractionMenu](#example-usage-of-showplayerinteractionmenu)
      - [`AddInteractionMenuItem`](#addinteractionmenuitem)
        - [Example usage of AddInteractionMenuItem](#example-usage-of-addinteractionmenuitem)
      - [`AddInteractionMenuItemEx`](#addinteractionmenuitemex)
        - [Example usage of AddInteractionMenuItemEx](#example-usage-of-addinteractionmenuitemex)
      - [`RemoveInteractionMenuItem`](#removeinteractionmenuitem)
        - [Example usage of RemoveInteractionMenuItem](#example-usage-of-removeinteractionmenuitem)
      - [`GetMenuItemString`](#getmenuitemstring)
        - [Example usage of GetMenuItemString](#example-usage-of-getmenuitemstring)
      - [`GetMenuScrollItemString`](#getmenuscrollitemstring)
        - [Example usage of GetMenuScrollItemString](#example-usage-of-getmenuscrollitemstring)
    - [🔄 CallBacks](#-callbacks)
      - [`OnInteractionMenuResponse`](#oninteractionmenuresponse)
        - [Example usage of OnInteractionMenuResponse](#example-usage-of-oninteractionmenuresponse)
      - [`OnScrollItemChange`](#onscrollitemchange)
        - [Example usage of OnScrollItemChange](#example-usage-of-onscrollitemchange)
      - [`OnInteractionMenuToggle`](#oninteractionmenutoggle)
        - [Example usage of OnInteractionMenuToggle](#example-usage-of-oninteractionmenutoggle)

---

## Installation

1. Include the script in your SA-MP gamemode:  
   ```#include <interaction_menu>```

## Defines
>[!IMPORTANT]
>| Description                                                          | Default Value | Redefinable |
>|----------------------------------------------------------------------|---------------|-------------|
>| Maximum total number of menu items (`MAX_MENU_ITEMS`)                | 35            | Yes         |
>| Maximum item length (`MAX_ITEM_LEN`)                                 | 256           | Yes         |
>| Maximum description length (`MAX_DESCRIPTION_LEN`)                   | 70            | Yes         |
>| Maximum number of scrollable values (`MAX_SCROLL_ITEMS`)             | 15            | Yes         |
>| Maximum scrollable value length (`MAX_SCROLL_LEN`)                   | 10            | Yes         |
>| Default box transparency color (`DEFAULT_BOX_ALPHA`)                 | 0xAA          | Yes         |
>| Maximum number of visible menu items (`MAX_VISIBLE_ITEMS`)           | 7             | Yes         |
>| Y position of the first menu item (`MENU_ITEM_START_Y`)              | 187.0         | Yes         |
>| Vertical spacing between menu items (`MENU_ITEM_SPACING`)            | 15.0          | Yes         |
>| Y offset of item text relative to item box (`MENU_ITEM_TEXT_OFFSET`) | 1.0           | Yes         |
>| Resize menu to fit item count (`MENU_DYNAMIC_SIZE`)                  | true          | Yes         |

> [!NOTE]
>When `MENU_DYNAMIC_SIZE` is set to `true`, the menu will automatically resize itself to fit the number of items instead of always using `MAX_VISIBLE_ITEMS` as the menu height.
>When enabled:
>- The menu height adjusts every time items are added, removed or the menu is opened.
>- The footer separator, arrow and description text are repositioned to sit right below the last item.
>- The menu will never grow beyond `MAX_VISIBLE_ITEMS`, items beyond that require scrolling as usual.

 ---
> [!CAUTION]
> `MENU_DYNAMIC_SIZE` requires `PlayerTextDrawSetPos`, which is only available on **open.mp**. It will not work on standard SA-MP.

---

> [!WARNING]
> For now, all defined values can be redefined, but the more you increase them, the more memory they will require  
> When increasing these values (and sometimes by default), you will be required to increase the heap/stack size accordingly, after including all your libraries:
>
> ```pawn
> // Fix for " Run time error 3: "Stack/heap collision (insufficient stack size)"
> // Serves as a maximum memory size for variables and arrays (ex: new string[8192])
> #pragma dynamic 4096*2
> #define CGEN_MEMORY (60000) // Fix for CGen space error
> ```

## Function Reference

### ⚙️ Functions

#### `ShowPlayerInteractionMenu`

```pawn
ShowPlayerInteractionMenu(playerid, menuid, tdMenuItems, title, description, color)
```

> Shows the player a menu.
>
> - **playerid**: The player to show the menu.
> - **menuid**:  The menu id to later use in `OnInteractionMenuResponse`, `OnScrollItemChange`, or `OnInteractionMenuToggle`.
> - **tdMenuItems**:  The whole items string, separated by `\n`.
> - **title**: The title of the menu. *(optional, defaults to no title)*
> - **description**: The whole description string, separated by `\n`. *(optional, defaults to "No Description")*
> - **color**: The color of the menu header. *(optional, defaults to COLOR_MENU_TITLE_BOX)*

##### Example usage of ShowPlayerInteractionMenu

```pawn
ShowPlayerInteractionMenu(playerid, EXAMPLE_MENU_ID, 
     "Item 1\n#Locked item#\nScroll item[Red, Green, Blue]\n|Toggle Off|\n|+Toggle On|\n#|Locked Toggle|#", "Example_Menu_Title", 
     "1st Item description\nLocked item description\nScroll item description\nToggle off description\nToggle on description\nLocked toggle description", 
     0xFFFFFF62);
```

![ShowPlayerInteractionMenu](./resources/menu_1.png "ShowPlayerInteractionMenu") Example image of ShowPlayerInteractionMenu

> [!NOTE]
> Remarks about ShowPlayerInteractionMenu
>
> - Each item is separated by **\n**, the same for descriptions
>
> `Item1\nItem2\nItem3\n`
>
> - To lock items wrap the item in **#**
>
> `#Locked Item#`
>
> - Put items inside **square brackets** to make them scroll items
>
> `Colors[Red, Green, Blue]`
>
> - Wrap items in **|** to make them toggle items (checkbox). Toggles off by default
>
> `|Toggle Item|`
>
> - Add **+** after the opening **|** to default the toggle to on
>
> `|+Toggle Item|`
>
> - Locked and toggle can be combined by wrapping in both **#** and **|**
>
> `#|Locked Toggle|#`

 ---
> [!WARNING]
>
> - The title must have **_** instead of spaces.
>
>`Example_Menu_Title`
>
> - Toggle and scroll items are **cannot be combined**. If both syntaxes are used on the same item, toggle takes priority and scroll is ignored.
>
 ---

#### `AddInteractionMenuItem`

```pawn
AddInteractionMenuItem(playerid, item, desc)
```

> Adds a new item to the player's open menu.
>
> - **playerid**: The player to add a new item.
> - **item**: The item string to add.
> - **desc**: The description for the item *(optional, defaults to "No Description")*

##### Example usage of AddInteractionMenuItem

```pawn
AddInteractionMenuItem(playerid, "New item", "New item description!");
```

![AddInteractionMenuItem](./resources/menu_2.png "AddInteractionMenuItem") Example image of AddInteractionMenuItem

> [!NOTE]
> Remarks about AddInteractionMenuItem
>
> - Added items can also be locked, scroll, or toggle items.
>
> - Added items will always be at the bottom of the item list.

 ---

#### `AddInteractionMenuItemEx`

```pawn
AddInteractionMenuItemEx(playerid, item, boxcolor, desc)
```
>
> Adds a new item to the player's open menu with a custom box color.
>
> - **playerid**: The player to add a new item.
> - **item**: The item string to add.
> - **boxcolor**: The custom box color to be used by the item. *(optional, defaults to COLOR_MENU_ITEM_BOX)*
> - **desc**: The description for the item *(optional, defaults to "No Description")*

 ---

##### Example usage of AddInteractionMenuItemEx

```pawn
AddInteractionMenuItemEx(playerid, "New colored item", 0xFF000060, "New item description!");
```

![AddInteractionMenuItemEx](./resources/menu_3.png "AddInteractionMenuItemEx") Example image of AddInteractionMenuItemEx

> [!NOTE]
> Remarks about AddInteractionMenuItemEx
>
> - Added items can also be locked, scroll, or toggle items.
>
> - Added items will always be at the bottom of the item list.
>
> - The box color of this item will be red

 ---
> [!WARNING]
>
> - When adding a colored item, it's preffered to use a
> semi-transparent color.
>
 ---

#### `RemoveInteractionMenuItem`

```pawn
RemoveInteractionMenuItem(playerid, itemid)
```

> Removes a item from the player's open menu.
>
> - **playerid**: The player to remove the item.
> - **itemid**: The item index in the menu.

##### Example usage of RemoveInteractionMenuItem

```pawn
RemoveInteractionMenuItem(playerid, 3);
```

> [!NOTE]
> Remarks about RemoveInteractionMenuItem
>
> - This will remove the 4th item in the menu.

---

#### `GetMenuItemString`

```pawn
GetMenuItemString(output, playerid, imenuitem)
```

> Formats a given string with the selected item's text.
>
> - **output**: The given string to format.
> - **playerid**: The player with the menu open.
> - **itemid**: The item index in the menu.

##### Example usage of GetMenuItemString

```pawn
new string[128];
GetMenuItemString(string, playerid, 0);
print(string);
```

> [!NOTE]
> Remarks about GetMenuItemString
>
> - This will format `string` with the 1st item text.
>
> - This would print the 1st item of [ShowPlayerInteractionMenu](#showplayerinteractionmenu), which is "Item 1"

---

#### `GetMenuScrollItemString`  

```pawn
GetMenuScrollItemString(output, playerid, imenuitem, scrollitem)
```

> Formats a given string with the selected scroll item's text.
>
> - **output**: The given string to format.
> - **playerid**: The player with the menu open.
> - **itemid**: The item index in the menu.
> - **scrollitem**: The scroll item index inside **itemid**.

##### Example usage of GetMenuScrollItemString

```pawn
new string[128];
GetMenuScrollItemString(string, playerid, 2, 0);
print(string);
```

> [!NOTE]
> Remarks about GetMenuScrollItemString
>
> - This will format `string` with the 3rd scroll option, inside the 1st menu item.
> - This would print the 1st scroll option inside the 3rd item in [ShowPlayerInteractionMenu](#showplayerinteractionmenu), which is "red"

### 🔄 CallBacks

#### `OnInteractionMenuResponse`

```pawn
OnInteractionMenuResponse(playerid, menuid, bool:response, menuitem)
```

>Called when a player clicks on an item or closes the menu.
>
> - **playerid**: The player who triggered the callback.
> - **menuid**: The id of the player's menu.
> - **response**: Weather or not an item was clicked.
> - **menuitem**: The item index the player clicked. *(-1 if response is `false`)*

##### Example usage of OnInteractionMenuResponse

```pawn
public OnInteractionMenuResponse(playerid, menuid, bool:response, menuitem)
{
    if (response)
    {
        if (menuid == EXAMPLE_MENU_ID)
        {
            switch (menuitem)
            {
                case 0: SendClientMessage(playerid, 0x00FF00, "You picked the first item!");
                case 1: SendClientMessage(playerid, 0x00FF00, "You picked the second item!");
                case 2: SendClientMessage(playerid, 0x00FF00, "You picked the third item!");
            }
        }
    }
    else return SendClientMessage(playerid, 0xFF0000, "You closed the menu!");

    return 1;
}
```

![OnInteractionMenuResponse](./resources/menu_5.gif "OnInteractionMenuResponse")

> [!NOTE]
> Remarks about `OnInteractionMenuResponse`
>
> - This checks if the player clicked an item inside menuid `EXAMPLE_MENU_ID` and sends a message accordingly.
> - Also sends a message if the player closes the menu.
> - Toggle items **do not** trigger [`OnInteractionMenuResponse`](#oninteractionmenuresponse). Use [`OnInteractionMenuToggle`](#oninteractionmenutoggle) instead.

 ---

#### `OnScrollItemChange`

```pawn
OnScrollItemChange(playerid, menuid, menuitem, scrollitem)
```

>Called when the player changes a scroll options inside a menu.
>
> - **playerid**: The player who triggered the callback.
> - **menuid**: The id of the player's menu.
> - **menuitem**: The item index the player scrolled in.
> - **scrollitem**: The scroll option the player scrolled to, inside `menuitem`

##### Example usage of OnScrollItemChange

```pawn
public OnScrollItemChange(playerid, menuid, menuitem, scrollitem)
{
    if (menuid == EXAMPLE_MENU_ID)
    {
        if (menuitem == 2)
        {
            switch (scrollitem)
            {
                case 0: SendClientMessage(playerid, 0xFF0000FF, "You picked the red color!");
                case 1: SendClientMessage(playerid, 0x00FF00FF, "You picked the green color!");
                case 2: SendClientMessage(playerid, 0x00B7FFFF, "You picked the blue color!");
            }
        }
    }

    return 1;
}
```

![OnScrllItemChange](./resources/menu_4.gif "OnScrollItemChange")

> [!NOTE]
> Remarks about OnScrollItemChange
>
> - This checks if the player scrolled on the 3rd item of EXAMPLE_MENU_ID menu.

 ---

#### `OnInteractionMenuToggle`

```pawn
OnInteractionMenuToggle(playerid, menuid, itemid, bool:toggled)
```

> Called when a player toggles a toggle item inside a menu.
>
> - **playerid**: The player who triggered the callback.
> - **menuid**: The id of the player's menu.
> - **itemid**: The item index the player toggled.
> - **toggled**: The new state of the toggle. `true` if now on, `false` if now off.

##### Example usage of OnInteractionMenuToggle

```pawn
public OnInteractionMenuToggle(playerid, menuid, itemid, bool:toggled)
{
    if (menuid == EXAMPLE_MENU_ID)
    {
        if (itemid == 3)
        {
            if (toggled) SendClientMessage(playerid, 0x00FF00FF, "Toggle is now ON!");
            else SendClientMessage(playerid, 0xFF0000FF, "Toggle is now OFF!");
        }
    }

    return 1;
}
```

> [!NOTE]
> Remarks about `OnInteractionMenuToggle`
>
> - Locked toggle items **do not** fire this callback.

 ---
