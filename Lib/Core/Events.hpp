#pragma once

#include "Defines.hpp"

#include "Function.hpp"
#include "Containers\String.hpp"
#include "Containers\Hashmap.hpp"

class NH_API Events
{
	struct Event
	{
		Event() {}
		Event(Event&& other) noexcept : listeners{ Move(other.listeners) } {}
		Event& operator=(Event&& other) noexcept { listeners = Move(other.listeners); return *this; }

		void Destroy() { listeners.Destroy(); }

		bool operator==(const Event& other) const { return this == &other; }
		bool operator!=(const Event& other) const { return this != &other; }

		Vector<Function<void()>> listeners;

		Event(const Event&) = delete;
		Event& operator=(const Event&) = delete;
	};

public:
	static void CreateEvent(const String& name);
	static void ListenForEvent(const String& name, const Function<void()>& response);
	static void ListenForEvent(const String& name, Function<void()>&& response) noexcept;

	static void Notify(const String& name);

private:
	static bool Initialize();
	static void Shutdown();

	static Hashmap<String, Event, false> events;

	STATIC_CLASS(Events);
	friend class Engine;
};