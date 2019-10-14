#include "botprofiletable.h"

#include <algorithm>
#include <random>

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "enginecallback.h"
#include "utlvector.h"

CBotProfileTable::CBotProfileTable()
{
}

bool CBotProfileTable::ProfileExists(const CUtlString& name) const
{
	return m_Table.HasElement(name);
}

CBotProfileTable::ProfileData& CBotProfileTable::CreateProfile(const CUtlString& name)
{
	m_Table.Remove(name);
	HashTable::IndexType_t index = m_Table.Insert(name);

	ProfileData& data = m_Table.Element(index);
	data.profileName = name;

	return data;
}

CBotProfileTable::ProfileData* CBotProfileTable::GetProfile(const CUtlString& name)
{
	HashTable::IndexType_t index = m_Table.Find(name);
	return index != m_Table.InvalidIndex() ? &m_Table.Element(index) : NULL;
}

const CBotProfileTable::ProfileData* CBotProfileTable::GetProfile(const CUtlString& name) const
{
	HashTable::IndexType_t index = m_Table.Find(name);
	return index != m_Table.InvalidIndex() ? &m_Table.Element(index) : NULL;
}

CBotProfileTable::ProfileData* CBotProfileTable::GetProfile(uint32_t index)
{
	return (index != m_Table.InvalidIndex() && index < m_Table.Count())
		? &m_Table.Element(index)
		: nullptr;
}

const CBotProfileTable::ProfileData* CBotProfileTable::GetProfile(uint32_t index) const
{
	return (index != m_Table.InvalidIndex() && index < m_Table.Count())
		? &m_Table.Element(index)
		: nullptr;
}

void CBotProfileTable::RemoveProfile(const CUtlString& name)
{
	m_Table.Remove(name);
}

void CBotProfileTable::Clear()
{
	m_Table.Purge();
}

size_t CBotProfileTable::Count() const
{
	return m_Table.Count();
}

void CBotProfileTable::RandomProfileNameList(CUtlVector<CUtlString>& list, size_t count) const
{
	list.Purge();

	if ( m_Table.Count() < 1 )
	{
		return;
	}

	list.EnsureCapacity(count);
	auto rng = std::default_random_engine {(unsigned int)RANDOM_LONG(0, 1000)};

	while ( static_cast<size_t>(list.Count()) < count )
	{
		// This has to be a std::vector in order to randomise it.
		std::vector<const char*> intermediateList;
		intermediateList.reserve(m_Table.Count());

		FOR_EACH_HASHMAP(m_Table, iterator)
		{
			intermediateList.push_back(m_Table.Key(iterator).String());
		}

		std::shuffle(std::begin(intermediateList), std::end(intermediateList), rng);

		for ( const char* name : intermediateList )
		{
			list.AddToTail(CUtlString(name));

			if ( static_cast<size_t>(list.Count()) >= count )
			{
				break;
			}
		}
	}
}
