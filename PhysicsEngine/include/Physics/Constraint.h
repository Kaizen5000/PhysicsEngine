#pragma once
/*
	Constraint pure virtual class which is a base for all types of constraints used in the project
*/
namespace Physics
{
	class Object;
	class Constraint
	{
	public:
		// Virtual destructor
		virtual ~Constraint();
	protected:
		// Protected constructor so that only child classes can initialise this
		// To initialise, the constructor takes object pointers to the objects that the constraint constrains
		Constraint(Object * objA, Object * objB);
		Object * m_objA;
		Object * m_objB;
	};
}

