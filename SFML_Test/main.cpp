#include <iostream>
#include "SFML\Graphics.hpp"

using namespace std;

const sf::Int64 MIN_FRAMERATE = 15;
const sf::Int64 MAX_FRAMERATE = 120;
const sf::Int64 MAX_DELTATIME = 1000000 / MIN_FRAMERATE;
const sf::Int64 MIN_DELTATIME = 1000000 / MAX_FRAMERATE;

int main(int argc, char** argv)
{
	// Core
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML Test", sf::Style::None);
	sf::Event event;
	sf::Clock clock;

	float fps = 0.0f;
	sf::Int64 deltaTime = 0;

	// Text + Rendering
	sf::Font font;
	font.loadFromFile("..\\Resources\\Fonts\\Monospace.ttf");

	sf::Text fpsCounter;
	fpsCounter.setPosition(16.0f, 16.0f);
	fpsCounter.setFont(font);

	float joystickX = 0.0f;
	float joystickY = 0.0f;
	sf::RectangleShape joystickPositionIndicator(sf::Vector2f(5.0f, 5.0f));

	sf::Color active = sf::Color(0x00, 0xFF, 0x00, 0xFF);	// full green
	sf::Color inactive = sf::Color(0x00, 0x40, 0x00, 0xFF);	// dark green

	sf::CircleShape joystickButtonIndicatorLight(16.0f);
	
	sf::Text joystickButtonNumber;
	joystickButtonNumber.setFont(font);
	joystickButtonNumber.setFillColor(sf::Color::Black);

	int i = 0;
	int col = 0;
	int row = 0;

	while (window.isOpen())
	{
		// handle time too short (too many frames), last frame was too short, wait a bit
		// frame limiter
		if (deltaTime < MIN_DELTATIME)
		{
			sf::sleep(sf::microseconds(MIN_DELTATIME - deltaTime));
		}
		deltaTime = clock.restart().asMicroseconds();
		fps = 1000000.0f / static_cast<float>(deltaTime);						// 1 million microseconds per second
		// handle time too long (low framerate)
		deltaTime = (deltaTime > MAX_DELTATIME) ? MAX_DELTATIME : deltaTime;	// slow game down so it handles at least 15 frames, no matter what.
																				// this prevents collission detection failures between frames, etc
		// display the framerate
		fpsCounter.setFillColor(fps < 30.0f ? sf::Color::Red : sf::Color::Green);
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::Resized:
				cout << window.getSize().x << "x" << window.getSize().y << endl;
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					window.close();
					break;
				default:
					break;
				}
				break;
			case sf::Event::JoystickMoved:
				if (event.joystickMove.axis == sf::Joystick::Axis::X)
				{
					joystickX = event.joystickMove.position;
				}
				else if (event.joystickMove.axis == sf::Joystick::Axis::Y)
				{
					joystickY = event.joystickMove.position;
				}
				// cout << "Joy[" << event.joystickMove.joystickId << "] Axis " << event.joystickMove.axis << " - " << event.joystickMove.position << endl;
				break;
			default:
				break;
			}
		}
		// logic
		row = 0;
		fpsCounter.setString(sf::String("FPS: " + to_string(fps)));
		joystickPositionIndicator.setPosition(
			((joystickX / 200.0f)*window.getSize().x) + window.getSize().x/2.0f - joystickPositionIndicator.getSize().x/2.0f,
			((joystickY / 200.0f)*window.getSize().y) + window.getSize().y/2.0f - joystickPositionIndicator.getSize().y/2.0f
		);
		// clear
		window.clear();
		// draw
		for (i = 0; i < sf::Joystick::getButtonCount(0); i++)
		{
			if (i % 8 == 0)
			{
				col = 0;
				row++;
			}
			joystickButtonIndicatorLight.setPosition(16.0f + (joystickButtonIndicatorLight.getRadius() * 2 + 8.0f) * col, 32.0f + (joystickButtonIndicatorLight.getRadius() * 2 + 8.0f) * row);
			joystickButtonIndicatorLight.setFillColor(sf::Joystick::isButtonPressed(0, i) ? active : inactive);
			window.draw(joystickButtonIndicatorLight);
			joystickButtonNumber.setString(to_string(i));
			joystickButtonNumber.setPosition(16.0f + (joystickButtonIndicatorLight.getRadius() * 2 + 8.0f)*col, 32.0f + (joystickButtonIndicatorLight.getRadius() * 2 + 8.0f) * row);
			window.draw(joystickButtonNumber);
			col++;
		}
		window.draw(joystickPositionIndicator);
		window.draw(fpsCounter);
		// framebuffer switch
		window.display();
	}
	return 0;
}