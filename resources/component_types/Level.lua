Level = {
	audioManager = nil,
	playerManager = nil,
	cameraManager = nil,
	terrain = nil,

	OnStart = function(self)
		self.audioManager = Actor.Instantiate("AudioManager")
		self.playerManager = Actor.Instantiate("PlayerManager")
		self.cameraManager = Actor.Instantiate("CameraManager")
		self.terrain = Actor.Instantiate("TerrainManager")
	end
}
