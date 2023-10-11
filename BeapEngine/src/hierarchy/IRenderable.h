#pragma once

class IRenderable {
public:
	virtual ~IRenderable() = default;
	virtual void Draw() = 0;
};