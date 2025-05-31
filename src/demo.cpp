#include "Reflex.h"
#include "demo.h"

#include <iostream>


int main()
{
	Demo demo;

	std::cout << "X: " << * ReflectionManager::GetVarFromObject<int>("x", &demo) << '\n';
	std::cout << "Y: " << * ReflectionManager::GetVarFromObject<int>("y", &demo);

	std::cin.get();
}
