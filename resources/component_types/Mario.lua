Mario = {
	lives = 3,
	rigidbody = nil,
	spriteRenderer = nil,

	OnStart = function(self)
		Debug.Log("Spawning Mario")
		self.spriteRenderer = self.actor:AddComponent("SpriteRenderer")
		self.spriteRenderer:SetSprite("mario_idle")
		self.rigidbody = self.actor:GetComponent("Rigidbody")
	end,

	OnUpdate = function(self)
		local position = self.rigidbody:GetPosition()
		self.spriteRenderer:SetPosition(position.x, position.y)
	end
}

