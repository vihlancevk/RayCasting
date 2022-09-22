#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "../Vectors/CoordinateSystem.hpp"
#include "../Vectors/Vector.hpp"

const unsigned SCREEN_WEIGHT = 1920;
const unsigned SCREEN_HIGHT = 1080;
const char *SCREEN_TITLE = "RayCasting";
const unsigned FRAME_RATE_LIMIT = 60;

int main()
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_WEIGHT, SCREEN_HIGHT), SCREEN_TITLE);
	window.setFramerateLimit(FRAME_RATE_LIMIT);

	sf::Event event;

    CoordinateSystem coordinateSystem = CoordinateSystem(800, 800, 400,
                                                         550, 100, 0,
                                                         -10, 10, -10, 10, 0, 10);
    size_t coordinateSystemCentreX = (size_t) (coordinateSystem.xLeftUp_ + coordinateSystem.weight_ / 2);
    size_t coordinateSystemCentreY = (size_t) (coordinateSystem.yLeftUp_ + coordinateSystem.hight_  / 2);

    float fxMin = 0, fyMax = 0;
    coordinateSystem.ConvertLocalToGlobalVectorCoordinate(coordinateSystem.xMin_, coordinateSystem.yMax_,
                                                          fxMin, fyMax);
    size_t xMin = (size_t) fxMin, yMin = (size_t) fyMax;

    float fxMax = 0, fyMin = 0;
    coordinateSystem.ConvertLocalToGlobalVectorCoordinate(coordinateSystem.xMax_, coordinateSystem.yMin_,
                                                          fxMax, fyMin);
    size_t xMax = (size_t) fxMax, yMax = (size_t) fyMin;

    Vector radius = Vector(5, 0, 0);

    float fradiusX = 0, fradiusY = 0;
    coordinateSystem.ConvertLocalToGlobalVectorCoordinate(radius.x_, radius.y_,
                                                          fradiusX, fradiusY);
    size_t radiusX = (size_t) fradiusX, radiusY = (size_t) fradiusY;

    size_t radiusLen2 = (radiusX - coordinateSystemCentreX) * (radiusX - coordinateSystemCentreX) +
                        (radiusY - coordinateSystemCentreY) * (radiusY - coordinateSystemCentreY);

    // std::cout << xMin << " " << xMax << " " << yMin << " " << yMax << "\n" << radiusLen2 << "\n";

    Vector light = Vector(1000, 1000, 1000);

    sf::VertexArray pixels(sf::Points, (size_t) coordinateSystem.weight_ * (size_t) coordinateSystem.hight_);

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
                window.close();

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
                    window.close();

                if (event.key.code == sf::Keyboard::W)
                    continue;

                if (event.key.code == sf::Keyboard::S)
                    continue;
			}

            if (event.type == sf::Event::MouseButtonPressed) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

                    float A = 0.1f;
                    float D = 0.9f;

                    for (size_t i = yMin; i < yMax; i++) {
                        for (size_t j = xMin; j < xMax; j++) {
                            size_t pointLen2 = (i - coordinateSystemCentreY) * (i - coordinateSystemCentreY) +
                                               (j - coordinateSystemCentreX) * (j - coordinateSystemCentreX);
                            pixels[(i - yMin) * (size_t) coordinateSystem.weight_ + (j - xMin)].position =
                                sf::Vector2f((float) j, (float) i);
                            if (pointLen2 >= radiusLen2) {
                                pixels[(i - yMin) * (size_t) coordinateSystem.weight_ + (j - xMin)].color = sf::Color::Black; 
                            } else {
                                size_t z = sqrt(radiusLen2 - (j - coordinateSystemCentreX)*(j - coordinateSystemCentreX)
                                - (i - coordinateSystemCentreY)*(i - coordinateSystemCentreY));
                                Vector point = Vector(j, i, z);
                                Vector pointToLight = light - point;
                                float cosF = (point * pointToLight) / (sqrt(point.len2_) * sqrt(pointToLight.len2_));
                                if (cosF < 0) { cosF = 0; }
                                // std::cout << point.x_ << " " << point.y_ << " " << point.z_ << point.len2_ << "\n";
                                // std::cout << pointToLight.x_ << " " << pointToLight.y_ << " " << pointToLight.z_ << pointToLight.len2_ << "\n";
                                // std::cout << (point * pointToLight) << " " << sqrt(point.len2_) << " " << sqrt(pointToLight.len2_) << " " << cosF << "\n";
                                sf::Color color((A + D * cosF) * 255, (A + D * cosF) * 255, (A + D * cosF) * 255);
                                pixels[(i - yMin) * (size_t) coordinateSystem.weight_ + (j - xMin)].color = color;
                            }
                        }
                    }

                }
            }
		}

        // std::cout << pixels[0].position.x << " " << pixels[0].position.y << "\n";

        window.draw(pixels);
        window.display();
		window.clear();
	}

	return 0;
}
