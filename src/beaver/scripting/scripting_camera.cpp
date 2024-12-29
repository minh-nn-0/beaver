#include <beaver/scripting/scripting_camera.hpp>


void beaver::scripting::bind_camera(camera2D& cam, sol::table& tbl)
{
	tbl.set_function("set_cam_target", [&](float x, float y)
			{
				cam._target = {x, y};
			});
	tbl.set_function("get_cam_view", [&]() -> std::tuple<float,float,float,float>
			{
				return std::make_tuple(cam._view._pos.x, cam._view._pos.y, cam._view._size.x, cam._view._size.y);
			});
	tbl.set_function("get_cam_zoom", [&]()
			{
				return cam._zoom;
			});
	tbl.set_function("set_cam_zoom", [&](float zoom)
			{
				cam._zoom = zoom;
			});
	tbl.set_function("set_cam_smooth_speed", [&](float speed)
			{
				cam._smooth_speed = speed;
			});
	tbl.set_function("get_cam_smooth_speed", [&]()
			{
				return cam._smooth_speed;
			});
	tbl.set_function("set_cam_offset", [&](float x, float y)
			{
				cam._offset = {x,y};
			});
	tbl.set_function("get_cam_offset", [&]() -> std::pair<float,float>
			{
				return std::make_pair(cam._offset.x, cam._offset.y);
			});
};
