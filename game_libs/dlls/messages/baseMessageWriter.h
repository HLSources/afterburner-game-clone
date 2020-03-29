#pragma once

class CBasePlayer;
typedef struct entvars_s entvars_t;

namespace Messages
{
	class CBaseMessageWriter
	{
	protected:
		CBaseMessageWriter(const char* name, int id);

		bool BeginMessage(CBasePlayer* target = nullptr);
		bool BeginMessage(entvars_t* target = nullptr);
		void EndMessage();
		bool CanWriteMessage() const;

	private:
		const char* m_MessageName = nullptr;
		int m_iMessageId = 0;
	};
}
