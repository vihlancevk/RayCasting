#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "../Vectors/CoordinateSystem.hpp"
#include "../Vectors/Vector.hpp"

const unsigned SCREEN_WEIGHT = 1920;
const unsigned SCREEN_HIGHT = 1080;
const char *SCREEN_TITLE = "RayCasting";
const unsigned FRAME_RATE_LIMIT = 60;

const float PI = 3.14159f;

void RayCasting(sf::VertexArray& pixels, CoordinateSystem& coordinateSystem, Vector& sphere, Vector& light);

int main()
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_WEIGHT, SCREEN_HIGHT), SCREEN_TITLE);
	window.setFramerateLimit(FRAME_RATE_LIMIT);

	sf::Event event;

    CoordinateSystem coordinateSystem = CoordinateSystem(800, 800,
                                                         550, 100,
                                                         -15, 15, -15, 15);
    Vector sphere = Vector(10, 0, 0);
    Vector light  = Vector(7, 8, 30);

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
                    continue;
                }
            }
		}

        RayCasting(pixels, coordinateSystem, sphere, light);

        window.draw(pixels);
        window.display();
		window.clear();
	}

	return 0;
}

void RayCasting(sf::VertexArray& pixels, CoordinateSystem& coordinateSystem, Vector& sphere, Vector& light) {
    light.RotateVector(PI / 400);

    float A = 0.2f;
    float D = 0.6f;
    float S = 0.2f;
    int n = 18;

    float coordinatesInPixelX = (coordinateSystem.xMax_ - coordinateSystem.xMin_) / coordinateSystem.weight_;
    float coordinatesInPixelY = (coordinateSystem.yMax_ - coordinateSystem.yMin_) / coordinateSystem.hight_;

    for (size_t i = 0; i < (size_t) coordinateSystem.hight_; i ++) {
        for (size_t j = 0; j < (size_t) coordinateSystem.weight_; j ++) {

            float xLocal = coordinateSystem.xMin_ + (float) j * coordinatesInPixelX;
            float yLocal = coordinateSystem.yMin_ + (float) i * coordinatesInPixelY;

            float xGlobal = 0, yGlobal = 0;
            coordinateSystem.ConvertLocalToGlobalVectorCoordinate(xLocal, yLocal, xGlobal, yGlobal);
            
            pixels[i * (size_t) coordinateSystem.weight_ + j].position = sf::Vector2f(xGlobal, yGlobal);
            
            if (xLocal*xLocal + yLocal*yLocal >= sphere.len2_) {
                pixels[i * (size_t) coordinateSystem.weight_ + j].color = sf::Color::Black; 
            } else {
                Vector point = Vector(xLocal, yLocal, sqrtf(sphere.len2_ - xLocal*xLocal - yLocal*yLocal));
                Vector pointToLight = light - point;

                float cosF = (point * pointToLight) / (sqrtf(point.len2_) * sqrtf(pointToLight.len2_));
                float cosA = 0;

                if (cosF > 0) {
                    Vector lightAfterReflection = light;
                    lightAfterReflection.RotateVector(2 * acosf(cosF));

                    Vector normalToLight = light;
                    normalToLight.RotateVector(PI / 2);

                    cosA = (lightAfterReflection * normalToLight) /
                           (sqrtf(lightAfterReflection.len2_) * sqrtf(normalToLight.len2_));
                } else {
                    cosF = 0;
                }

                sf::Color color((sf::Uint8) ((A + D * cosF + S * pow(cosA, n)) * 255),
                                (sf::Uint8) ((A + D * cosF + S * pow(cosA, n)) * 255),
                                (sf::Uint8) ((A + D * cosF + S * pow(cosA, n)) * 255));
                pixels[i * (size_t) coordinateSystem.weight_ + j].color = color;
            }
        }
    }
}
