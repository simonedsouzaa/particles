#include "Particle.h"
#include <iostream>
#include <random>

Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition)
    : m_A(2, numPoints), m_ttl(TIME_TO_LIVE), m_numPoints(numPoints), 
      m_radiansPerSec(static_cast<float>(rand()) / RAND_MAX * PI)
{
    m_cartesianPlane = target.getView();
    m_cartesianPlane.setCenter(0, 0);
    m_cartesianPlane.setSize(target.getSize().x, -1 * target.getSize().y);

    m_centerCoordinate = target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane);

    std::default_random_engine generator;
    std::uniform_real_distribution<float> velocityDist(100.0f, 500.0f);
    m_vx = velocityDist(generator) * (rand() % 2 == 0 ? 1 : -1);
    m_vy = velocityDist(generator) * (rand() % 2 == 0 ? 1 : -1);

    m_color1 = Color::White;
    m_color2 = Color::Green;

    double angleIncrement = 2 * PI / m_numPoints;
    for (int i = 0; i < m_numPoints; ++i)
    {
        double angle = i * angleIncrement;
        double radius = 20.0f + static_cast<float>(rand() % (80 - 20 + 1));
        m_A(0, i) = m_centerCoordinate.x + radius * cos(angle);
        m_A(1, i) = m_centerCoordinate.y + radius * sin(angle);
    }
}

void Particle::draw(RenderTarget& target, RenderStates states) const {
    VertexArray lines(TriangleFan, m_numPoints + 1);
    Vector2f center = target.mapCoordsToPixel(m_centerCoordinate, m_cartesianPlane);

    lines[0].position = center;
    lines[0].color = m_color1;

    for (int j = 0; j < m_numPoints; ++j) {
        Vector2f vertex = target.mapCoordsToPixel({m_A(0, j), m_A(1, j)}, m_cartesianPlane);
        lines[j + 1].position = vertex;
        lines[j + 1].color = m_color2;
    }

    target.draw(lines);
}

void Particle::update(float dt)
{
    m_ttl -= dt;
    m_vy += GRAVITY * dt; // Gravity effect

    m_centerCoordinate.x += m_vx * dt;
    m_centerCoordinate.y += m_vy * dt;
}

bool Particle::almostEqual(double a, double b, double eps) noexcept
{
    return std::abs(a - b) < eps;
}

void Particle::unitTests()
{
    // Add unit tests here
    std::cout << "Running Unit Tests..." << std::endl;
    // For example:
    assert(almostEqual(3.0, 3.0));
}
