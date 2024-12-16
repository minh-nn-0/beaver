local geo = {}
function geo.has_intersect(A, B)
	if A.w and A.h then
		-- rect and rect
		return A.x < B.x + B.w and
			A.x + A.w > B.x and
			A.y < B.y + B.h and
			A.y + A.h > B.y
	else
		-- point and rect
		return A.x > B.x and
			A.x < B.x + B.w and
			A.y > B.y and
			A.y < B.y + B.w
	end
end

function geo.circle_rectangle_colliding(C, R)
	local testx = C.x
	local testy = C.y
	if C.x < R.x then testx = R.x
	elseif C.x > R.x + R.w then
		testx = R.x + R.w
	end

	if C.y < R.y then testy = R.y
	elseif C.y > R.y + R.h then
		testy = R.y + R.h
	end

	local distance = math.sqrt((C.x - testx) ^ 2 + (C.y - testy) ^ 2)

	if distance < C.r then return true end
	return false
end

function geo.rect_at(id, tilesize, numx, numy)
	local divquote = math.floor(id / numx)
	local divrem = id % numx
	if divquote > numy then error("logic error, exceeded height of tile source" .. divquote .. "," .. tilesize .. numx .. numy) end

	return {
		x = divrem * tilesize,
		y = divquote * tilesize,
		w = tilesize,
		h = tilesize
	}
end

return geo
