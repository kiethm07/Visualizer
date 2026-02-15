#include <SFML/Graphics.hpp>
#include <iostream>
#include <UI/UI.h>
#include <AssetManager/AssetManager.h>

int main() {
    AssetManager asset_manager;
    asset_manager.loadAllFontFromFile();
    UI ui(asset_manager);
    //asset_manager.printAllFont();
    ui.run();
}
