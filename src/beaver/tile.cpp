#include <beaver/tile.hpp>

using namespace beaver::tile;

// Tiled store tintcolor as #aarrggbb, convert it to #rrggbbaa
constexpr auto correct_format_tiledcolor = [](const std::string& c) 
{return c.substr(3) + c.substr(1,2); };

drawdata ddata_from_json(const nlohmann::json& j)
{
	drawdata rs;

	if (j.count("parallaxx") > 0) rs._parallax.x = j.at("parallaxx");
	if (j.count("parallaxy") > 0) rs._parallax.y = j.at("parallaxy");

	if (j.count("offsetx") > 0) rs._offset.x = j.at("offsetx");
	if (j.count("offsety") > 0) rs._offset.y = j.at("offsety");
	
	if (j.count("tintcolor") > 0)
	{
		std::string tint_hex = j.at("tintcolor");
		if (tint_hex.length() == 9) tint_hex = correct_format_tiledcolor(tint_hex);
		rs._tint = utils::hex_to_rgba<unsigned char>(tint_hex);
	};
	
	if (j.count("opacity") > 0) rs._tint[3] = static_cast<float>(j.at("opacity")) * 255;

	return rs;
};

std::string loadlayer(const nlohmann::json& tmj, const std::string& parent_name, tilemap::layer_manager& layers)
{
	layer_t rs;
	rs._drawdata = ddata_from_json(tmj);
	rs._visible = tmj.at("visible");
	
	std::string lname = parent_name.empty() ? static_cast<std::string>(tmj.at("name"))
			: parent_name + "." + static_cast<std::string>(tmj.at("name"));
	std::string ltype = tmj.at("type");
	if (ltype == "tilelayer")
	{
		tilelayer tl;
		std::ranges::transform(static_cast<std::vector<long>>(tmj.at("data")),
								std::back_inserter(tl._data),
								[](long id){return --id;});
		rs._data = tl;
		layers.second.emplace_back(rs);
		layers.first.emplace(lname, layers.second.size() - 1);
	}
	else if (ltype == "imagelayer")
	{
		image_layer il;
		il._image_name = std::filesystem::path{tmj.at("image")}.filename();
		il._position = {tmj.count("offsetx") > 0 ? static_cast<float>(tmj.at("offsetx")) : 0,
						tmj.count("offsety") > 0 ? static_cast<float>(tmj.at("offsety")) : 0};

		rs._data = il;
		layers.second.emplace_back(rs);
		layers.first.emplace(lname, layers.second.size() - 1);
	}
	else if (ltype == "group")
	{
		group gr;
		for (auto& gr_layer: tmj.at("layers")) 
			if (gr_layer.at("type") == "tilelayer" || gr_layer.at("type") == "imagelayer" ||
					gr_layer.at("type") == "group")
				gr._layers.push_back(loadlayer(gr_layer, lname, layers));
		rs._data = gr;
		layers.second.emplace_back(rs);
		layers.first.emplace(lname, layers.second.size() - 1);
	};

	return lname;
};

auto print_drawdata = [](auto&& arg)
{
	auto [prll, offs, tint] = arg._drawdata;
	std::stringstream o;
	o << "parallax: " << prll << ", " 
		<< "offset: " << offs << ", "
		<< "color: " << +tint[0] << ", " << +tint[1] << ", " << +tint[2] << ", " << +tint[3];
	return o.str();
};


void printlayers(const tilemap::layer_manager& layers) {
    static int level = 0;

    for (size_t i = 0; i < layers.second.size(); ++i) {
        const auto& layer = layers.second[i];

        // Reverse lookup for the layer name
        std::string lname;
        for (const auto& [name, index] : layers.first) {
            if (index == i) {
                lname = name;
                break;
            }
        }

        if (lname.empty()) {
            std::cerr << "Error: Layer name not found for index " << i << '\n';
            continue;
        }

        if (std::holds_alternative<tilelayer>(layer._data)) {
            std::cout << std::string(level, '\t') 
                      << lname << '\t' 
                      << "type = tilelayer\t" 
                      << print_drawdata(layer) 
                      << '\n';
        }

        if (std::holds_alternative<image_layer>(layer._data)) {
            std::cout << std::string(level, '\t') 
                      << lname << '\t' 
                      << "type = image\t" 
                      << print_drawdata(layer) << '\t'
					  << std::get<image_layer>(layer._data)._image_name
                      << '\n';
        }
        if (std::holds_alternative<group>(layer._data)) {
            std::cout << std::string(level, '\t') 
                      << lname << '\t' 
                      << "type = group\t" 
                      << print_drawdata(layer) 
                      << '\n';
			std::println("gr layer");
			for (auto& l: std::get<group>(layer._data)._layers)
				std::print("{} ", l);
			std::println();
        }
    }
}

beaver::tile::tilemap::tilemap(const std::filesystem::path& path)
{
	std::ifstream f (path);
	nlohmann::json tmj;
	f >> tmj;
	f.close();
	_tilesize = tmj.at("tilewidth");
	_numx = static_cast<int>(tmj.at("width"));
	_numy = static_cast<int>(tmj.at("height"));


	// PROPERTIES
	//_properties = get_properties(tmj);

	// TILESET
	for (auto& ts: tmj.at("tilesets"))
	{
		nlohmann::json tsj;
		f.open(path.parent_path() / std::filesystem::path(ts.at("source")).filename());
		f >> tsj;
		std::println("{}",std::filesystem::path(tsj.at("image")).filename().string());
		_tilesets.emplace(static_cast<int>(ts["firstgid"]) - 1, 
						tileset{._filename = std::filesystem::path(tsj.at("image")).filename().string(),
								._tilesize = tsj["tilewidth"],
								._numx = static_cast<int>(tsj.at("imagewidth"))
										/static_cast<int>(tsj.at("tilewidth")),
								._numy = static_cast<int>(tsj.at("imageheight"))
										/static_cast<int>(tsj.at("tilewidth"))});
	};

	if (tmj.count("backgroundcolor") > 0) 
		_bgcolor = utils::hex_to_rgba<unsigned char>(correct_format_tiledcolor(tmj.at("backgroundcolor")));

	// LAYERS
	for (auto& layer: tmj.at("layers")) 
		loadlayer(layer, "", _layers);

	printlayers(_layers);
};

void beaver::tile::load_textures(tilemap& tm, std::vector<sdl::texture*>& textures)
{
	auto find_texture = [&](const std::string& texture_name) -> int
	{
		if (auto find_rs = std::ranges::find_if(textures, [&](auto&& tex)
				{ return tex->_name == texture_name; });
				find_rs != textures.end())
			return std::distance(textures.begin(), find_rs);
		else return -1;
		
	};
	//tileset
	for (auto& [_, ts]: tm._tilesets)
		ts._textureid = find_texture(ts._filename);
	//images
	
	for (auto& layer: tm._layers.second | std::views::filter([](auto& layer){ return std::holds_alternative<image_layer>(layer._data);}))
	{
		auto& il = std::get<image_layer>(layer._data);
		il._textureid = find_texture(il._image_name);
	};
};
