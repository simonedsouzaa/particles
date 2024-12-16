#include "Engine.h"
#include <iostream>

Engine::Engine()
    : m_Window(VideoMode(800, 600), "Particle Engine")
{
    // Initialize empty particle vector
}

void Engine::input()
{
    Event event;
    while (m_Window.pollEvent(event))
    {
        if (event.type == Event::Closed)
        {
            m_Window.close();
        }
        else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
        {
            Vector2i mousePosition(event.mouseButton.x, event.mouseButton.y);

            // Generate 5 particles with randomized shapes and behaviors
            for (int i = 0; i < 5; ++i)
            {
                int numPoints = rand() % 26 + 25; // Randomize vertices between 25 and 50
                m_particles.emplace_back(m_Window, numPoints, mousePosition);
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
    for (const auto& particle : m_particles)
    {
        m_Window.draw(particle);
    }
    m_Window.display();
}

void Engine::run()
{
    Clock clock;

    while (m_Window.isOpen())
    {
        float dtAsSeconds = clock.restart().asSeconds();

        input();
        update(dtAsSeconds);
        draw();
    }
}
