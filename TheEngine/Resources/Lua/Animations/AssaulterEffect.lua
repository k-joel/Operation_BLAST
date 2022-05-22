assaulterFX = {}

assaulterFX.default = {}
assaulterFX.default.startTime = 0.0
assaulterFX.default.endTime = 0.42
assaulterFX.default.rate = 0.7

function GetAssaulterFXVar(animation, data)
	return assaulterFX[animation][data]
end