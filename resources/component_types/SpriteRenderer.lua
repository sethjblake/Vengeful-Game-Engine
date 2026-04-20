SpriteRenderer = {
	sprite = "???",
	r = 255,
	g = 255,
	b = 255,
	a = 255,
	sorting_order = 0,
	position = nil,
	rotation_degrees = 0,
	scale = 1,

	SetSprite = function(self, name)
		self.sprite = name
	end,

	SetPosition = function(self, x, y)
		if self.position == nil then
			self.position = Vector2(0,0)
		end
		self.position.x = x
		self.position.y = y
	end,

	OnStart = function(self)
		if self.position == nil then
			self.position = Vector2(0,0)
		end
	end,

	OnUpdate = function(self)
		Image.DrawEx(self.sprite, self.position.x, self.position.y, self.rotation_degrees, self.scale, self.scale, 0.5, 0.5, self.r, self.g, self.b, self.a, self.sorting_order)
	end
}

