CameraManager = {
	ease_factor = 0.3,
	tracking_player = false,
	maxLeft = -0.64,

	OnUpdate = function(self)
		local player_actor = Actor.Find("Mario")
		
		if player_actor == nil then
			return
		end

		local player_rb = player_actor:GetComponent("Rigidbody")
		local desired_position = player_rb:GetPosition()

		Camera.SetPosition(desired_position.x * 3, 0)

	end
}

