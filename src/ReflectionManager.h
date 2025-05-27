#pragma once

#include <vector>
#include <string>

namespace Reflex
{
	//All reflected types should inherit from this class
	class BaseObject
	{
	protected:
		static int s_parentTypeID;
		static int s_TypeID;
		static int s_Size;
		static char _adder;

	public:
		virtual int GetTypeID()
		{
			return s_TypeID;
		}

		virtual BaseObject* GetCopy(char* binary)
		{
			return new BaseObject(*(BaseObject*)binary);
		}

		virtual int GetSize()
		{
			return s_Size;
		}

		static int s_GetTypeID() 
		{
			return s_TypeID;
		}

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
		//ReflectedProp(std::string name, int offset, InspectableType type, class ReflectedTypeData* parent);
		ReflectedProp(std::string name, int offset, InspectableType type, int ID);

		InspectableType type;
		std::string name;
		int offset;
	};

	class ReflectedTypeData
	{
	public:
		std::vector<ReflectedProp*> vars;
		ReflectedTypeData(std::string type_name, BaseObject* e, BaseObject* parent, void(*setter)(int, int));

		int AddedIndex, parentIndex;

		std::string type_name;
		BaseObject* ent;
	};

	class ReflectionManager
	{
	public:
		static void Init();

		static int Add(ReflectedTypeData* a);
		static std::vector<BaseObject*>* GetTypes();
		static std::vector<ReflectedProp*>* GetVarsFromType(int ID);
		static const std::string GetTypeName(int ID);
		static const std::string GetTypeName(BaseObject*);
		static BaseObject* GetType(std::string type_name);

		template<typename T>
		static T* GetVarFromObject(std::string name, BaseObject* obj)
		{
			if (!obj || name.empty())
				return nullptr;

			int ID = ((BaseObject*)obj)->GetTypeID(); //Get the static type ID from the Entity based object (TO DO: change to support components) 
			if (!&(*s_reflectionDataList)[ID])
				return nullptr;

			std::vector<ReflectedProp*>* vars = &(*s_reflectionDataList)[ID]->vars; //Get the variables metadata list

			for (int i = 0; i < s_reflectionDataList->size(); i++)
			{
				if ((*vars)[i]->name == name) //iterate to find the variable with the same name
				{
					return (T*)((char*)obj + (*vars)[i]->offset); //add the offset to the object pointer to find the needed variable in memory
				}
			}

			return nullptr; //safely return nullptr if there are no variables with the requested name
		}

	private:
		static std::vector<BaseObject*>*        s_types;
		static std::vector<ReflectedTypeData*>* s_reflectionDataList;
		friend class  ReflectedProp;
	};
} 

using namespace Reflex;
#define GEN(x, parent) friend class ReflectionManager; protected: static int s_parentTypeID; protected: static int s_TypeID; static int s_Size;  static char x##_adder; public: typedef parent Super; virtual int GetTypeID() override {return s_TypeID;} virtual BaseObject* GetCopy(char* binary) {return new x##(*(x##*)binary);} virtual int GetSize() override {return s_Size;} static int s_GetTypeID() {return s_TypeID;}
#define ADD(x) int  x##::s_parentTypeID = 0; int x##::s_TypeID = 0; char x::x##_adder = *(char*)new ReflectedTypeData(#x, new x, new x##::Super, [](int ID, int pID){x##::s_TypeID = ID; x##::s_parentTypeID = pID;}); int x##::s_Size = sizeof(x);
#define ADDVAR(type, x, v) inline static char v_for_##v## = *(char*)new ReflectedProp(#v, offsetof(x, v), type, x##::s_GetTypeID());
#define ADDPRV(type, x, v) static char;