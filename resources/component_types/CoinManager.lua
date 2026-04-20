CoinManager = {
	coinCount = 0,

	OnStart = function(self)
		Event.Subscribe("GainCoin", self, self.AddCoin)
		local savedCount = Progress.GetInt("coin_count")
		if savedCount ~= nil then
			self.coinCount = savedCount["coin_count"]
		else
			Debug.Log("error: coin_count returned nil")
		end
	end,

	AddCoin = function(self, eventObject)
		self.coinCount = self.coinCount + 1
		Progress.SetInt("coin_count", self.coinCount)

		if self.coinCount % 3 == 0 then
			Progress.Save()
		end

	end,

	OnUpdate = function(self)
		Text.Draw("" .. self.coinCount, 10, 10, "NotoSans-Regular", 12, 255, 255, 255, 255)
	end
}

