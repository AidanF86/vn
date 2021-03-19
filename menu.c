void initializeMenus()
{
    menuStructs[0] = NULL;
    menuStructs[1] = &titleMenuData;
    menuStructs[2] = &pauseMenuData;
    menuStructs[3] = &saveMenuData;
    menuStructs[4] = &loadMenuData;
    menuStructs[5] = &settingsMenuData;
}

void openMenu(enum menus menuToOpen)
{
    currentMenu = menuToOpen;
    menuChanged = true;
}

void closeMenu()
{
    currentMenu = noMenu;
}
