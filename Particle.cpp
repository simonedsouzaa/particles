#include "Particle.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition)
    : m_A(2, numPoints) // Initialize the matrix for storing particle vertices
{
    // Initialize time-to-live with TTL constant
    m_ttl = TTL;
    m_numPoints = numPoints;

    // Set random angular velocity in the range [0, PI]
    m_radiansPerSec = (float)rand() / RAND_MAX * M_PI;

    // Create and set up the Cartesian plane view to map coordinates correctly
    m_cartesianPlane.setCenter(0, 0);
    m_cartesianPlane.setSize(target.getSize().x, -1.0f * target.getSize().y);

    // Map the mouse click position to the Cartesian plane's coordinates
    Vector2f center = Vector2f(target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane));
    m_centerCoordinate = center;

    // Random initial velocities for particle's movement
    m_vx = (rand() % 401 + 100) * (rand() % 2 == 0 ? 1.0f : -1.0f); // Random horizontal velocity between 100 and 500
    m_vy = (rand() % 401 + 100) * (rand() % 2 == 0 ? 1.0f : -1.0f); // Random vertical velocity between 100 and 500

    // Assign random colors for particle
    m_color1 = Color::White;
    m_color2 = Color(rand() % 256, rand() % 256, rand() % 256);

    // Initialize angle for sweeping around the origin
    float theta = (float)rand() / RAND_MAX * (M_PI / 2);  // Random angle between 0 and PI/2
    float dTheta = 2 * M_PI / (numPoints - 1);  // Angle step per point

    // Generate the vertices for the particle shape
    for (int j = 0; j < numPoints; ++j) {
        // Generate random radius between 20 and 150 for more variety
        float r = 20 + (rand() % 131); // Random radius between 20 and 150

        // Convert polar to Cartesian coordinates (dx, dy)
        float dx = r * cos(theta);
        float dy = r * sin(theta);

        // Store the vertices in the matrix (in Cartesian coordinates)
        m_A(0, j) = m_centerCoordinate.x + dx;
        m_A(1, j) = m_centerCoordinate.y + dy;

        // Increment the angle for the next point
        theta += dTheta + (float)rand() / RAND_MAX * 0.1f;  // Add small randomness to dTheta for irregularity
    }
}

void Particle::draw(RenderTarget& target, RenderStates states) const {
    // Create a VertexArray to render the shape using TriangleFan
    VertexArray lines(PrimitiveType::TriangleFan, m_numPoints + 1); // +1 for the center point

    // Map the particle center to pixel coordinates
    Vector2f center = Vector2f(target.mapCoordsToPixel(m_centerCoordinate, m_cartesianPlane));

    // Set the first vertex as the center of the particle
    lines[0].position = center;
    lines[0].color = m_color1; // Color of the center

    // Loop through the particle points and add them to the VertexArray
    for (int j = 0; j < m_numPoints; ++j) {
        Vector2f vertexPos = Vector2f(target.mapCoordsToPixel(Vector2f(m_A(0, j), m_A(1, j)), m_cartesianPlane));
        lines[j + 1].position = vertexPos;  // Map each vertex position from Cartesian to pixel
        lines[j + 1].color = m_color2; // Outer vertex color
    }

    // Draw the particle to the target
    target.draw(lines);
}

void Particle::update(float dt) {
    // Decrease TTL by dt (Time to live countdown)
    m_ttl -= dt;

    // Rotate the particle
    rotate(dt * m_radiansPerSec);

    // Scale the particle gradually
    scale(SCALE);

    // Calculate velocity changes (gravity effect)
    float dx = m_vx * dt;
    float dy = m_vy * dt;
    m_vy -= G * dt;  // Apply gravity to vertical velocity

    translate(dx, dy);
}

void Particle::rotate(double theta) {
    // Temporarily translate to the origin to rotate around the center
    Vector2f temp = m_centerCoordinate;
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y); // Shift to origin
    RotationMatrix R(theta); // Construct rotation matrix
    m_A = R * m_A; // Apply rotation
    translate(temp.x, temp.y); // Translate back to original center
}

void Particle::scale(double c) {
    // Temporarily translate to the origin to scale from the center
    Vector2f temp = m_centerCoordinate;
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y); // Shift to origin
    ScalingMatrix S(c); // Construct scaling matrix
    m_A = S * m_A; // Apply scaling
    translate(temp.x, temp.y); // Translate back to original center
}

