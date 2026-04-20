#pragma once
#include <vector>
#include <string>
#include <Model/Button.h>
#include <Model/CodeLine.h>
#include <AssetManager/AssetManager.h>

class PseudoCodePanel : public sf::Drawable {
public:
    PseudoCodePanel(const AssetManager& asset_manager, const std::string& font_name = "Roboto-Regular");

    void setCode(const std::vector<std::string>& code_lines);
    void setHighlight(int index);
    void setPosition(const sf::Vector2f& pos);
    void setSize(const sf::Vector2f& size);
	void setBackgroundColor(const sf::Color& color);
	void setTextColor(const sf::Color& color);
    void animateHighlight(float progress, bool isHighlight, int index);
    void handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);
	void update(const sf::RenderWindow& window, const sf::View& view);
	bool handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev, const sf::Vector2f& mouse_pos); //0 is minimize, 1 is maximize
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    const AssetManager& asset_manager;
    std::vector<std::string> lines;
    std::vector<CodeLine> code_lines;
    int current_highlight;
    bool minimized;

    sf::RectangleShape background;
    Button minimize_button;
    Button maximize_button;
 
    sf::Vector2f position;
    sf::Vector2f size;

    const int PADDING = 0;
    const int LINE_HEIGHT = 10;
    const int BUTTON_SIZE = 30;
    const int CHAR_SIZE = 20;
    
	const sf::Color DEFAULT_BG_COLOR = sf::Color(255, 255, 255);
	const sf::Color DEFAULT_TEXT_COLOR = sf::Color(0, 0, 0);
	const sf::Color HIGHLIGHT_TEXT_COLOR = sf::Color(0, 0, 0);
};