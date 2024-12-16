#ifndef BEAVER_EVENT_H
#define BEAVER_EVENT_H

#include <unordered_map>
#include <functional>
#include <queue>
#include <set>
namespace beaver
{
	namespace component
	{
		namespace event_system
		{
			template<typename T> requires std::is_scoped_enum_v<T>
			struct immediate
			{
				std::unordered_map<T, std::function<void()>> _handlers;
				
				void receive(T e)
				{
					if (_handlers.contains(e)) _handlers.at(e)();
				};
			};


			//will contains immediate event systems
			template<typename EventT> requires std::is_scoped_enum_v<EventT> 
			struct queued
			{
				std::queue<EventT> _event_queue;
				std::set<event_system::immediate<EventT>*> _receivers;
				
				void update()
				{
					while (!_event_queue.empty())
					{
						EventT front_event = _event_queue.front();
						for (auto* receiver: _receivers) receiver->receive(front_event);
						_event_queue.pop();
					};
				};
			};
		}
	}
}
#endif
