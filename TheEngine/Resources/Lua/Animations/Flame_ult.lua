flameUlt = {}

flameUlt.default = {}
flameUlt.default.startTime = 0.0416
flameUlt.default.endTime = 0.4166
flameUlt.default.rate = 1.0

flameUlt.idle = {}
flameUlt.idle.startTime = 0.0416
flameUlt.idle.endTime = 0.4166
flameUlt.idle.rate = 1.0

function GetFlameUltVar(animation, data)
	return flameUlt[animation][data]
end