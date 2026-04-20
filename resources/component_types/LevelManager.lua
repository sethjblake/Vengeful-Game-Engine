LevelManager = {
	selectedLevel = 1,
	currentLevel = 1,
	maxUnlockedLevel = 1,
	map = nil,
	miniMario = nil,
	buttons = {},

	OnStart = function(self)

		self.map = Actor.Instantiate("EmptySprite")
		local sr = self.map:GetComponent("SpriteRenderer")
		if sr ~= nil then
			sr.sprite = "map"
			sr.scale = 2
			sr:SetPosition(0, 0.16)
		end

		for i = 1, 4 do
			local newButton = Actor.Instantiate("EmptySprite")
			table.insert(self.buttons, newButton)
			self:UpdateButton(i)
		end

		self:InitButton(1, -1.14, 0.16)
		self:InitButton(2, -0.06, 0.16)
		self:InitButton(3, -0.06, -0.25)
		self:InitButton(4, 1, 0.16)


		local savedMaxLevel = Progress.GetInt("max_unlocked_level")
		if savedMaxLevel == nil then
			Progress.SetInt("max_unlocked_level", 1)
			self.maxUnlockedLevel = 1
		else
			Debug.Log("Max level " .. savedMaxLevel["max_unlocked_level"])
			self.maxUnlockedLevel = savedMaxLevel["max_unlocked_level"]
			for i = 2, self.maxUnlockedLevel do
				self:UnlockLevel(i)
			end
		end

		local savedCurrentLevel = Progress.GetInt("current_level")
		if savedCurrentLevel ~= nil then
			local temp = savedCurrentLevel["current_level"]
			if temp <= self.maxUnlockedLevel then
				savedCurrentLevel = savedCurrentLevel["current_level"]
			end
		end
		self.miniMario = Actor.Instantiate("EmptySprite")
		self:MarioToButton(self.currentLevel, self.currentLevel)
	end,

	InitButton = function(self, id, x, y)
		local sr = self.buttons[id]:GetComponent("SpriteRenderer")
		if sr ~= nil then
			sr:SetPosition(x, y)
			sr.scale = 2
		end
	end,

	UpdateButton = function(self, id)
		Debug.Log(self.maxUnlockedLevel)
		Debug.Log(id)
		local sr = self.buttons[id]:GetComponent("SpriteRenderer")
		if sr ~= nil then
			if self.maxUnlockedLevel >= id then
				if id == self.selectedLevel then
					sr.sprite = "button_pressed"
				else
					sr.sprite = "button_unlocked"
				end
			else
				sr.sprite = "button_locked"
			end
		end
		Debug.Log(sr.sprite)
	end,

	MarioToButton = function(self, buttonIDStart, buttonIDEnd)
		local sr = self.miniMario:GetComponent("SpriteRenderer")
		if sr ~= nil then
			sr.sprite = "mario_idle"
			local buttonStartSpriteRenderer = self.buttons[buttonIDStart]:GetComponent("SpriteRenderer")
			local buttonEndSpriteRenderer = self.buttons[buttonIDEnd]:GetComponent("SpriteRenderer")

			if buttonStartSpriteRenderer ~= nil then
				buttonStartSpriteRenderer.sprite = "button_unlocked"
				buttonStartSpriteRenderer.position.y = buttonStartSpriteRenderer.position.y - 0.01
			end

			if buttonEndSpriteRenderer ~= nil then
				sr:SetPosition(buttonEndSpriteRenderer.position.x, buttonEndSpriteRenderer.position.y - 0.08)
				buttonEndSpriteRenderer.sprite = "button_pressed"
				buttonEndSpriteRenderer.position.y = buttonEndSpriteRenderer.position.y + 0.01
			end
		end
	end,

	UnlockLevel = function(self, levelID)
		if levelID > 4 or levelID < 1 then
			return
		end

		local button = self.buttons[levelID]
		local sr = button:GetComponent("SpriteRenderer")
		if sr ~= nil then
			sr.sprite = "button_unlocked"
		end

		Debug.Log("Level ID to unlock is " .. levelID)
		Debug.Log("Max level unlocked is " .. self.maxUnlockedLevel)
		self.max_unlocked_level = levelID
		Progress.SetInt("max_unlocked_level", levelID)

	end,

	OnUpdate = function(self)
		Text.Draw("PRESS E TO CHOOSE LEVEL", 10, 10, "NotoSans-Regular", 8, 255, 255, 255, 255)
		Text.Draw("PRESS S TO SAVE", 10, 20, "NotoSans-Regular", 8, 255, 255, 255, 255)

		if Input.GetKeyDown("s") then
			Progress.Save()
			Debug.Log("Progress saved")
		end

		if Input.GetKeyDown("e") then
			Scene.Load("level_" .. self.selectedLevel)
		end

		if Input.GetKeyDown("space") then
			if self.maxUnlockedLevel == 1 then
				self:UnlockLevel(2)
				self:UnlockLevel(3)
				self.maxUnlockedLevel = 3
			elseif self.maxUnlockedLevel == 3 then
				self:UnlockLevel(4)
				self.maxUnlockedLevel = 4
			end
		end

		if Input.GetKeyDown("right") then
			if self.selectedLevel == 1 and self.maxUnlockedLevel >= 2 then
				self.selectedLevel = 2
				self:MarioToButton(1, 2)
			elseif self.selectedLevel == 2 and self.maxUnlockedLevel >= 4 then
				self.selectedLevel = 4
				self:MarioToButton(2, 4)
			end
		end

		if Input.GetKeyDown("left") then
			if self.selectedLevel == 2 then
				self.selectedLevel = 1
				self:MarioToButton(2, 1)

			elseif self.selectedLevel == 4 then
				self.selectedLevel = 2
				self:MarioToButton(4, 2)
			end
		end

		if Input.GetKeyDown("up") then
			if self.selectedLevel == 2 then
				self.selectedLevel = 3
			self:MarioToButton(2, 3)
			end
		end

		if Input.GetKeyDown("down") then
			if self.selectedLevel == 3 then
				self.selectedLevel = 2
				self:MarioToButton(3, 2)
			end
		end
	end
}

