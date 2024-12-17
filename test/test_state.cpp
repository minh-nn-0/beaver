#include <Beaver/state.hpp>
#include <print>
#include <iostream>
enum class test_state
{
	enter,
	exit,
	attack,
};
void test_fsm()
{
	beaver::fsm<std::string> player_fsm;

    // Add states
    player_fsm.add_state("Idle", 
                         []() { std::cout << "Entering Idle\n"; },
                         []() { std::cout << "Exiting Idle\n"; });

    player_fsm.add_state("Running", 
                         []() { std::cout << "Entering Running\n"; },
                         []() { std::cout << "Exiting Running\n"; });

    player_fsm.add_state("Jumping", 
                         []() { std::cout << "Entering Jumping\n"; },
                         []() { std::cout << "Exiting Jumping\n"; });

    // Add transitions
    player_fsm.add_transition("Idle", "Running", []() { std::cout << "Transition: Idle -> Running\n"; });
    player_fsm.add_transition("Running", "Idle", []() { std::cout << "Transition: Running -> Idle\n"; });
    player_fsm.add_transition("Running", "Jumping", []() { std::cout << "Transition: Running -> Jumping\n"; });
    player_fsm.add_transition("Jumping", "Running", []() { std::cout << "Transition: Jumping -> Running\n"; });

    // Test case 1: Transition from Idle to Running
    std::cout << "Test 1: Transition from Idle to Running\n";
    player_fsm.set("Idle");
    player_fsm.set("Running"); // Expected: Enter Running, Exit Idle, Transition Idle -> Running

    // Test case 2: Transition from Running to Idle
    std::cout << "\nTest 2: Transition from Running to Idle\n";
    player_fsm.set("Running");
    player_fsm.set("Idle"); // Expected: Enter Idle, Exit Running, Transition Running -> Idle

    // Test case 3: Transition from Running to Jumping
    std::cout << "\nTest 3: Transition from Running to Jumping\n";
    player_fsm.set("Running");
    player_fsm.set("Jumping"); // Expected: Enter Jumping, Exit Running, Transition Running -> Jumping

    // Test case 4: Invalid transition (Jumping -> Idle)
    std::cout << "\nTest 4: Invalid transition from Jumping to Idle (no direct transition)\n";
    player_fsm.set("Jumping");
    player_fsm.set("Idle"); // Expected: Warning: No transition defined from Jumping to Idle

    // Test case 5: Conditional transition (Idle -> Running only if can_run is true)
    std::cout << "\nTest 5: Conditional transition (Idle -> Running)\n";
    bool can_run = true;
    player_fsm.set_if("Idle", []() { return true; }); // Should set Idle
    player_fsm.set_if("Running", [&]() { return can_run; }); // Should set Running

    can_run = false;
    player_fsm.set_if("Running", [&]() { return can_run; }); // Should NOT set Running
}

int main(int argc, const char** argv)
{
	test_fsm();
};
