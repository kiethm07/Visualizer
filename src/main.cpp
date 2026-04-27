#include <SFML/Graphics.hpp>
#include <iostream>
#include <UI/UI.h>
#include <AssetManager/AssetManager.h>
#ifdef _WIN32
#include <windows.h>
#include <objbase.h>
#endif

int main() {
#ifdef _WIN32
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
#endif

    AssetManager asset_manager;
    asset_manager.loadAllFontsFromFile();
    asset_manager.loadAllTexturesFromFile();
    asset_manager.loadAllSoundsFromFile();
    
    SoundController sound_controller(asset_manager);
    Setting::setSoundController(&sound_controller);
    Button::setSoundController(&sound_controller);
    TextureButton::setSoundController(&sound_controller);

    UI ui(asset_manager);
    //asset_manager.printAllFont();
    ui.run();

#ifdef _WIN32
    if (SUCCEEDED(hr)) {
        CoUninitialize();
    }
#endif

    return 0;
}
