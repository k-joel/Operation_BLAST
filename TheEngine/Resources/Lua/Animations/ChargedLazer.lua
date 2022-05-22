lazer = {}

lazer.default = {}
lazer.default.startTime = 0.0416
lazer.default.endTime = 1.0
lazer.default.rate = 0.75

--------bigLazer-------------------

lazer.charge = {}
lazer.charge.startTime = 0.0416
lazer.charge.endTime = 0.5416
lazer.charge.rate = 1.0

lazer.sustain = {}
lazer.sustain.startTime = 0.5416
lazer.sustain.endTime = 0.7916
lazer.sustain.rate = 1.0

lazer.die = {}
lazer.die.startTime = 0.7916
lazer.die.endTime = 1.0
lazer.die.rate = 1.0

function GetLazerVar(animation, data)
	return lazer[animation][data]
end