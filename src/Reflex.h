#pragma once

#include <vector>
#include <string>

namespace Reflex
{
	//placeholder for Super in BaseObject
	class S
	{
	public:
		static int s_TypeID;
		static int s_parentTypeID;
	};
	//All reflected types should inherit from this class
	class BaseObject
	{
		typedef S Super;
	protected:
		static int s_TypeID;
		static int s_parentTypeID;
	public:
		virtual BaseObject* GetCopy(char* binary) { return nullptr; }

		static int s_GetTypeID() { return 99999999; }

		virtual int GetSize() { return 0; }
		virtual int GetTypeID() { return 99999999; }
		virtual int GetParentTypeID() {return 99999999;}
		friend class ReflectionManager;
	};

	enum class InspectableType
	{
		Int, Float, String, Vec3
	};

	class ReflectedProp
	{
	public:
		ReflectedProp() = default;
		ReflectedProp(std::string name, int offset, InspectableType type, int ID);

		InspectableType type;
		std::string name;
		int offset;
	};

	class ReflectedTypeData
	{
	public:
		std::vector<ReflectedProp*> vars;
		ReflectedTypeData(std::string type_name);

		BaseObject* obj;
		std::string type_name;
	};

	class ReflectionManager
	{
	public:
		static char AddType(std::string type_name, BaseObject* e, int& typeID);
		static char AddProp(std::string name, int offset, InspectableType type, int ID);

		static std::vector<ReflectedTypeData*>* GetTypes();
		static std::vector<ReflectedProp*>*     GetVarsFromType(int ID);

		static const std::string GetTypeName(int ID);
		static const std::string GetTypeName(BaseObject*);

		static ReflectedTypeData* GetType(std::string type_name);

		template<typename T>
		static T* GetVarFromObject(std::string name, BaseObject* obj)
		{
			if (!obj || name.empty())
				return nullptr;

			int ID = ((BaseObject*)obj)->GetTypeID(); //Get the static type ID from the BaseObject derived
			if (!&(*s_reflectionDataList)[ID])
				return nullptr;

			std::vector<ReflectedProp*>* vars = &(*s_reflectionDataList)[ID]->vars; //Get the variables metadata list

			for (int i = 0; i < vars->size(); i++)
			{
				if ((*vars)[i]->name == name) //iterate to find the variable with the same name
				{
					return (T*)((char*)obj + (*vars)[i]->offset); //add the offset to the object pointer to find the needed variable in memory
				}
			}

			return nullptr; //safely return nullptr if there are no variables with the requested name
		}

	private:
		//static std::vector<BaseObject*>* s_types; //this keeps copies of the objects
		static std::vector<ReflectedTypeData*>* s_reflectionDataList; //this stores per-type metadata
	};
}

using namespace Reflex;
#define REFLECTABLE_CLASS(thisClass, parent)  protected: protected: static int s_TypeID; static int s_Size;  static char thisClass##_adder; public: typedef parent Super; virtual int GetTypeID() override {return s_TypeID;} virtual int GetParentTypeID() override {return Super::s_GetTypeID();} virtual BaseObject* GetCopy(char* binary) {return new thisClass##(*(thisClass##*)binary);} virtual int GetSize() override {return s_Size;} static int s_GetSize() {return s_Size;} static int s_GetTypeID() {return s_TypeID;} static int s_GetParentTypeID() {return Super::s_GetTypeID();}
#define REFLECT_REGISTER_TYPE(thisClass) int thisClass##::s_parentTypeID = 0; int thisClass##::s_TypeID = 0; char thisClass::thisClass##_adder = ReflectionManager::AddType(#thisClass, new thisClass, s_TypeID); int thisClass##::s_Size = sizeof(thisClass);
#define REFLECT_PROP(type, thisClass, thisProp) static char  thisProp##_in_##thisClass##      = ReflectionManager::AddProp(#thisProp, offsetof(thisClass, thisProp), type, thisClass##::s_GetTypeID());
#define REFLECTED_PRIV_DECL(type, thisClass, thisProp) static char thisProp##_in_##thisClass;
#define REFLECT_PRIV_PROP(type, thisClass, thisProp) char thisClass##::thisProp##_in_##thisClass##  = ReflectionManager::AddProp(#thisProp, offsetof(thisClass, thisProp), type, thisClass##::s_GetTypeID());