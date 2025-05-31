#pragma once
#include "Reflex.h"

class Demo : public BaseObject
{
	REFLECTABLE_CLASS(Demo, BaseObject)
public:
	Demo() : x(234118), y(53215)
	{

	}

	int x;
private:
	int y;

	REFLECTED_PRIV_DECL(InspectableType::Int, Demo, y)
};

REFLECT_REGISTER_TYPE(Demo)
REFLECT_PRIV_PROP(InspectableType::Int, Demo, y)
REFLECT_PROP(InspectableType::Int, Demo, x) //no need for DECL as this is public