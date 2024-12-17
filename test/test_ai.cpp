#include <Beaver/core.hpp>
#include <iostream>
#include <cassert>


using enum beaver::BT::NODE_RESULT;
// provided by chatgpt
//void test_selector() 
//{
//    // Test 1: Selector should return SUCCESS if one child succeeds
//	beaver::BT::selector sel;
//    sel._childs = { [] { return FAILURE; }, 
//                    [] { return SUCCESS; }, 
//                    [] { return FAILURE; } };
//    assert(sel.update() == RUNNING);   // First child fails, selector moves to next
//    assert(sel.update() == SUCCESS);   // Second child succeeds, selector should succeed
//
//    // Test 2: Selector should return FAILURE if all children fail
//    sel._childs = { [] { return FAILURE; }, 
//                    [] { return FAILURE; } };
//    sel._current_child = 0;
//    assert(sel.update() == RUNNING);   // First child fails, selector moves to next
//    assert(sel.update() == FAILURE);   // All fail, selector should fail
//
//    // Test 3: Selector should handle RUNNING states and resume correctly
//    int callCount = 0;
//    sel._childs = { [&callCount] { callCount++; return RUNNING; }, 
//                    [] { return SUCCESS; } };
//    sel._current_child = 0;
//    assert(sel.update() == RUNNING);   // First child is running
//    assert(callCount == 1);
//    assert(sel.update() == RUNNING);   // Still running
//    assert(callCount == 2);
//}
//
//void test_sequence()
//{
//    // Test 1: Sequence should return SUCCESS if all children succeed
//	beaver::BT::sequence seq;
//    seq._childs = { [] { return SUCCESS; }, 
//                    [] { return SUCCESS; } };
//    assert(seq.update() == RUNNING);   // First child succeeds, sequence moves to next
//    assert(seq.update() == SUCCESS);   // All succeed, sequence should succeed
//
//    // Test 2: Sequence should return FAILURE if one child fails
//    seq._childs = { [] { return SUCCESS; }, 
//                    [] { return FAILURE; }, 
//                    [] { return SUCCESS; } };
//    seq._current_child = 0;
//    assert(seq.update() == RUNNING);   // First child succeeds
//    assert(seq.update() == FAILURE);   // Second child fails, sequence should fail
//
//    // Test 3: Sequence should handle RUNNING states and resume correctly
//    int callCount = 0;
//    seq._childs = { [] { return SUCCESS; }, 
//                    [&callCount] { callCount++; return RUNNING; }, 
//                    [] { return SUCCESS; } };
//    seq._current_child = 0;
//    assert(seq.update() == RUNNING);   // First child succeeds, sequence moves to next
//    assert(seq.update() == RUNNING);   // Second child is running
//    assert(callCount == 1);
//    assert(seq.update() == RUNNING);   // Still running
//    assert(callCount == 2);
//}



struct position
{
	int x, y;
};

struct dialogue
{
	std::string content;
	float timer;
};

struct blackboard
{

};

struct planner
{
};


struct character_state
{
};
void move(position& p, int x)
{
	p.x += x;
};

enum class action_rs { success, running, failure};

struct world
{
	beaver::entity::manager<position, dialogue, blackboard, planner> _charmanager;

};
int main()
{

};
