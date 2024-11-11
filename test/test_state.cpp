#include <Beaver/state.hpp>
#include <print>

enum class test_state
{
	enter,
	exit,
	attack,
};

int main(int argc, const char** argv)
{
	beaver::state<test_state> test;
	test._cur = test_state::enter;
	beaver::state<test_state> test2;
	
	test.new_transition(test_state::enter, test_state::exit, [](){std::print("new transition");});
	test.new_transition(test_state::enter, test_state::attack, [](){std::print("attack from enter");});
	test.new_transition(test_state::exit, test_state::attack, [](){std::print("attack from exit");});
	test.new_transition(test_state::attack, test_state::exit, [](){std::print("exit from attack");});
	//beaver::state<player_states> player {	{enter,{exit,[](){}}
	//
	//
	if (std::stoi(argv[1]) == 1) 
	{
		test.change(test_state::exit);
		std::println("{}",test.is(test_state::exit));
		test.change(test_state::attack);
		test.change(test_state::exit);
	} 
};
