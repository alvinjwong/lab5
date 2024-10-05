#include "screenSaver.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <cmath>

//Self-explanatory
ScreenSaver::ScreenSaver() {
  shape = sf::CircleShape(10.f);
  velocity = sf::Vector2f(0.0f, 0.0f);
  shape.setFillColor(sf::Color::White);
}

//Self-explanatory
ScreenSaver::ScreenSaver(float radius, sf::Vector2f cVelocity, sf::Color color) {
  shape = sf::CircleShape(radius);
  velocity = cVelocity;
  shape.setFillColor(color);
}

//We have to return shape as a reference!
sf::CircleShape &ScreenSaver::getShape() { return shape; }

//this will draw the sf::CircleShape member of ScreenSaver to the screen
//this will be inherited by all derived class, and shouldn't have to be overriden
void ScreenSaver::draw(sf::RenderWindow &window) { window.draw(getShape()); }

/*Here's some nifty code to resolve any collsions with a wall, by reversing
the velocity of the object to mimic bouncing, you can call this function
in your update functions to make sure your ScreenSaver doesn't wander off-screen*/
void ScreenSaver::checkWallCollision(int screenWidth, int screenHeight){
  sf::Vector2f position = getShape().getPosition();
  float diameter = getShape().getRadius() * 2;

  // Check if the circle hits the right wall
  if (position.x + diameter > screenWidth)
  {
    getShape().setPosition(screenWidth - diameter, position.y);
    velocity.x *= -1.f;
  }

  // Check if the circle hits the left wall
  if (position.x < 0)
  {
    getShape().setPosition(0, position.y);
    velocity.x *= -1.f;
  }

  // Check if the circle hits the top wall
  if (position.y < 0)
  {
    getShape().setPosition(position.x, 0);
    velocity.y *= -1.f;
  }

  // Check if the circle hits the bottom wall
  if (position.y + diameter > screenHeight)
  {
    getShape().setPosition(position.x, screenHeight - diameter);
    velocity.y *= -1.f;
  }
}


//TODO Define all necessary member functions for ClassicSaver, ColorChangingSaver, and CustomSaver Here
//https://learn.microsoft.com/en-us/cpp/cpp/constructors-cpp?view=msvc-170
ClassicSaver::ClassicSaver(float radius, sf::Vector2f velocity, sf::Color color) : 
ScreenSaver(radius, velocity, color) {}

void ClassicSaver::update(float deltaTime, int screenWidth, int screenHeight) {
  getShape().move(velocity.x * deltaTime, velocity.y * deltaTime);
  checkWallCollision(screenWidth, screenHeight);
}

ColorChangingSaver::ColorChangingSaver(float radius, sf::Vector2f velocity, float colorSpeed) : 
ClassicSaver(radius, velocity, sf::Color::Red), colorSpeed(colorSpeed), currentColor(0), colorChange(0.0f) {}

void ColorChangingSaver::update(float deltaTime, int screenWidth, int screenHeight) {
  ClassicSaver::update(deltaTime, screenWidth, screenHeight);
  sf::Color colors[] = {sf::Color::Red, sf::Color::Blue, sf::Color::Green};


  colorChange = colorSpeed * deltaTime;

  if (colorChange > 0.5f) {
    currentColor = (currentColor + 1) % 3;
    getShape().setFillColor(colors[currentColor]);
    colorChange = 0.0f;
  }

}

CustomSaver::CustomSaver(float someFloat, sf::Vector2f someVector, float someOtherFloat) :
ScreenSaver(someFloat, someVector, sf::Color::Yellow), someFloat(someFloat), someVector(someVector),
someOtherFloat(someOtherFloat) {}

void CustomSaver::update(float deltaTime, int screenWidth, int screenHeight) {
  float angle = deltaTime * someOtherFloat * .001;
  float radius = 100.0f;
  float centerX = screenWidth / 2.0;
  float centerY = screenHeight / 2.0;

  float x = centerX + radius * std::cos(angle);
  float y = centerY + radius * std::sin(angle);

  getShape().setPosition(x, y);
}