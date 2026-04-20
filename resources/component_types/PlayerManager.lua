PlayerManager = {
	player = nil,
	rigidbody = nil,

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
		if self.rigidbody.position.y > 1.6 then
			self:KillPlayer()
		end
	end,

	KillPlayer = function(self)
		Actor.Destroy(self.player)
		self.rigidbody = nil
		self.player = nil
		self:SpawnPlayer()
	end
}

