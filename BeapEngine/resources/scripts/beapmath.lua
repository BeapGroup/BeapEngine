BeapEngine = {}
BeapEngine.Vector2 = {x = 0, y = 0}

function BeapEngine.Vector2:New(o)
	o = o or {}
	setmetatable(o, self)
	self.__index = self
	return o
end

function BeapEngine.Vector2:FromNum(a, b)
	return Vector2:New({x = a, y = b})
end

function BeapEngine.Vector2:Dot(other)
	return self.x * other.x + self.y * other.y
end

function BeapEngine.Vector2:__tostring()
    return "{"..self.x..","..self.y.."}"    
end

function BeapEngine.Vector2:__add(other)
	return Vector2:New({x = self.x + other.x, y = self.y + other.y})
end

function BeapEngine.Vector2:__sub(other)
	return Vector2:New({x = self.x - other.x, y = self.y - other.y})
end

function BeapEngine.Vector2:__mul(other)
	return Vector2:New({x = self.x * other.x, y = self.y * other.y})
end

function BeapEngine.Vector2:__div(other)
	return Vector2:New({x = self.x / other.x, y = self.y / other.y})
end