#pragma once

#include "Define.h"
#include "Math.h"
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "..\Parser\tinyxml\ticpp.h"
#include "..\Parser\tinyxml\tinyxml.h"

enum DataFlags
{
	ds_Named = 0,
};


enum DataEntryType
{
	_float = 0,
	_int,
	_string
};


struct DataEntry
{
	unsigned int		type;
	std::string			id;

	unsigned int		flags;

	int					iValue;
	float				fValue;
	std::string			sValue;

	DataEntry() { }
	DataEntry(int type, std::string id, int iValue, float fValue, std::string sValue);

	bool operator < (const DataEntry& rhs) const
	{
		return this->id < rhs.id;
	}

	bool operator == (const DataEntry& rhs) const
	{
		return this->id == rhs.id;
	}


};


class DataSet
{
	protected:
		std::string					m_id;

		unsigned int				m_flags;
		
		std::vector<DataEntry>		m_data;
		std::vector<DataSet>		m_childSets;
		std::vector<DataSet *>		m_templateSets;

		DataEntry* getDataEntry(const char* pname);
		DataEntry* SearchTemplate(const char* pname);

	public:
		DataSet();
		DataSet(std::string id);

		void addDataEntry(DataEntry data);
		void addChildSet(ticpp::Document *loadedDoc, ticpp::Iterator<ticpp::Element> iDataSet);
		void addTemplate(DataSet* dsTemplate);

		int GetInt(const char* pname, int def = 0);
		float GetFloat(const char* pname, float def = 0.0f);
		std::string GetString(const char* pname, const char* def = "unknown");
		DataSet* GetChildSet(const char* pname);

		int GetFlags() { return m_flags; }
		void SetFlags(int flags) { m_flags |= flags; }
		bool CheckFlags(int flags) const { return ((m_flags & flags) == m_flags ? true : false); }

		std::string GetId() { return m_id; }

		bool operator < (const DataSet& rhs) const
		{
			return this->m_id < rhs.m_id;
		}

		bool operator == (const DataSet& rhs) const
		{
			return this->m_id == rhs.m_id;
		}
};

class Definition
{
	friend class DataSet;
	protected:
		std::string						m_name;
		std::vector<DataSet>			m_dataSets;

		
	public:
		Definition();
		Definition(std::string s) { m_name = s; }

		void addDataSet(DataSet set);
		DataSet* findDataSet(const char* pname);

		void setName(std::string s) { m_name = s; }
		std::string getName() { return m_name; }


		void release();

		bool operator < (const Definition& rhs) const
		{
			return this->m_name < rhs.m_name;
		}

		bool operator == (const Definition& rhs) const
		{
			return this->m_name == rhs.m_name;
		}
};

extern class DefMgr gDefMgr;
class DefMgr 
{
	friend class Definition;
	protected:
		std::vector<Definition>			m_vDefs;

		void releaseDefinition(const char* pname);
		void addDefinition(Definition def);
	
	public:
		DefMgr();
		// Load a single definition.
		void loadDefinition(const char* pname);
		Definition * getDefinition(const char* pname);
		
		// Load multiple definitions.
		void loadBundle(const char* pbundle);
};








	