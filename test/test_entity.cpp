#include <Beaver/entity.hpp>

#include <iostream>
#include <chrono>
#include <cassert>
#include <thread>

struct pos
{
	int x {50},y;
};

struct dialogue
{
	std::string i;
};

struct health
{
	int f,g;
};

// Example components
struct Position { float x, y; };
struct Velocity { float dx, dy; };
struct Health { int hp; };

// Function to measure execution time
template<typename Func>
void benchmark(const std::string& label, Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << label << " took "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";
}
void test_manager_v2()
{
    std::cout << "Running tests for manager_v2...\n";
	beaver::entity::manager_v2<Position, Velocity> manager;

    // Utility for printing components
    auto print_entity_info = [&](std::size_t eid) {
        try {
            auto tags = manager.get_tags(eid);
            std::cout << "Entity " << eid << " Tags: ";
            for (const auto& tag : tags)
                std::cout << tag << " ";
            std::cout << "\n";

            if (manager.have_component<Position>(eid))
                std::cout << "  Position: " << manager.get_component<Position>(eid)->x << " " << manager.get_component<Position>(eid)->y << "\n";
            else
                std::cout << "  Position: None\n";

            if (manager.have_component<Velocity>(eid))
                std::cout << "  Velocity: " << manager.get_component<Velocity>(eid)->dx << " " << manager.get_component<Velocity>(eid)->dy << "\n";
            else
                std::cout << "  Velocity: None\n";

        } catch (const std::exception& ex) {
            std::cout << "Error accessing entity " << eid << ": " << ex.what() << "\n";
        }
    };

    // Test 1: Adding entities
    std::size_t e1 = manager.add_entity();
    std::size_t e2 = manager.add_entity();
    std::cout << "Added entities: e1=" << e1 << ", e2=" << e2 << "\n";

    // Add components
    manager.add_component<Position>(e1, {1.0f, 2.0f});
    manager.add_component<Velocity>(e2, {3.0f, 4.0f});
    std::cout << "Added components to entities.\n";

    // Print initial state
    print_entity_info(e1);
    print_entity_info(e2);

    // Test 2: Adding and removing tags
    manager.add_tags(e1, "player");
    manager.add_tags(e1, "active");
    manager.add_tags(e2, "enemy");
    std::cout << "Tags added.\n";

    print_entity_info(e1);
    print_entity_info(e2);

    manager.remove_tags(e1, "active");
    std::cout << "Removed tag 'active' from e1.\n";
    print_entity_info(e1);

    // Test 3: Resetting entity
    manager.reset_entity(e1);
    std::cout << "Reset entity e1.\n";
    print_entity_info(e1);

    // Test 4: Recycling entity IDs
    std::size_t e3 = manager.add_entity();
    std::cout << "Recycled entity ID: e3=" << e3 << "\n";
    print_entity_info(e3);

    // Test 5: Edge cases
    try {
        manager.get_component<Position>(100); // Invalid entity ID
    } catch (const std::exception& ex) {
        std::cout << "Caught expected exception: " << ex.what() << "\n";
    }

    // Test 6: Performance Benchmark
    std::cout << "Running performance benchmark...\n";
    const std::size_t num_entities = 100000;
    for (std::size_t i = 0; i < num_entities; ++i) {
        manager.add_entity();
    }
    std::cout << "Added " << num_entities << " entities.\n";

    for (std::size_t i = 0; i < num_entities; ++i) {
        manager.add_component<Position>(i, {static_cast<float>(i), static_cast<float>(i)});
    }
    std::cout << "Added Position components to " << num_entities << " entities.\n";

    // Verify a few random entities
    for (std::size_t i = 0; i < 10; ++i) {
        auto eid = i * 1000; // Sample entities
        print_entity_info(eid);
    }

    std::cout << "Tests completed.\n";
}


