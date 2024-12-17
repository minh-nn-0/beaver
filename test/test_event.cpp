#include <Beaver/event.hpp>
#include <print>
enum class testevent
{
	pdie,
	ptalk,
	ctalk,
};

struct character
{
	beaver::event_system::immediate<testevent> _events;
	std::string _dialogue;
};

void otherupdatefunc(character& a, character& b)
{
	a._dialogue = "dmm";
	if (a._dialogue == "dmm") b._events.receive(testevent::ptalk);
};



struct world
{
	void update()
	{
		_events.update();
		std::println("{}", _events._event_queue.size());
	};
	beaver::event_system::queued<testevent> _events;
};




int main()
{
	character a,b;
	a._events._handlers.emplace(testevent::ctalk, [&a](){a._dialogue = "a ctalk event"; std::println("character talk");});
	a._events._handlers.emplace(testevent::ptalk, [&a](){a._dialogue = "a ptalk event"; std::println("char talk");});
	b._events._handlers.emplace(testevent::ctalk, [&b](){b._dialogue = "hahahahahaha"; std::println("dmmmmm");}); 
	
	otherupdatefunc(a,b);
	a._events.receive(testevent::ptalk);
	
	std::println("a dialogue {} ; b dialogue {}", a._dialogue, b._dialogue);
	
	world w;
	w._events._event_queue.push(testevent::ctalk);
	w._events._receivers.insert({&a._events, &b._events});
	std::println("receiver num {}", w._events._receivers.size());
	w.update();
	
	std::println("a dialogue {} ; b dialogue {}", a._dialogue, b._dialogue);
	
};
