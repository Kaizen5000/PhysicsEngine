#include "Physics/Constraint.h"
using namespace Physics;
Physics::Constraint::~Constraint()
{
}
Physics::Constraint::Constraint(Object * objA, Object * objB): m_objA(objA), m_objB(objB)
{

}
