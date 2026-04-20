ManagerSystemComponent = {
	audioManager = nil,
	levelManager = nil,
	playerManager = nil,
	cameraManager = nil,

	OnStart = function(self)
		self.audioManager = Actor.Instantiate("AudioManager")
		self.levelManager = Actor.Instantiate("LevelManager")
	end,

	LevelOne = function(self)
		self.playerManager = Actor.Instantiate("PlayerManager")
		self.cameraManager = Actor.Instantiate("CameraManager")
	end
}
