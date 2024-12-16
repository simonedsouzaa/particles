#pragma once
#include <SFML/Graphics.hpp>
#include "Particle.h"
using namespace sf;
using namespace std;

class Engine
{
private:
    RenderWindow m_Window;
    vector<Particle> m_particles;

    void input();
    void update(float dtAsSeconds);
    void draw();

public:
    Engine();
    void run();
};
