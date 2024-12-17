local beaver = require "beaver"
function LOAD()
	print("LOADDDDD")
	rx = 0
	ry = 30
end

function UPDATE(dt)
	rx = rx + dy
end
function DRAW()
	beaver.draw_circle(10,20,40,true);
	beaver.draw_rectangle(rx, ry, 40,40, true)
end