void Particle::translate(double xShift, double yShift) {
    // Construct translation matrix with xShift, yShift, and number of points (m_numPoints)
    TranslationMatrix T(xShift, yShift, m_numPoints);
    
    // Apply the translation matrix to m_A (matrix of points)
    m_A = T + m_A;  // Translation is an addition operation for matrices
    
    // Update the center of the particle
    m_centerCoordinate.x += xShift;
    m_centerCoordinate.y += yShift;
}

bool Particle::almostEqual(double a, double b, double eps)
{
return fabs(a - b) < eps;
}
void Particle::unitTests()
{
int score = 0;
cout << "Testing RotationMatrix constructor...";
double theta = M_PI / 4.0;
RotationMatrix r(M_PI / 4);
if (r.getRows() == 2 && r.getCols() == 2 && almostEqual(r(0, 0), cos(theta))
&& almostEqual(r(0, 1), -sin(theta))
&& almostEqual(r(1, 0), sin(theta))
&& almostEqual(r(1, 1), cos(theta)))
{
cout << "Passed. +1" << endl;
score++;
}
else
{
cout << "Failed." << endl;
}
cout << "Testing ScalingMatrix constructor...";
ScalingMatrix s(1.5);
if (s.getRows() == 2 && s.getCols() == 2
&& almostEqual(s(0, 0), 1.5)
&& almostEqual(s(0, 1), 0)
&& almostEqual(s(1, 0), 0)
&& almostEqual(s(1, 1), 1.5))
{
cout << "Passed. +1" << endl;
score++;
}
else
{
cout << "Failed." << endl;
}
cout << "Testing TranslationMatrix constructor...";
TranslationMatrix t(5, -5, 3);
if (t.getRows() == 2 && t.getCols() == 3
&& almostEqual(t(0, 0), 5)
&& almostEqual(t(1, 0), -5)
&& almostEqual(t(0, 1), 5)
&& almostEqual(t(1, 1), -5)
&& almostEqual(t(0, 2), 5)
&& almostEqual(t(1, 2), -5))
{
cout << "Passed. +1" << endl;
score++;
}
else
{
cout << "Failed." << endl;
}
cout << "Testing Particles..." << endl;
cout << "Testing Particle mapping to Cartesian origin..." << endl;
if (m_centerCoordinate.x != 0 || m_centerCoordinate.y != 0)
{
cout << "Failed. Expected (0,0). Received: (" << m_centerCoordinate.x <<
"," << m_centerCoordinate.y << ")" << endl;
}
else
{
cout << "Passed. +1" << endl;
score++;
}
cout << "Applying one rotation of 90 degrees about the origin..." << endl;
Matrix initialCoords = m_A;
rotate(M_PI / 2.0);
bool rotationPassed = true;
for (int j = 0; j < initialCoords.getCols(); j++)
{
if (!almostEqual(m_A(0, j), -initialCoords(1, j)) || !almostEqual(m_A(1,
j), initialCoords(0, j)))
{
cout << "Failed mapping: ";
cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
rotationPassed = false;
}
}
if (rotationPassed)
{
cout << "Passed. +1" << endl;
score++;
}
else
{
cout << "Failed." << endl;
}
cout << "Applying a scale of 0.5..." << endl;
initialCoords = m_A;
scale(0.5);
bool scalePassed = true;
for (int j = 0; j < initialCoords.getCols(); j++)
{
if (!almostEqual(m_A(0, j), 0.5 * initialCoords(0,j)) || !
almostEqual(m_A(1, j), 0.5 * initialCoords(1, j)))
{
cout << "Failed mapping: ";
cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
scalePassed = false;
}
}
if (scalePassed)
{
cout << "Passed. +1" << endl;
score++;
}
else
{
cout << "Failed." << endl;
}
cout << "Applying a translation of (10, 5)..." << endl;
initialCoords = m_A;
translate(10, 5);
bool translatePassed = true;
for (int j = 0; j < initialCoords.getCols(); j++)
{
if (!almostEqual(m_A(0, j), 10 + initialCoords(0, j)) || !
almostEqual(m_A(1, j), 5 + initialCoords(1, j)))
{
cout << "Failed mapping: ";
cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
translatePassed = false;
}
}
if (translatePassed)
{
cout << "Passed. +1" << endl;
score++;
}
else
{
cout << "Failed." << endl;
}
cout << "Score: " << score << " / 7" << endl;
}
