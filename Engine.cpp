//Mildred Orella and Simone D'souza 

#include "Engine.h"
#include <iostream>

Engine::Engine()
    : m_Window(VideoMode(800, 600), "Particle Engine") {}

void Engine::input()
{
    Event event;
    while (m_Window.pollEvent(event))
    {
        if (event.type == Event::Closed)
        {
            m_Window.close();
        }
        if (event.type == Event::KeyPressed)
        {
            m_Window.close();
        }

        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
        {
            Vector2i mousePos = Mouse::getPosition(m_Window);
            
            for (int i = 0; i < 5; i++)
            {
                int numPoints = rand() % 26 +25;
                Particle p(m_Window, numPoints, {mousePos.x, mousePos.y});
                m_particles.push_back(p);
            }
        }
    }
}

void Engine::update(float dtAsSeconds)
{
    // Use iterator to manage particle lifecycle
    auto it = m_particles.begin();
    while (it != m_particles.end())
    {
        it->update(dtAsSeconds);

        if (it->getTTL() <= 0.0f)
        {
            it = m_particles.erase(it); // Erase returns the next iterator
        }
        else
        {
            ++it;
        }
    }
}

void Engine::draw()
{
    m_Window.clear();
    for (auto& particle : m_particles)
    {
        m_Window.draw(particle);
    }
    m_Window.display();
}

void Engine::run()
{
    Clock clock;
    cout << "Starting Particle unit tests..." << endl;
    Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 });
    p.unitTests();
    cout << "Unit tests complete.  Starting engine..." << endl;

    while (m_Window.isOpen())
    {
        float dtAsSeconds = clock.restart().asSeconds();

        input();
        update(dtAsSeconds);
        draw();
    }
}
