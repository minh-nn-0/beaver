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
		


		template<typename... RsrcTs>
		struct manager
		{
			template<typename T>
			using map_t = std::unordered_map<std::string, std::unique_ptr<T>>;
			template<typename T>
			using vec_t = std::vector<T*>;
			
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


		// actual data is stored in a vector. A map is used to map a name with the index
		template<typename... RsrcTs>
		struct manager_v2
		{
			using map_t = std::unordered_map<std::string, std::size_t>;

			template<typename T>
			using vec_t = std::vector<T>;

			template<typename T>
			using storage_t = std::pair<std::unordered_map<std::string, std::size_t>,
				  						std::vector<T>>;
			
			std::tuple<storage_t<RsrcTs>...> _storage;

			template<typename RsrcT>
			void add(const std::string& name, RsrcT&& arg = {})
			{
				get_vec<RsrcT>().emplace_back(std::forward<RsrcT>(arg));
			};
			template<typename RsrcT>
			RsrcT& get(const std::string& name) 
			{
				return get_vec<RsrcT>().at(get_map<RsrcT>().at(name));
			};

			template<typename RsrcT>
			const RsrcT& get(const std::string& name) const
			{
				return get_cvec<RsrcT>().at(get_cmap<RsrcT>().at(name));
			};

			template<typename RsrcT>
			map_t& get_map() {return std::get<storage_t<RsrcT>>(_storage).first;};
			
			template<typename RsrcT>
			const map_t& get_cmap() const {return std::get<storage_t<RsrcT>>(_storage).first;};
			
			template<typename RsrcT>
			vec_t<RsrcT>& get_vec() {return std::get<storage_t<RsrcT>>(_storage).second;};
			
			template<typename RsrcT>
			const vec_t<RsrcT>& get_cvec() const {return std::get<storage_t<RsrcT>>(_storage).second;};
			

		};
	};
};
#endif
