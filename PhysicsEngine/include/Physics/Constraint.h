#pragma once
namespace Physics
{
	class Object;
	class Constraint
	{
	public:
		virtual ~Constraint();
	protected:
		Constraint(Object * objA, Object * objB);
		Object * m_objA;
		Object * m_objB;
	};
}

