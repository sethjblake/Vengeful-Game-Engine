PlayerManager = {
	player = nil,
	rigidbody = nil,
	lives = 1,

	OnStart = function(self)
		self:SpawnPlayer()
	end,

	SpawnPlayer = function(self)
		if self.player ~= nil then
			return
		end
		self.player = Actor.Instantiate("Mario")
		self.rigidbody = self.player:GetComponent("Rigidbody")
	end,

	OnUpdate = function(self)
		
		if self.rigidbody == nil then
			return
		end

		if self.rigidbody.y > 1.6 then
			self:KillPlayer()
		end
		Text.Draw("" .. self.lives, 10, 10, "NotoSans-Regular", 12, 255, 255, 255, 255)
	end,

	KillPlayer = function(self)
		self.rigidbody.x = 0
		self.rigidbody.y = 0
		self.lives = self.lives - 1
		if self.lives == 0 then
			Scene.Load("level_select")
		end
	end
}

