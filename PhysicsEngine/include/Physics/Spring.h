#pragma once
#include "Constraint.h"
namespace Physics
{
	class Object;
	class Spring : public Constraint
	{
	public:
		// Constructor
		Spring(Object * objA, Object * objB, float restingLength, float springCoefficient, float damping);

		// Destructor
		~Spring();

		// Update and draw
		void update(float deltaTime);
		void draw();

	protected:
		float m_restingLength;		// At this length, the spring doesn't apply force
		float m_springCoefficient;	// How strongly the spring will try return to resting length
		float m_damping;			// Internal spring friction
	};
}

