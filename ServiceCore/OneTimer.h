#pragma once
#include <atomic>
#include <mutex>
#include <thread>
#include <chrono>
#include <functional>

class OneTimer
{
	typedef std::chrono::duration<double> oa_seconds;
	enum GateIdx { Timeout_Idx = 0, Reset_Idx, Stop_Idx, Pause_Idx };
public:
	template <class callable, class... arguments>
	OneTimer(int timeout, bool isRepeat, callable&& f, arguments&&... args);

	template <class callable>
	OneTimer(int timeout, bool isRepeat, callable&& f);
	~OneTimer();

	void Start();
	void Reset();
	void Pause();
	void Resume();
	void Stop();

private:
	std::thread _worker;
	std::function<void()> _task;
	oa_seconds m_nTimeout;
	oa_seconds m_duration;

	bool m_IsRepeat;
	std::atomic<GateIdx> m_gateIdx;
	std::mutex m_mutex;
	std::condition_variable timer_condition;
	std::condition_variable pause_condition;
	std::atomic_bool isDone;
	void run();
};

template<class callable, class ...arguments>
inline OneTimer::OneTimer(int timeout, bool isRepeat, callable&& f, arguments&& ...args)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	_task = std::bind(std::forward<callable>(f), std::forward<arguments>(args)...);
	m_nTimeout = oa_seconds(timeout);
	m_duration = oa_seconds(0);
	m_IsRepeat = isRepeat;
	m_gateIdx = Timeout_Idx;
	isDone = false;
}

template <class callable>
OneTimer::OneTimer(int timeout, bool isRepeat, callable&& f)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	_task = std::bind(std::forward<callable>(f));
	m_nTimeout = oa_seconds(timeout);
	m_duration = oa_seconds(0);
	m_IsRepeat = isRepeat;
	m_gateIdx = Timeout_Idx;
	isDone = false;
}
