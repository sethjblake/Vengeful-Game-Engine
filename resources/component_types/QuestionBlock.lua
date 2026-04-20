QuestionBlock = {
	spriteRenderer = nil,
	rigidbody = nil,
	trigger = nil,
	position = nil,
	frameCooldown = 0,

	SetPosition = function(self, pos)
		self.position = pos
	end,

	OnHit = function(self, eventObject)
		if self.frameCooldown > 0 then
			return
		end
		local id = eventObject[1]
		if id ~= nil then
			if id == self.actor:GetID() then
				Debug.Log("I was hit")
				Event.Publish("GainCoin", {})
				self.frameCooldown = 60
			end
		end
	end,

	OnStart = function(self)
		Event.Subscribe("UpwardRaycast", self, self.OnHit)

		self.spriteRenderer = self.actor:AddComponent("SpriteRenderer")

		if self.spriteRenderer == nil then
			Debug.Log("Error adding SpriteRender component to Question Block")
			return
		end

		self.spriteRenderer:SetSprite("question_block")
		self.spriteRenderer:SetPosition(self.position.x, self.position.y)

		self.rigidbody = self.actor:GetComponentByKey("1_collider")

		if self.rigidbody == nil then
			Debug.Log("self.rigidbody is nil (QuesitonBlock.lua)")
			return
		end

		self.rigidbody:SetPosition(self.position);

	end,

	OnUpdate = function(self)
		if self.frameCooldown > 0 then
			self.frameCooldown = self.frameCooldown - 1
		end
	end

}

