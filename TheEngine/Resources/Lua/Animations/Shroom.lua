shroom = {}

-------largeExplosion---------------------

shroom.default = {}
shroom.default.startTime = 0.0416
shroom.default.endTime = 0.5333
shroom.default.rate = 1.0

function GetShroomVar(animation, data)
	return shroom[animation][data]
end