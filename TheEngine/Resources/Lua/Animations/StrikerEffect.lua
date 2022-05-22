strikerFX = {}

strikerFX.default = {}
strikerFX.default.startTime = 0.0
strikerFX.default.endTime = 0.42
strikerFX.default.rate = 0.7

function GetStrikerFXVar(animation, data)
	return strikerFX[animation][data]
end