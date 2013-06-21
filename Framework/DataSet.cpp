#include "DataSet.h"
DefMgr gDefMgr;

DefMgr::DefMgr() { }

void DefMgr::loadDefinition(const char *pname)
{
	std::vector<Definition *>temp_templates;	
	ticpp::Document doc(pname);
	doc.LoadFile();
	
	Definition newDef(pname);

	ticpp::Element *pParent = doc.FirstChildElement( "Definition" );
	if(!pParent)
		ASSERT_FAILED;

	ticpp::Iterator<ticpp::Element> iDataSet( "DataSet" );
	for(iDataSet = iDataSet.begin( pParent ); iDataSet != iDataSet.end(); iDataSet++ )
	{
		std::string ds_id;		
	
		(*iDataSet).GetAttributeOrDefault("id", &ds_id, "[unknown]");
		if(ds_id.compare("TemplateDef") == 0)
		{
			Definition* tempDef = getDefinition((*iDataSet).GetText().c_str());
			if(!tempDef)
			{
				loadDefinition((*iDataSet).GetText().c_str());
			}
			temp_templates.push_back(getDefinition((*iDataSet).GetText().c_str()));
			continue;
		}	
		
		DataSet dsTemp(ds_id);
		ticpp::Iterator<ticpp::Element> iDataEntry;
		for(iDataEntry = iDataEntry.begin( &(*iDataSet) ); iDataEntry != iDataEntry.end(); iDataEntry++)
		{
			std::stringstream s;
			int	iType;
			std::string sVal;
			float fVal;
			int iVal;
				
			std::string id;
			std::string type = (*iDataEntry).Value();
			if(type.compare("DataSet") == 0)
			{
				dsTemp.addChildSet(&doc, iDataEntry);
				continue;
			}
			(*iDataEntry).GetAttributeOrDefault("id", &id, "[unknown]");
			if(id.compare("TemplateName") == 0)
			{
				std::string templateName = (*iDataEntry).GetText();
				DataSet* templateSet;
				DataSet* temp;
				if(temp = newDef.findDataSet(templateName.c_str()))
				{
					templateSet = temp;
				}
				else
				{
					std::vector<Definition*>::iterator ipDef;
					for(ipDef = temp_templates.begin(); ipDef != temp_templates.end(); ipDef++)
					{
						if(temp = (*ipDef)->findDataSet(templateName.c_str()))
						{
							templateSet = temp;
							break;
						}
					}
				}
				//dsTemp.addTemplate(templateSet);
				dsTemp.addTemplate(newDef.findDataSet(templateName.c_str()));
				continue;
			}
			s << "Found entry " << id << " of type " << type << " with a value of ";
			if(type.compare("float") == 0)
			{
				(*iDataEntry).GetText<float>(&fVal);
				iType = _float;
				s << fVal;
			}
			else if(type.compare("int") == 0)
			{
				(*iDataEntry).GetText<int>(&iVal);
				iType = _int;
				s << iVal;
			}
			else if(type.compare("string") == 0)
			{
				(*iDataEntry).GetText<std::string>(&sVal);
				iType = _string;
				s << sVal;
			}
			else
			{
				ASSERT_FAILED;
			}
			OutputDebugString(s.str().c_str());
			DataEntry deTemp(iType, id, iVal, fVal, sVal);
			dsTemp.addDataEntry(deTemp);
		}
		newDef.addDataSet(dsTemp);
	}
		
	addDefinition(newDef);
		
}

void DefMgr::loadBundle(const char* pbundle)
{
	std::ifstream	filestream;
	std::string str;

	filestream.open(pbundle, std::ifstream::in);

	if(!filestream)
	{
		OutputDebugString(("Couldn't open bundle %s!", pbundle));
		ASSERT_FAILED;
	}

	while(filestream.good())
	{
		str.clear();
		std::getline(filestream, str);
		loadDefinition(str.c_str());
	}
	filestream.close();
}

void DefMgr::addDefinition(Definition def)
{
	m_vDefs.push_back(def);
	std::sort(m_vDefs.begin(), m_vDefs.end());
}

void DefMgr::releaseDefinition(const char* pname)
{
	// Slow to erase, fast to find.
	std::vector<Definition>::iterator iDef;
	for(iDef = m_vDefs.begin(); iDef != m_vDefs.end(); iDef++)
	{
		if((*iDef).getName().compare(pname) == 0)
		{
			m_vDefs.erase(iDef);
			break;
		}
	}
	std::sort(m_vDefs.begin(), m_vDefs.end());
}

Definition* DefMgr::getDefinition(const char* pname)
{

	if(m_vDefs.size() == 0)
	{
		return NULL;
	}
	else if(m_vDefs.size() == 1)
		return &m_vDefs.front();
	
	Definition temp;
	temp.setName(pname);
	
	return binarySearch( m_vDefs.begin(), m_vDefs.end(), temp );
}

