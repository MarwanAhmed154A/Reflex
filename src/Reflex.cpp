#include "Reflex.h"

#include <string>

namespace Reflex
{
	std::vector<BaseObject*>*        ReflectionManager::s_types;
	std::vector<ReflectedTypeData*>* ReflectionManager::s_reflectionDataList;

	ReflectedProp::ReflectedProp(std::string name, int offset, InspectableType type, int ID) : name(name), offset(offset), type(type) {}

	char ReflectedProp::Create(std::string name, int offset, InspectableType type, int ID)
	{
		(*ReflectionManager::s_reflectionDataList)[ID]->vars.push_back(new ReflectedProp(name, offset, type, ID)); //add this prop's metadata to it's owning class's data
		return '0';
	}


	ReflectedTypeData::ReflectedTypeData(std::string type_name, BaseObject* e, BaseObject* parent, int& typeID, int& parentTypeID) : type_name(type_name), ent(e)
	{
		typeID       = AddedIndex  = ReflectionManager::Add(this) - 1;
		parentTypeID = parentIndex = parent->GetTypeID();
	}

	char ReflectedTypeData::Create(std::string type_name, BaseObject* e, BaseObject* parent, int& typeID, int& parentTypeID)
	{
		new ReflectedTypeData(type_name, e, parent, typeID, parentTypeID);
		return '0';
	}

	int ReflectionManager::Add(ReflectedTypeData* a)
	{
		//initialize lists as ptrs, safety from unkown static-init order and UB, on static defintion lines to avoid multiple assignments of value
		static std::vector<BaseObject*>*        types              = s_types              = new std::vector<BaseObject*>();
		static std::vector<ReflectedTypeData*>* reflectionDataList = s_reflectionDataList = new std::vector<ReflectedTypeData*>();

		s_types->push_back(a->ent);
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

int BaseObject::s_TypeID = 999999999; //for safety