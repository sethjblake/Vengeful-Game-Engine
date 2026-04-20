TerrainManager = {
	
	box = nil,
	boxes = {},
	colliders = {},
	question_blocks = {},

	SpawnBox = function(self, x, y, type)
		local newBox = Actor.Instantiate("Box")
		local sr = newBox:GetComponent("SpriteRenderer")
		if sr ~= nil then
			sr:SetPosition(x, y)
			sr:SetSprite(type)
			table.insert(self.boxes, newBox)
		else
			Debug.Log("Error loading box of type " .. type)
		end
	end,

	SpawnCollider = function(self, x, y, width, height)
		local newCollider = Actor.Instantiate("ColliderObject")
		local rb = newCollider:GetComponent("Rigidbody")
		if rb ~= nil then
			rb:SetBodyType("kinematic")
			rb.x = x
			rb.y = y
			rb.width = width
			rb.height = height
			rb.friction = 0.7
			rb.has_trigger = false
			table.insert(self.colliders, newCollider)
		end
	end,

	OnStart = function(self)

		self:SpawnCollider(-1.92, 0, 0.16, 2)

		self:SpawnCollider(0, 0.64, 6.4, 0.16)

		local spawnX = -1.76
		for i = 1, 32 do
			local spawnY = 0.64
			self:SpawnBox(spawnX, spawnY, "grass")
			for j = 1, 2 do
				spawnY = spawnY + 0.16
				self:SpawnBox(spawnX, spawnY, "dirt")
			end
			spawnX = spawnX + 0.16
		end

		local question_block = Actor.Instantiate("QuestionBlock")

		local qb = question_block:GetComponent("QuestionBlock")

		if qb ~= nil then
			qb:SetPosition(Vector2(0.64, 0))
		end
	end

}

