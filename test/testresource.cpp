#include <beaver/resource.hpp>
struct A
{
	int x;
	std::string u;
};
beaver::resource::manager_v2<int, float, A> rsrc;

int main()
{
	auto x = rsrc.add<int>(34);
	auto s = rsrc.add<A>({.x = 43,.u = "hihihi"});

	auto v = rsrc.get<int>(x);
};
