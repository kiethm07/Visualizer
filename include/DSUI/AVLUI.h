#pragma once
#include <SFML/Graphics.hpp>
#include <Panel/AVLPanel.h>
#include <Panel/TimelinePanel.h>
#include <Panel/InitPanel.h>
#include <DataStructures/AVL.h>
#include <Model/AVLModel/AVLTimeline.h>
#include <Model/AVLModel/AVLRenderer.h>
#include <Model/Button.h>
#include <AssetManager/AssetManager.h>
#include <UI/Camera.h>
#include <DSUI/UIState.h>
#include <Model/AVLModel/AVLDictionary.h>
#include <Model/PseudoCodeController/PseudoCodeController.h>
#include <chrono>
#include <random>

class AVLUI
{
public:
    AVLUI(const AssetManager& a_manager);
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
    AVLPanel panel;
    AVLState current_state;
    AVLTimeline timeline;
    AVL avl;
    AVLRecorder recorder;
    sf::Clock clock;
    TimelinePanel timeline_panel;
    UIState ui_state;
    InitPanel init_panel;
    PseudoCodeController <AVLOperationType, AVLDictionary> code_panel;
};