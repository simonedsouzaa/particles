#pragma once
#include "Matrices.h"
#include <SFML/Graphics.hpp>
#define M_PI 3.1415926535897932384626433

// Constants for particle behavior
const float G = 1000;        // Gravity constant
const float TTL = 5.0;       // Time to live (TTL)
const float SCALE = 0.999;   // Scaling factor for particle shrinking

using namespace Matrices;     // Include Matrix class for transformations
using namespace sf;           // Include SFML for rendering and math

// Particle class definition
class Particle : public Drawable
{
public:
    // Constructor: Initializes a particle with a random shape and behavior
    Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition);

    // Override draw function to render the particle
    virtual void draw(RenderTarget& target, RenderStates states) const override;

    // Update the particle's state based on the time delta
    void update(float dt);

    // Getter for the particle's TTL (time to live)
    float getTTL() { return m_ttl; }

    // Functions for unit testing
    bool almostEqual(double a, double b, double eps = 0.0001);
    void unitTests();

private:
    float m_ttl;                // Time-to-live of the particle
    int m_numPoints;            // Number of points defining the particle's shape
    Vector2f m_centerCoordinate; // Particle's center position in Cartesian space
    float m_radiansPerSec;      // Angular velocity for rotation
    float m_vx;                 // Velocity in the X direction
    float m_vy;                 // Velocity in the Y direction
    View m_cartesianPlane;      // View for mapping Cartesian coordinates to screen
    Color m_color1;             // Color for the particle's center
    Color m_color2;             // Color for the particle's edges
    Matrix m_A;                 // Matrix storing the particle's shape (vertices)

    // Rotate the particle by theta radians counter-clockwise
    // Uses a RotationMatrix to perform the transformation
    void rotate(double theta);

    // Scale the particle by a factor 'c'
    // Uses a ScalingMatrix to perform the transformation
    void scale(double c);

    // Translate the particle by (xShift, yShift) coordinates
    // Uses a TranslationMatrix to perform the transformation
    void translate(double xShift, double yShift);
};
