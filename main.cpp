#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>

class CustomRectangleShape : public sf::RectangleShape
{
public:
    CustomRectangleShape(sf::Vector2f size, sf::Vector2f position) : sf::RectangleShape(size)
    {
        setPosition(position);
    }

    void setSpeed(double speed_x, double speed_y, double speed_r)
    {
        speed_x_ = speed_x;
        speed_y_ = speed_y;
        speed_r_ = speed_r;
    }

    void setSelected(bool is_selected)
    {
        isSelected_ = is_selected;
    }
    bool getSelected()
    {
        return isSelected_;
    }

    void animate(sf::Time elapsed)
    {
        bounce();
        move(speed_x_ * elapsed.asSeconds(), speed_y_ * elapsed.asSeconds());
        rotate(speed_r_ * elapsed.asSeconds());
    }

    void setBounds(double top, double bottom, double left, double right)
    {
        top_ = top;
        bottom_ = bottom;
        left_ = left;
        right_ = right;
    }

    void moveInDirection(const sf::Time &elapsed, const sf::Keyboard::Key &key)
    {
        sf::FloatRect bounds = getGlobalBounds();
        if(key == sf::Keyboard::Up && !(bounds.top <= 0.0))
        {
            move(0, -speed_y_ * elapsed.asSeconds());
        }
        else if(key == sf::Keyboard::Right && !(bounds.left >= right_ - bounds.width))
        {
            move(speed_x_ * elapsed.asSeconds(), 0);
        }
        else if(key == sf::Keyboard::Down && !(bounds.top >= bottom_ - bounds.height))
        {
            move(0, speed_y_ * elapsed.asSeconds());
        }
        else if(key == sf::Keyboard::Left && !(bounds.left <= 0.0))
        {
            move(-speed_x_ * elapsed.asSeconds(), 0);
        }
    }

    bool isClicked(sf::Vector2i &mouse_position)
    {
        sf::FloatRect bounds = getGlobalBounds();
        if(mouse_position.x >= bounds.left && mouse_position.x <= bounds.left + bounds.width && mouse_position.y >= bounds.top && mouse_position.y <= bounds.top + bounds.height)
        {
            return true;
        }
        return false;
    }

    void highlight()
    {
        if(isSelected_ == true)
        {
            setFillColor(sf::Color(255, 0, 0));
        }
        else
        {
            setFillColor(sf::Color(0, 255, 0));
        }
    }

private:
    double speed_x_=0;
    double speed_y_=0;
    double speed_r_=0;
    double top_=0;
    double bottom_=0;
    double left_=0;
    double right_=0;
    bool isSelected_ = false;

    void bounce()
    {
        sf::FloatRect bounds = getGlobalBounds();
        if(bounds.top >= bottom_ - bounds.height)
        {
            speed_y_ = -std::abs(speed_y_);
        }
        else if(bounds.left >= right_ - bounds.width)
        {
            speed_x_ = -std::abs(speed_x_);
        }
        else if(bounds.top <= 0.0)
        {
            speed_y_ = std::abs(speed_y_);
        }
        else if(bounds.left <= 0.0)
        {
            speed_x_ = std::abs(speed_x_);
        }
    }

};

void change_color(sf::RectangleShape &rectangle)
{
    rectangle.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
}

int main()
{

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    std::srand(std::time(nullptr));
    std::vector<CustomRectangleShape> rectangles;

    for(int i=0; i<10; i++)
    {
        sf::Vector2f size(120.0, 60.0);
        sf::Vector2f position(std::rand() % (window.getSize().x - 120), std::rand() % (window.getSize().y - 60));
        rectangles.emplace_back(CustomRectangleShape(size, position));
    }

    for(auto &rec : rectangles)
    {
        rec.setFillColor(sf::Color(0, 255, 0));
        rec.setBounds(0, window.getSize().y, 0, window.getSize().x);
        rec.setSpeed(150, 200, 15);
    }

    sf::Clock clock;


    while (window.isOpen())
    {

        sf::Time elapsed = clock.restart();

        for(auto &rec : rectangles)
        {
            if(rec.getSelected() == false) continue;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                rec.moveInDirection(elapsed, sf::Keyboard::Up);
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                rec.moveInDirection(elapsed, sf::Keyboard::Down);
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                rec.moveInDirection(elapsed, sf::Keyboard::Left);
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                rec.moveInDirection(elapsed, sf::Keyboard::Right);
            }
        }

        sf::Event event;
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if(event.type == sf::Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

                    for(auto &rec : rectangles)
                    {
                        if(rec.isClicked(mouse_pos))
                        {
                            rec.setSelected(true);
                        }
                        else
                        {
                            rec.setSelected(false);
                        }
                        rec.highlight();
                    }
                }
            }
        }

        window.clear(sf::Color::Black);

        for(auto &rec : rectangles)
        {
            window.draw(rec);
        }

        window.display();
    }

    return 0;
}
