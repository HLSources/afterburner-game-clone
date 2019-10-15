#pragma once

#include "utlstring.h"
#include "utlhashmap.h"

class CBotProfileTable
{
public:
	struct ProfileData
	{
		CUtlString profileName;
		CUtlString skin;
		CUtlString playerName;
	};

	CBotProfileTable();

	void Clear();
	size_t Count() const;

	bool ProfileExists(const CUtlString& name) const;

	ProfileData* GetProfile(const CUtlString& name);
	const ProfileData* GetProfile(const CUtlString& name) const;

	ProfileData* GetProfile(uint32_t index);
	const ProfileData* GetProfile(uint32_t index) const;

	ProfileData& CreateProfile(const CUtlString& name);
	void RemoveProfile(const CUtlString& name);

	void RandomProfileNameList(CUtlVector<CUtlString>& list, size_t count) const;

private:
	typedef CUtlHashMap<CUtlString, ProfileData> HashTable;
	HashTable m_Table;
};
