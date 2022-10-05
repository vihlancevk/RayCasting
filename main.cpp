#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "../MathVector/CoordinateSystem.hpp"
#include "../MathVector/MathVector.hpp"

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
                                                         -20, 20, -20, 20);
    Vector sphere = Vector(20, 0, 0);
    Vector light  = Vector(7, 8, 70);

    sf::VertexArray pixels(sf::Points, (size_t) SCREEN_WEIGHT * (size_t) SCREEN_HIGHT);

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
    light.rotateVector(PI / 400);

    float A = 0.1f;
    float D = 0.5f;
    float S = 0.4f;
    int n = 18;

    float PixelsInCoordinateX = coordinateSystem.weight_/ (coordinateSystem.xMax_ - coordinateSystem.xMin_);
    float PixelsInCoordinateY = coordinateSystem.hight_ / (coordinateSystem.yMax_ - coordinateSystem.yMin_);

    float radiusSphere = powf(sphere.getVectorLen(), 2);

    for (size_t i = 0; i < (size_t) SCREEN_HIGHT; i ++) {
        for (size_t j = 0; j < (size_t) SCREEN_WEIGHT; j ++) {

            float xLocal = ((float) j - (coordinateSystem.xLeftUp_ + coordinateSystem.weight_ / 2)) / PixelsInCoordinateX;
            float yLocal = ((float) i - (coordinateSystem.yLeftUp_ + coordinateSystem.hight_  / 2)) / PixelsInCoordinateY;
            float zLocal2 = 0;
            
            pixels[i * (size_t) SCREEN_WEIGHT + j].position = sf::Vector2f((float) j, (float) i);
            
            zLocal2 = radiusSphere - powf(xLocal, 2) - powf(yLocal, 2);
            if (zLocal2 <= 0) {
                pixels[i * (size_t) SCREEN_WEIGHT + j].color = sf::Color::Black; 
            } else {
                Vector normal = Vector(xLocal, yLocal, sqrtf(zLocal2));
                Vector normalLight = light - normal;

                normal.normalizeVector();
                normalLight.normalizeVector();

                float cosF = normal * normalLight;
                float cosA = 0;

                if (cosF > 0) {
                    Vector lightAfterReflection = 2 * (normal * normalLight) * normal - normalLight;
                    lightAfterReflection.normalizeVector();

                    Vector perpendicularToNormal = normal;
                    perpendicularToNormal.rotateVector(PI / 2);

                    cosA = lightAfterReflection * perpendicularToNormal;
                } else {
                    cosF = 0;
                }

                sf::Uint8 componentColor = (sf::Uint8) ((A + D * cosF + S * pow(cosA, n)) * 255);
                sf::Color color(componentColor, componentColor, componentColor);
                pixels[i * (size_t) SCREEN_WEIGHT + j].color = color;
            }
        }
    }
}
