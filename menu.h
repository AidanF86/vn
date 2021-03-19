bool menuChanged;
#define MENUCOUNT 6
enum menus{noMenu, titleMenu, pauseMenu, saveMenu, loadMenu, settingsMenu};
enum originX{left, right};
enum originY{top, bottom};
enum menus currentMenu;

SDL_Texture* menuBackgrounds[6];
SDL_Texture* textBackground;

void initalizeMenus();
void openMenu(enum menus);
void closeMenu();

struct menu
{
    int columns;
    int rows;
    int pages;
} titleMenuData, pauseMenuData, saveMenuData, loadMenuData, settingsMenuData;

struct menu* menuStructs[MENUCOUNT];
