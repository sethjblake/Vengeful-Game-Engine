Background = {
	
	backgroundSpriteRenderer = nil,

	OnStart = function(self)
		self.backgroundSpriteRenderer = self.actor:AddComponent("SpriteRenderer")
		self.backgroundSpriteRenderer:SetSprite("Background")
	end

}

