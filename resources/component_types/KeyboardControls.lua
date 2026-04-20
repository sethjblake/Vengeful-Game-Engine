KeyboardControls = {
	speed = 3,
	jump_power = 100,
	maxSpeed = 1,

	OnStart = function(self)
		self.rb = self.actor:GetComponent("Rigidbody")
	end,

	CheckForBlocks = function(self)
		local hitResult = Physics.Raycast(self.rb:GetPosition(), Vector2(0, -1), 0.08)

		if hitResult ~= nil then
			local actor = hitResult.actor

			local qb = actor:GetComponent("QuestionBlock")

			if qb ~= nil then
				local obj = {}
				local id = actor:GetID()
				table.insert(obj, id)
				Event.Publish("UpwardRaycast", obj)
			end
		end
	end,

	OnUpdate = function(self)

		self:CheckForBlocks()

		-- Horizontal
		local horizontal_input = 0
		if Input.GetKey("right") then
			horizontal_input = self.speed
		end

		if Input.GetKey("left") then
			horizontal_input = -self.speed
		end

		-- Vertical
		local vertical_input = 0

		-- Check on ground
		local on_ground = false
		ground_object = Physics.Raycast(self.rb:GetPosition(), Vector2(0, 1), 0.1)
		if Input.GetKeyDown("up") or Input.GetKeyDown("space") then
			if ground_object ~= nil then
				vertical_input = -self.jump_power
				local eventObject = {}
				table.insert(eventObject, "jump_up")
				Event.Publish("soundEffect", eventObject)
			end
		end

		self.rb:SetRotation(0)

		local velocity = self.rb:GetVelocity()

		if velocity.x < self.maxSpeed then
			if velocity.x > -1 * self.maxSpeed then
				self.rb:AddForce(Vector2(horizontal_input, 0))
			end
		end

		if velocity.y < 0.2 then
			if velocity.y > -0.2 then
				self.rb:AddForce(Vector2(0, vertical_input))
			end
		end

		if velocity.x > self.maxSpeed then
			velocity.x = self.maxSpeed
		end
		if velocity.x < -1 * self.maxSpeed then
			velocity.x = -1 * self.maxSpeed
		end

		self.rb:SetVelocity(velocity)
	end
}

