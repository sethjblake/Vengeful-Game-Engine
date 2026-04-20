AudioManager = {
	OnStart = function(self)
		Audio.Play(0, "overworld", true)
		Event.Subscribe("soundEffect", self, self.PlaySoundEffect)
	end,

	PlaySoundEffect = function(self, eventObject)
		Audio.Play(1, eventObject[1], false)
	end
}
