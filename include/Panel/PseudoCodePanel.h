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
    //void setSize(const sf::Vector2f& size);
	void setBackgroundColor(const sf::Color& color);
	void setTextColor(const sf::Color& color);
    void animateHighlight(float progress, bool isHighlight, int index);
    void handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);
	void update(const sf::RenderWindow& window, const sf::View& view);
	bool handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev, const sf::Vector2f& mouse_pos); //0 is minimize, 1 is maximize
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void updateButtonPosition(); //Update the button position based on the code lines
    const AssetManager& asset_manager;
    std::vector<std::string> lines;
    std::vector<CodeLine> code_lines;
    int current_highlight;
    bool minimized;

    sf::RectangleShape background;
    Button title; 
    //Use Button class but not actually a button, just display
    //Non clickable and non hoverable
    Button minimize_button;
    Button maximize_button;
 
    sf::Vector2f position;
    sf::Vector2f size;

    const int PADDING = 0;
    const int LINE_HEIGHT = 25;
    const int CHAR_SIZE = 20;
    const sf::Vector2f BUTTON_SIZE = { 30,30 };

	const sf::Color DEFAULT_BG_COLOR = sf::Color(100, 100, 100);
	const sf::Color DEFAULT_TEXT_COLOR = sf::Color(200, 200, 200);
	const sf::Color HIGHLIGHT_TEXT_COLOR = sf::Color(0, 0, 0);
};