#include <tiledwrapper/tiledwrapper.hpp>
#include <fstream>
#include <utilities.hpp>

//tileset::tileset(const std::filesystem::path& p)
//{
//	std::ifstream f{p};
//	nlohmann::json tsj;
//	f >> tsj;
//	try
//	{
//		_tilesize = tsj.at("tilewidth");
//		_numx = static_cast<int>(tsj.at("imagewidth")) / _tilesize;
//		_numy = static_cast<int>(tsj.at("imageheight")) / _tilesize;
//		std::cout << "loadts\n" << _tilesize << ", " << _numx << ", " << _numy << '\n';
//		_src._t = SDL_CreateTextureFromSurface(rdr,
//					IMG_Load(std::string("assets" / std::filesystem::path(tsj.at("image")).filename()).c_str()));
//	}
//	catch (nlohmann::json::out_of_range e) {};
//};

// Tiled store tintcolor as #aarrggbb, convert it to #rrggbbaa
constexpr auto correct_format_tiledcolor = [](const std::string& c) 
{return c.substr(3) + c.substr(1,2); };

tiled::drawdata::drawdata(const nlohmann::json& j)
{
	if (j.count("opacity") > 0) _opacity = j.at("opacity");

	if (j.count("parallaxx") > 0) _parallax.x = j.at("parallaxx");
	if (j.count("parallaxy") > 0) _parallax.y = j.at("parallaxy");

	if (j.count("offsetx") > 0) _offset.x = j.at("offsetx");
	if (j.count("offsety") > 0) _offset.y = j.at("offsety");
	
	if (j.count("tintcolor") > 0)
	{
		std::string tint_hex = j.at("tintcolor");
		if (tint_hex.length() == 9) tint_hex = correct_format_tiledcolor(tint_hex);
		_tint = utils::normalize_rgba(utils::hex_to_rgba(tint_hex));
	};
}

//object::object(const nlohmann::json& j) : _drawdata(j)
//{
//	_name = j.at("name");
//	
//	Eigen::Vector2f min {static_cast<float>(j["x"]), static_cast<float>(j["y"])}, 
//							diagonal {0,0};
//			
//	if (j.count("width") > 0) 	diagonal[0] = j["width"];
//	if (j.count("height") > 0)	diagonal[1] = j["height"];
//	
//	// Tiled store rect xy at topleft (except for tile object ;) )
//	min[1] -= diagonal[1];
//
//	if (j.count("gid") > 0)
//	{
//		_tileid = j.at("gid");
//		min[1] += diagonal[1];
//	};
//
//	_rect = {min, min + diagonal};
//};

tiled::layer loadlayer(const nlohmann::json& tmj, int tilesize)
{
	tiled::layer rs;
	std::string lname = tmj.at("name");
	std::string ltype = tmj.at("type");
	tiled::drawdata ldrawdata {tmj};
	bool lvisible = tmj.at("visible");
	
	if (ltype == "tilelayer")
	{
		tiled::tilelayer tl {lname, ldrawdata, lvisible, {}, tilesize, tmj.at("width"), tmj.at("height")};
		std::ranges::transform(static_cast<std::vector<long>>(tmj.at("data")),
								std::back_inserter(tl._data),
								[](long id){return --id;});
		rs = tl;
	}
	else if (ltype == "objectgroup")
	{
		tiled::objectlayer objlayer {lname, ldrawdata, lvisible};
		for (auto& obj: tmj.at("objects"))
		{
			// point
			if (obj.count("point") > 0) {}
			else
			{
				std::string objname {obj.at("name")};
				tiled::rect rect { 	{obj.at("x"), obj.at("y")},
									{obj.at("width"), obj.at("height")}};

				nlohmann::json props;
				if (obj.count("properties") > 0)
				{
					for (auto& prop: obj.at("properties"))
					{
						props[prop.at("name")] = prop.at("value");
						std::cout << prop.at("name") << " " << prop.at("value") << '\n';
					};
				};

				objlayer._objects.emplace_back(objname, rect, props);
			};
		};

		rs = objlayer;
	}
	else if (ltype == "group")
	{
		tiled::grouplayer gr {lname, ldrawdata, lvisible};
		
		for (auto& gr_layer: tmj.at("layers")) gr._layers.push_back(loadlayer(gr_layer, tilesize));
		
		rs = gr;
	};

	return rs;
};

