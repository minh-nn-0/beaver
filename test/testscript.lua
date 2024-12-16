local beaver = require "beaver"
function LOAD()
	print("LOADDDDD")
	beaver.new_image("/home/minhmacg/projects/videogames/madewithlove/space_shooter/assets/spritesheet.png")
end

function DRAW()
	beaver.draw_circle(10,20,40,true);
	beaver.draw_texture("spritesheet.png")
	beaver.draw_texture("spritesheet.png", {
							dst = {x = 50, y = 50, w = 50, h = 50},
							src = {x = 0, y = 0, w = 16, h = 16},
							angle = 30
						})
end