void stress_test_manager_v2()
{
    std::cout << "Running stress test for manager_v2...\n";

	beaver::entity::manager_v2<Position, Velocity> manager;

    const std::size_t num_entities = 10000000; // High number of entities for stress testing
    const std::size_t num_operations = 10000000; // Number of operations to perform on the entities

    // Benchmark for adding and removing entities
    auto start = std::chrono::high_resolution_clock::now();
    for (std::size_t i = 0; i < num_operations; ++i) {
        std::size_t eid = manager.add_entity();
        if (i % 2 == 0) {
            manager.reset_entity(eid); // Reset some entities in between
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken for adding and resetting " << num_operations << " entities: " 
              << duration.count() << " seconds\n";

    // Benchmark for adding and resetting components
    start = std::chrono::high_resolution_clock::now();
    for (std::size_t i = 0; i < num_operations; ++i) {
        std::size_t eid = manager.add_entity();
        manager.add_component<Position>(eid, {static_cast<float>(eid), static_cast<float>(eid)});
        if (i % 2 == 0) {
            manager.reset_component<Position>(eid); // Reset components intermittently
        }
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Time taken for adding and resetting components: " << duration.count() << " seconds\n";

    // Benchmark for adding and removing tags
    start = std::chrono::high_resolution_clock::now();
    for (std::size_t i = 0; i < num_operations; ++i) {
        std::size_t eid = manager.add_entity();
        manager.add_tags(eid, "active");
        if (i % 2 == 0) {
            manager.remove_tags(eid, "active");
        }
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Time taken for adding and removing tags: " << duration.count() << " seconds\n";

    // Edge case: Test adding a large number of entities and components in a single batch
    std::cout << "Testing adding a large number of entities and components in a batch...\n";
    start = std::chrono::high_resolution_clock::now();
    for (std::size_t i = 0; i < num_entities; ++i) {
        std::size_t eid = manager.add_entity();
        manager.add_component<Position>(eid, {static_cast<float>(i), static_cast<float>(i)});
        manager.add_component<Velocity>(eid, {static_cast<float>(i + 1), static_cast<float>(i + 1)});
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Time taken for adding " << num_entities << " entities and components: " 
              << duration.count() << " seconds\n";

    // Random entity component access and manipulation
    std::cout << "Testing random entity access and manipulation...\n";
    start = std::chrono::high_resolution_clock::now();
    for (std::size_t i = 0; i < num_operations; ++i) {
        std::size_t eid = i % num_entities;
        if (manager.have_component<Position>(eid)) {
            manager.get_component<Position>(eid); // Access component
        }
        if (i % 2 == 0) {
            manager.add_component<Velocity>(eid, {static_cast<float>(eid), static_cast<float>(eid)});
        }
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Time taken for random entity access and component manipulation: " 
              << duration.count() << " seconds\n";

    // Concurrency: Simulate concurrent entity additions (multithreaded)
    std::cout << "Testing multithreaded entity additions...\n";
    std::vector<std::thread> threads;
    auto thread_func = [&]() {
        for (std::size_t i = 0; i < num_operations / 10; ++i) {
            manager.add_entity();
        }
    };
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10; ++i) {
        threads.push_back(std::thread(thread_func));
    }
    for (auto& t : threads) {
        t.join();
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Time taken for multithreaded entity additions: " << duration.count() << " seconds\n";

    std::cout << "Stress test completed.\n";
}

beaver::entity::manager<pos, dialogue, health> emanager;

beaver::entity::manager_v2<pos, dialogue, health> emanager2;

int main()
{
	//auto x = emanager.add_entity();
	//auto g = emanager.get_component<pos>(x);
	//*g = {89, 1231};
	//
	//auto x2 = emanager.add_entity();
	//auto g2 = emanager.get_component<pos>(x2);
	//*g2 = {22222, 1231};
	//
	//for (const auto& e: emanager._entities)
	//{
	//	std::cout << emanager.get_component<pos>(e._id)->_value.x << '\n';
	//	std::cout << e._id << '\n';
	//};

	//emanager._entities.at(x2)._active = false;

	//auto x3 = emanager.add_entity();
	//auto g3 = emanager.get_component<pos>(x3);
	//*g3 = {55555, 1231};

	//for (auto i = 0; i != emanager._entities.size(); i++)
	//{
	//	auto [p, h] = emanager.get_components<pos, health>(i);
	//	p->_value.x++;
	//	h->_value.f++;
	//	std::cout << i << '\n';
	//	std::cout << p->_value.x << '\t' << h->_value.f << '\n';

	//};
	//std::cout << "=============================\n";

	//for (auto [p, h] : emanager.view<pos, health>())
	//{
	//	std::cout << p->x << '\t' << h->f << '\n';
	//};
	//std::cout << emanager._entities.max_size();
	

	test_manager_v2();

	stress_test_manager_v2();

	return 1;

};

