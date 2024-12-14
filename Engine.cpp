#include "Engine.h"

Engine::Engine()
    : m_Window(VideoMode(800, 600), "Particle Engine")
{
    m_particles.push_back(Particle(m_Window, 5, Vector2i(400, 300)));
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
    }
}

void Engine::update(float dtAsSeconds)
{
    for (auto& particle : m_particles)
    {
        particle.update(dtAsSeconds);
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
