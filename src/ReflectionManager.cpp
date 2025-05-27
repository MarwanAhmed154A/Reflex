#include "ReflectionManager.h"

#include <string>

namespace Reflex
{
	std::vector<BaseObject*>*        ReflectionManager::s_types;
	std::vector<ReflectedTypeData*>* ReflectionManager::s_reflectionDataList;

	ReflectedProp::ReflectedProp(std::string name, int offset, InspectableType type, int ID) : name(name), offset(offset), type(type)
	{
		ReflectionManager::s_reflectionDataList[0][ID]->vars.push_back(this);
	}


	ReflectedTypeData::ReflectedTypeData(std::string type_name, BaseObject* e, BaseObject* parent, void(*setter)(int, int)) : type_name(type_name)
	{
		ent = e;
		AddedIndex = ReflectionManager::Add(this) - 1;
		parentIndex = parent->GetTypeID();
		setter(AddedIndex, parentIndex);
	}

	void ReflectionManager::Init()
	{
		//types.resize(1);
	}

	int ReflectionManager::Add(ReflectedTypeData* a)
	{
		//initialize lists as ptr, safety from unkown static-init order and UB, on static defintion lines to avoid multiple assignments of value
		static std::vector<BaseObject*>*        types              = s_types              = new std::vector<BaseObject*>();
		static std::vector<ReflectedTypeData*>* reflectionDataList = s_reflectionDataList = new std::vector<ReflectedTypeData*>();

		s_types->push_back( a->ent);
		s_reflectionDataList->push_back(a);
		
		return (int)s_types->size();
	}

	std::vector<BaseObject*>* ReflectionManager::GetTypes()
	{
		return s_types;
	}

	std::vector<ReflectedProp*>* ReflectionManager::GetVarsFromType(int ID)
	{
		return &(*s_reflectionDataList)[ID]->vars;
	}

	const std::string ReflectionManager::GetTypeName(int ID)
	{
		return (*s_reflectionDataList)[ID]->type_name;
	}

	const std::string ReflectionManager::GetTypeName(BaseObject* obj)
	{
		return (*s_reflectionDataList)[obj->GetTypeID()]->type_name;
	}

	BaseObject* ReflectionManager::GetType(std::string type_name)
	{
		for (int i = 0; i < s_reflectionDataList->size(); i++)
		{
			if ((*s_reflectionDataList)[i]->type_name == type_name)
			{
				return (*s_types)[i];
			}
		}
		return nullptr;
	}
}