auto print_drawdata = [](auto&& arg)
{
	auto [opct, prll, offs, tint] = arg._drawdata;
	std::stringstream o;
	o << "opacity: " << opct << ", " 
		<< "parallax: " << prll << ", " 
		<< "offset: " << offs << ", " ;
	return o.str();
};


void printlayers (const tiled::layer& layer)
{
	static int level = 0;

	if (std::holds_alternative<tiled::tilelayer>(layer))
	{
		auto& ngroup = std::get<tiled::tilelayer>(layer);
		std::cout << std::string(level, '\t')
			<< ngroup._name << '\n'
			<< print_drawdata(ngroup) << '\n';
	};

	if (std::holds_alternative<tiled::grouplayer>(layer))
	{
		auto& gr = std::get<tiled::grouplayer>(layer);
		std::cout << std::string(level, '\t') << gr._name << '\n'
			<< print_drawdata(gr) << '\n';
		for (auto& gr_layer: gr._layers)
			printlayers(gr_layer);
	};

};

tiled::tilemap::tilemap(const std::filesystem::path& p)
{
	std::ifstream f {p};
	nlohmann::json tmj;
	f >> tmj;
	f.close();

	_tilesize = tmj.at("tilewidth");
	_numx = static_cast<int>(tmj.at("width"));
	_numy = static_cast<int>(tmj.at("height"));
	
	for (auto& ts: tmj.at("tilesets"))
	{
		std::cout << "ts\n";
		
		nlohmann::json tsj;
		f.open(p.parent_path() / ts.at("source"));
		f >> tsj;
		_tilesets.emplace(static_cast<int>(ts["firstgid"]) - 1, 
						tiled::tileset{._img = std::filesystem::path(tsj.at("image")).filename(),
										._tilesize = tsj["tilewidth"],
										._numx = static_cast<int>(tsj.at("imagewidth"))
												/static_cast<int>(tsj.at("tilewidth")),
										._numy = static_cast<int>(tsj.at("imageheight"))
												/static_cast<int>(tsj.at("tilewidth"))});
	};

	if (tmj.count("backgroundcolor") > 0) 
		_bgcolor = utils::hex_to_rgba(correct_format_tiledcolor(tmj.at("backgroundcolor")));

	// LAYERS
	for (auto& layer: tmj.at("layers")) 
		_layersdata.push_back(loadlayer(layer, _tilesize));
	
	//for (auto& layer: _layersdata)
	//	printlayers(layer);
};


//void drawlayers(const layer& l, const mg::SDL_wrapper& sdl, const tilemap& map, const mg::Camera2D cam) 
//{
//	auto draw_tilelayer = [&](const tilelayer& tl)
//	{
//		const auto& [opacity, parallax, offset, tint] = tl._drawdata;
//		for (int i = 0; i != tl._data.size(); i++)
//		{
//			if (int tile = tl._data[i]; tile >= 0)
//			{
//				auto& ts = std::ranges::find_if(map._tilesets | std::views::reverse,[&](auto&& ts){return ts.first <= tile;})->second;
//				SDL_SetTextureAlphaMod(ts._src, opacity*255);
//				
//				auto [flipflag, tileid] = get_flipflags(tile);
//				mg::frect dst = mg::rect_at(i, map);
//
//				dst._pos.x -= cam._view._pos.x * parallax.x - offset.x;
//				dst._pos.y -= cam._view._pos.y * parallax.y - offset.y;
//				
//				sdl.draw(ts._src, mg::rect_at(tile, ts), dst, flipflag);
//
//				SDL_SetTextureAlphaMod(ts._src, 255);
//			}
//		}
//	};
//	
//	
//	if (std::holds_alternative<tilelayer>(l))
//		draw_tilelayer(std::get<tilelayer>(l));
//
//	if (std::holds_alternative<grouplayer>(l))
//		for (auto& gr_l: std::get<grouplayer>(l)._layers)
//			drawlayers(gr_l, sdl, map, cam);
//};
//
//
//void tilemap::draw(const mg::SDL_wrapper& sdl, const mg::Camera2D& cam)
//{
//	for (const auto& l: _layersdata)
//		drawlayers(l, sdl, *this, cam);
//}
