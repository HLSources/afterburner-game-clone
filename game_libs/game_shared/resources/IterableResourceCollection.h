#pragma once

#include <cstdint>
#include <cstddef>
#include "utlvector.h"
#include "utlstring.h"

class IIterableSoundResourceCollection
{
public:
	class Iterator
	{
	public:
		inline Iterator()
		{
		}

		inline Iterator(const Iterator& other) :
			m_Parent(other.m_Parent),
			m_Id(other.m_Id),
			m_Index(other.m_Index)
		{
		}

		inline const char* SoundName() const
		{
			const CUtlVector<CUtlString>* soundList = CurrentSoundList();

			if ( !soundList )
			{
				return nullptr;
			}

			if ( m_Index >= soundList->Count() )
			{
				return nullptr;
			}

			return (*soundList)[m_Index].Get();
		}

		inline bool IsValid() const
		{
			return SoundName() != nullptr;
		}

		inline operator bool() const
		{
			return IsValid();
		}

		// Prefix
		inline Iterator& operator ++()
		{
			const CUtlVector<CUtlString>* soundList = CurrentSoundList();

			if ( !soundList )
			{
				// We're invalid - nowhere further to go.
				return *this;
			}

			++m_Index;

			// We keep trying until we encounter a list with items in,
			// or we can no longer get any more lists.
			while ( soundList && m_Index >= soundList->Count() )
			{
				++m_Id;
				m_Index = 0;
				soundList = CurrentSoundList();
			}

			return *this;
		}

		// Postfix
		inline Iterator& operator ++(int)
		{
			Iterator next(*this);
			++(*this);
			return next;
		}

	private:
		friend class IIterableSoundResourceCollection;

		inline Iterator(const IIterableSoundResourceCollection* parent, uint32_t id, uint32_t index) :
			m_Parent(parent),
			m_Id(id),
			m_Index(index)
		{
		}

		const CUtlVector<CUtlString>* CurrentSoundList() const
		{
			if ( !m_Parent || m_Id >= m_Parent->m_Count )
			{
				return nullptr;
			}

			return &m_Parent->m_List[m_Id];
		}

		const IIterableSoundResourceCollection* m_Parent = nullptr;
		uint32_t m_Id = ~0;
		uint32_t m_Index = ~0;
	};

	inline IIterableSoundResourceCollection(const CUtlVector<CUtlString>* list, size_t count) :
		m_List(list),
		m_Count(count)
	{
	}

	inline Iterator Begin() const
	{
		return Iterator(this, 0, 0);
	}

	inline Iterator End() const
	{
		return Iterator();
	}

private:
	friend class Iterator;

	const CUtlVector<CUtlString>* m_List;
	size_t m_Count = 0;
};
