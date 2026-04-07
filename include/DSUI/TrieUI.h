#pragma once
#include <SFML/Graphics.hpp>
#include <Panel/TriePanel.h>
#include <Panel/TimelinePanel.h>
#include <Panel/InitPanel.h>
#include <DataStructures/Trie.h>
#include <Model/TrieModel/TrieTimeline.h>
#include <Model/TrieModel/TrieRenderer.h>
#include <Model/Button.h>
#include <AssetManager/AssetManager.h>
#include <UI/Camera.h>
#include <DSUI/UIState.h>
#include <chrono>
class TrieUI
{
public:
    TrieUI(const AssetManager& a_manager);
    void update(const sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view);
    void handleEvent(const sf::RenderWindow& window, const sf::View& view, sf::View& cam_view, CameraController& cam, const sf::Event& ev);
    void draw(sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view);
    void setInit()
    {
        ui_state = UIState::Init;
        init_panel.clearValueForManualInput();
    }
private:
    void Init(const sf::RenderWindow& window, const sf::View& view, sf::View& cam_view, CameraController& cam, const PanelData& data);
    bool inAnimation = 0;
    const AssetManager& a_manager;
    const sf::Font& NODE_FONT;
    TriePanel panel;
    TrieState current_state;
    TrieTimeline timeline;
    Trie trie;
    TrieRecorder recorder;
    sf::Clock clock;
    TimelinePanel timeline_panel;
    UIState ui_state;
    InitPanel init_panel;
};