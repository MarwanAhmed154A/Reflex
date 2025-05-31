#pragma once
#include "Reflex.h"

class Demo : public BaseObject
{
	REFLECTABLE_CLASS(Demo, BaseObject) //sets up class to be register-ready
public:
	Demo() : x(234118), y(53215)
	{

	}

	int x;
private:
	int y;

	REFLECTED_PRIV_DECL(InspectableType::Int, Demo, y) //sets up private member to be register-ready
};

REFLECT_REGISTER_TYPE(Demo) //registers class for reflection
REFLECT_PRIV_PROP(InspectableType::Int, Demo, y) //registers private member for reflection
REFLECT_PROP(InspectableType::Int, Demo, x) //registers member for reflectio, no need for DECL as this is public