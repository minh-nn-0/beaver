#ifndef M_RESOURCE_H
#define M_RESOURCE_H
#include <unordered_map>
#include <memory>
#include <vector>

template <typename... Ts>
struct contains : std::false_type {};

template <typename T, typename Head, typename... Tail>
struct contains<T, Head, Tail...> : std::conditional<
                                        std::is_same_v<T, Head>,
                                        std::true_type,
                                        contains<T, Tail...>>::type {};
namespace beaver
{
	namespace resource
	{
		// TODO fix this struct - do we need another class for handling information, should T contains all needed information everytime we touch it?
		// -- generally yes. For now we don't actually need this
		//template<typename T>
		//struct base_resource1
		//{
		//	std::unique_ptr<T> _value;

		//	operator T&() {return *_value;};
		//	operator T*() {return _value.get();};
		//	
		//	std::size_t _id;
		//	std::string _name;
		//	std::string _tag;
		//};
		

		template<typename T>
		using map_t = std::unordered_map<std::string, std::unique_ptr<T>>;
		template<typename T>
		using vec_t = std::vector<T*>;

		template<typename... RsrcTs>
		struct manager
		{
			std::tuple<map_t<RsrcTs>...> _maps;
			std::tuple<vec_t<RsrcTs>...> _vecs;
			
			template<typename RsrcT>
			RsrcT* get(const std::string& name) 
			{
				return get_map<RsrcT>().at(name).get();
			};

			template<typename RsrcT>
			const RsrcT* get(const std::string& name) const
			{
				return get_cmap<RsrcT>().at(name).get();
			};

			template<typename RsrcT>
			map_t<RsrcT>& get_map() {return std::get<map_t<RsrcT>>(_maps);};
			
			template<typename RsrcT>
			const map_t<RsrcT>& get_cmap() const {return std::get<map_t<RsrcT>>(_maps);};
			
			template<typename RsrcT>
			vec_t<RsrcT>& get_vec() {return std::get<vec_t<RsrcT>>(_vecs);};
			
			template<typename RsrcT>
			const vec_t<RsrcT>& get_cvec() const {return std::get<vec_t<RsrcT>>(_vecs);};
			
			template<typename RsrcT>
			RsrcT* add(const std::string& custom_name = "", RsrcT&& arg = {})
			{
				// emplace a new resource in the map and store reference to 
				// that new resource/*which is an unique pointer*/.get(), to the vec;
				auto [new_resource, _] = get_map<RsrcT>().emplace(make_resource_name<RsrcT>(custom_name),
																	std::make_unique<RsrcT>(std::forward<RsrcT>(arg)));
				return get_vec<RsrcT>().emplace_back(new_resource->second.get()); 
			};

			template<typename RsrcT>
			std::string make_resource_name(const std::string& custom_name)
			{
				std::string new_name = custom_name.empty() ? "_" : custom_name;
				std::size_t dup = 1;

				if (!get_map<RsrcT>().contains(new_name)) return new_name;
				else
				{
					while (get_map<RsrcT>().contains(new_name + "_" + std::to_string(dup)))
						dup++;
					return {new_name += "_" + std::to_string(dup)};
				}
			};
		};
	};
};
#endif
