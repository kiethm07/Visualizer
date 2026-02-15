#include <SFML/Graphics.hpp>
#include <iostream>
#include <UI/UI.h>
#include <AssetManager/AssetManager.h>

int main() {
    AssetManager asset_manager;
    UI ui;
    asset_manager.loadAllFontFromFile();
    //asset_manager.printAllFont();
    ui.run();
}
