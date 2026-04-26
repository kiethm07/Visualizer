#pragma once
#include <SFML/Graphics.hpp>
#include <Panel/GraphPanel.h>
#include <Panel/TimelinePanel.h>
#include <Panel/InitPanel.h>
#include <DataStructures/Graph.h>
#include <Model/GraphModel/GraphTimeline.h>
#include <Model/GraphModel/GraphRenderer.h>
#include <Model/Button.h>
#include <AssetManager/AssetManager.h>
#include <UI/Camera.h>
#include <DSUI/UIState.h>
#include <Model/GraphModel/GraphDictionary.h>
#include <Model/PseudoCodeController/PseudoCodeController.h>
#include <Model/MenuState.h>
#include <chrono>
#include <random>

class GraphUI
{
public:
    GraphUI(const AssetManager& a_manager);
    void update(const sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view);
    std::optional<MenuState> handleEvent(const sf::RenderWindow& window, const sf::View& view, sf::View& cam_view, CameraController& cam, const sf::Event& ev);
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
    GraphPanel panel;
    GraphState current_state;
    GraphTimeline timeline;
    Graph graph;
    GraphRecorder recorder;
    sf::Clock clock;
    TimelinePanel timeline_panel;
    UIState ui_state;
    InitPanel init_panel;
    PseudoCodeController <GraphOperationType, GraphDictionary> code_panel;
};