#pragma once

#include <vector>
#include <functional>
#include <memory>
#include <list>
#include <mutex>
#include "IBaseClass.h"

template<typename... inputParameterTypes>
class IDelegate
{
private:

	struct BindedFunction
	{
		std::function<void(inputParameterTypes...)> function;
		std::function<bool()> validPredicate;
	};

public:

	IDelegate() = default;
	virtual ~IDelegate()
	{
		for (auto bindedEvent : m_BindedEvents)
			delete bindedEvent;
	}

	IDelegate(const IDelegate<inputParameterTypes...>& other) = default;
	IDelegate(IDelegate<inputParameterTypes...>&& other) = default;
	IDelegate& operator=(const IDelegate<inputParameterTypes...>& other) = default;
	IDelegate& operator=(IDelegate<inputParameterTypes...>&& other) = default;

	// bind a function that will be invalidated when the IBaseClass instance goes out of scope
	void BindFunction(IBaseClass* bindedObject, const std::function<void(inputParameterTypes...)>& function)
	{
		if (bindedObject)
		{
			auto weakPtr = bindedObject->GetWeakReference();
			m_BindedEvents.push_back(new BindedFunction{
				function,
				[weakPtr]() {return !weakPtr.expired(); }
				});
		}
	}

	// bind a function and a predecate that should return false if the function is invalidated
	void BindFunction(const std::function<bool()>& ValidPredicate, const std::function<void(inputParameterTypes...)>& function)
	{
		m_BindedEvents.push_back(new BindedFunction{
				function,
				ValidPredicate });
	}

	// bind a global function that should never get invalidated
	void BindFunction(const std::function<void(inputParameterTypes...)>& function)
	{
		m_BindedEvents.push_back(new BindedFunction{
				function,
				[]() {return true; } });
	}

	void BroadCast(const inputParameterTypes&... input...)
	{
		//m_BroadcastMutex.lock();
		auto it = m_BindedEvents.rbegin();
		while (it != m_BindedEvents.rend())
		{
			if ((*it)->validPredicate())
			{
				(*it)->function(input...);
			}
			else
			{
				m_BindedEvents.remove(*it);
			}
			if (it != m_BindedEvents.rend())++it;
			else break;
		}
		//m_BroadcastMutex.unlock();
	}

	void RemoveAllFunctions()
	{
		m_BindedEvents.clear();
	}

private:

	std::list<BindedFunction*> m_BindedEvents;

	//std::mutex m_BroadcastMutex;

};