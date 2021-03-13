#include "ThreadEvent.h"

ThreadEvent::ThreadEvent(bool isManualReset)
	: m_Initialized(true)
	, m_IsManualReset(isManualReset)
	, m_Trigger(TriggerType::NONE)
	, m_WaitingThreads(0)
{
}

ThreadEvent::~ThreadEvent()
{
	if (m_Initialized)
	{
		Lock();
		m_IsManualReset = true;
		Unlock();

		Trigger();

		Lock();
		m_Initialized = false;
		while (m_WaitingThreads > 0)
		{
			Unlock();
			Lock();
		}
		Unlock();
	}
}

void ThreadEvent::Trigger()
{
	Lock();

	if (m_IsManualReset)
	{
		m_Trigger = TriggerType::ALL;
		m_Condition.notify_all();
	}
	else
	{
		m_Trigger = TriggerType::NONE;
		m_Condition.notify_one();
	}

	Unlock();
}

void ThreadEvent::Reset()
{
	Lock();
	m_Trigger = TriggerType::NONE;
	Unlock();
}

bool ThreadEvent::Wait(uint32 waitTime)
{
	std::unique_lock<std::mutex> uniqueLock(m_Mutex);

	bool needWaiting = true;

	do
	{
		if (m_Trigger == TriggerType::ONE)
		{
			m_Trigger = TriggerType::NONE;
			needWaiting = false;
		}
		else if (m_Trigger == TriggerType::ALL)
		{
			needWaiting = false;
		}
		else if (waitTime == (uint32)-1)
		{
			m_WaitingThreads += 1;
			m_Condition.wait(uniqueLock);
			m_WaitingThreads -= 1;
		}
		else
		{
			m_WaitingThreads += 1;
			m_Condition.wait_for(uniqueLock, std::chrono::microseconds(waitTime));
			m_WaitingThreads -= 1;
		}
	} while (needWaiting);

	return !needWaiting;
}
