#include <SFML/Graphics.hpp>
#include <iostream>
#include <UI/UI.h>
#include <AssetManager/AssetManager.h>

int main() {
    AssetManager asset_manager;
    asset_manager.loadAllFontsFromFile();
    asset_manager.loadAllTexturesFromFile();
    asset_manager.loadAllSoundsFromFile();
    UI ui(asset_manager);
    //asset_manager.printAllFont();
    ui.run();
}