Definition::Definition() {
	m_name.clear();
	m_dataSets.clear();
}

void Definition::addDataSet(DataSet set)
{
	m_dataSets.push_back(set);
	std::sort(m_dataSets.begin(), m_dataSets.end());
}

DataSet* Definition::findDataSet(const char *pname)
{
	DataSet temp(pname);
	return binarySearch(m_dataSets.begin(), m_dataSets.end(), temp);
}

void Definition::release()
{
	gDefMgr.releaseDefinition(m_name.c_str());
}

DataEntry::DataEntry(int type, std::string id, int iValue, float fValue, std::string sValue)
{
	this->type = type;
	this->id = id;
	this->iValue = iValue;
	this->fValue = fValue;
	this->sValue = sValue;
}

DataSet::DataSet() { }

DataSet::DataSet(std::string id)
{
	m_id = id;
}

void DataSet::addDataEntry(DataEntry data)
{
	m_data.push_back(data);
	std::sort(m_data.begin(), m_data.end());
}

void DataSet::addChildSet(ticpp::Document *loadedDoc, ticpp::Iterator<ticpp::Element> iDataSet)
{
	if(!loadedDoc)
		return;

	std::string ds_id;

	(*iDataSet).GetAttributeOrDefault("id", &ds_id, "[unknown]");
	DataSet dsTemp(ds_id);

	ticpp::Iterator<ticpp::Element> iDataEntry;
	for(iDataEntry = iDataEntry.begin( &(*iDataSet) ); iDataEntry != iDataEntry.end(); iDataEntry++)
	{
		std::stringstream s;
		int	iType;
		std::string sVal;
		float fVal;
		int iVal;
					
		std::string id;
		std::string type = (*iDataEntry).Value();
		if(type.compare("DataSet") == 0)
		{
			dsTemp.addChildSet(loadedDoc, iDataEntry);
			continue;
		}
		(*iDataEntry).GetAttributeOrDefault("id", &id, "[unknown]");
		s << "Found entry " << id << " of type " << type << " with a value of ";
		if(type.compare("float") == 0)
		{
			(*iDataEntry).GetText<float>(&fVal);
			iType = _float;
			s << fVal;
		}
		else if(type.compare("int") == 0)
		{
			(*iDataEntry).GetText<int>(&iVal);
			iType = _int;
			s << iVal;
		}
		else if(type.compare("string") == 0)
		{
			(*iDataEntry).GetText<std::string>(&sVal);
			iType = _string;
			s << sVal;
		}
		else
		{
			ASSERT_FAILED;
		}
		OutputDebugString(s.str().c_str());
		DataEntry deTemp(iType, id, iVal, fVal, sVal);
		dsTemp.addDataEntry(deTemp);
	}
	m_childSets.push_back(dsTemp);
	std::sort(m_childSets.begin(), m_childSets.end());	
}

void DataSet::addTemplate(DataSet* dsTemplate)
{
	m_templateSets.push_back(dsTemplate);
}

DataEntry* DataSet::getDataEntry(const char* pname) 
{
	DataEntry temp;
	temp.id = pname;

	return binarySearch( m_data.begin(), m_data.end(), temp );
}

int DataSet::GetInt(const char* pname, int def)
{
	DataEntry* de = getDataEntry(pname);
	if(!de)
		de = SearchTemplate(pname);
	if(!de)
		return def;
	if(de->type != _int)
		return def;
	return de->iValue;
}

float DataSet::GetFloat(const char* pname, float def)
{
	DataEntry* de = getDataEntry(pname);
	if(!de)
		de = SearchTemplate(pname);
	if(!de)
		return def;
	if(de->type != _float)
		return def;
	return de->fValue;
}

std::string DataSet::GetString(const char* pname, const char* def)
{
	DataEntry* de = getDataEntry(pname);
	if(!de)
		de = SearchTemplate(pname);
	if(!de)
		return def;	
	if(de->type != _string)
		return def;
	return de->sValue.c_str();
}

DataEntry* DataSet::SearchTemplate(const char* pname)
{
	std::vector<DataSet*>::iterator iTemplate;
	DataEntry* temp = NULL;
	for(iTemplate = m_templateSets.begin(); iTemplate != m_templateSets.end(); iTemplate++)
	{
		if(temp = (*iTemplate)->getDataEntry(pname))
		{
			break;
		}
	}
	return temp;
}
DataSet* DataSet::GetChildSet(const char* pname)
{
	DataSet temp(pname);

	return binarySearch( m_childSets.begin(), m_childSets.end(), temp );
}