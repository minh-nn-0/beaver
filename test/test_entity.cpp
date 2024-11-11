#include <Beaver/entity.hpp>

#include <iostream>
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


beaver::entity::manager<pos, dialogue, health> emanager;

int main()
{
	auto x = emanager.add_entity();
	auto g = emanager.get_component<pos>(x);
	*g = {89, 1231};
	
	auto x2 = emanager.add_entity();
	auto g2 = emanager.get_component<pos>(x2);
	*g2 = {22222, 1231};
	
	for (const auto& e: emanager._entities)
	{
		std::cout << emanager.get_component<pos>(e._id)->x << '\n';
		std::cout << e._id << '\n';
	};

	emanager._entities.at(x2)._active = false;

	auto x3 = emanager.add_entity();
	auto g3 = emanager.get_component<pos>(x3);
	*g3 = {55555, 1231};

	for (auto i = 0; i != emanager._entities.size(); i++)
	{
		auto [p, h] = emanager.get_components<pos, health>(i);
		p->x++;
		h->f++;
		std::cout << i << '\n';
		std::cout << p->x << '\t' << h->f << '\n';

	};
	std::cout << "=============================\n";

	for (auto [p, h] : emanager.view<pos, health>())
	{
		std::cout << p->x << '\t' << h->f << '\n';
	};
	std::cout << emanager._entities.max_size();
};

