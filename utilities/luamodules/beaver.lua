local beaver = {}

--- @param path string
--- @param custom_name? string
function beaver.new_image(path, custom_name)
	NEW_IMAGE(path, custom_name or "")
end

--- @param path string
--- @param custom_name? string
function beaver.new_sound(path, custom_name)
	NEW_SOUND(path, custom_name or "")
end

--- @param path string
--- @param custom_name? string
function beaver.new_music(path, custom_name)
	NEW_MUSIC(path, custom_name or "")
end

--- @param path string
--- @param fontsize integer
--- @param custom_name? string
function beaver.new_font(path, fontsize, custom_name)
	NEW_FONT(path, fontsize, custom_name or "")
end

--- @param keyname string
function beaver.get_input(keyname)
	return GET_INPUT(keyname)
end

function beaver.get_elapsed_time()
	return GET_ELAPSED_TIME()
end

--- @param x number
--- @param y number
function beaver.draw_point(x,y)
	DRAW_POINT(x,y)
end
--- @param x1 number
--- @param y1 number
--- @param x2 number
--- @param y2 number
function beaver.draw_line(x1,y1,x2,y2)
	DRAW_LINE(x1,y1,x2,y2)
end

--- @param x number
--- @param y number
--- @param w number
--- @param h number
--- @param filled boolean
function beaver.draw_rectangle(x,y,w,h,filled)
	DRAW_RECTANGLE(x,y,w,h,filled)
end

--- @param x number
--- @param y number
--- @param r number
--- @param filled boolean
function beaver.draw_circle(x,y,r,filled)
	DRAW_CIRCLE(x,y,r,filled)
end

--- @param texture_name string
--- @param param? table
function beaver.draw_texture(texture_name, param)
	DRAW_TEXTURE(texture_name, param or {})
end

function beaver.draw_text(x, y, fontname, content, wraplength, blended)
	DRAW_TEXT(x,y,fontname,content,wraplength or 0, blended or false)
end
-- @param r integer
-- @param g integer
-- @param b integer
-- @param a integer
function beaver.set_draw_color(r,g,b,a)
	SET_DRAW_COLOR(r,g,b,a)
end

function beaver.clear()
	CLS()
end

--- @param x integer
--- @param y integer
function beaver.set_scale(x,y)
	SET_SCALE(x,y)
end

function beaver.set_render_logical_size(x,y)
	SET_RENDER_LOGICAL_SIZE(x,y)
end

function beaver.get_render_logical_size(x,y)
	GET_RENDER_LOGICAL_SCALE(x,y)
end

function beaver.set_fullscreen(fc)
	SET_FULLSCREEN(fc)
end

function beaver.get_image_size(name)
	return IMAGE_SIZE(name)
end

function beaver.play_sound(name, channel, loop)
	PLAY_SOUND(name, channel or -1, loop or 0)
end
function beaver.play_music(name, loop)
	PLAY_MUSIC(name, loop or -1)
end

function beaver.set_volume_master(volume)
	SET_VOLUME_MASTER(volume)
end
function beaver.set_volume_music(volume)
	SET_VOLUME_MUSIC(volume)
end
function beaver.set_volume_sound(name, volume)
	SET_VOLUME_SOUND(name, volume)
end
function beaver.set_volume_channel(channel, volume)
	SET_VOLUME_CHANNEL(channel, volume)
end
return beaver
