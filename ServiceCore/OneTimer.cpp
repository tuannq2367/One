#include "pch.h"
#include "OneTimer.h"

OneTimer::~OneTimer()
{
	Stop();
	try
	{
		_worker.join();
	}
	catch (std::system_error&)
	{
	}
}

void OneTimer::Start()
{
	if (m_gateIdx != Stop_Idx && m_gateIdx != Timeout_Idx)
		return;

	m_gateIdx = Timeout_Idx;
	_worker = std::thread(&OneTimer::run, this);
}

void OneTimer::Reset()
{
	//std::unique_lock<std::mutex> lock(m_mutex);
	if (m_gateIdx == Stop_Idx)
		return;

	m_gateIdx = Reset_Idx;
	m_duration = oa_seconds(0);
	timer_condition.notify_all();
	pause_condition.notify_all();
}

void OneTimer::Pause()
{
	//std::unique_lock<std::mutex> lock(m_mutex);
	if (m_gateIdx != Timeout_Idx)
		return;

	m_gateIdx = Pause_Idx;
	timer_condition.notify_all();
}

void OneTimer::Resume()
{
	//std::unique_lock<std::mutex> lock(m_mutex);
	if (m_gateIdx != Pause_Idx)
		return;

	m_gateIdx = Timeout_Idx;
	pause_condition.notify_all();
}

void OneTimer::Stop()
{
	if (m_gateIdx != Stop_Idx)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_gateIdx = Stop_Idx;
		timer_condition.notify_all();
		pause_condition.notify_all();
	}
}

void OneTimer::run()
{
	while (!isDone)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		auto _start = std::chrono::system_clock::now();
		m_gateIdx = Timeout_Idx;

		if (!timer_condition.wait_until(lock, _start + m_nTimeout - m_duration, [this]() {return m_gateIdx != Timeout_Idx; }))
		{
			m_duration = oa_seconds(0);
			if (!m_IsRepeat)
				isDone = TRUE;
			// do work
			_task();
		}
		else {
			if (m_gateIdx == Stop_Idx)
			{
				m_duration = oa_seconds(0);
				return;
			}
			else if (m_gateIdx == Reset_Idx)
			{
				m_gateIdx = Timeout_Idx;
				m_duration = oa_seconds(0);
				continue;
			}
			else if (m_gateIdx == Pause_Idx)
			{
				m_duration += (std::chrono::system_clock::now() - _start);
				std::mutex m;
				std::unique_lock<std::mutex> _lock(m);
				pause_condition.wait(_lock, [this]() {return m_gateIdx != Pause_Idx; });
			}
		}
	}
}

