flameBurst = {}

flameBurst.default = {}
flameBurst.default.startTime = 0.0416
flameBurst.default.endTime = 1.083
flameBurst.default.rate = 1.0

flameBurst.idle = {}
flameBurst.idle.startTime = 0.0416
flameBurst.idle.endTime = 1.083
flameBurst.idle.rate = 1.0

flameBurst.grow = {}
flameBurst.grow.startTime = 0.0416
flameBurst.grow.endTime = 0.4583
flameBurst.grow.rate = 1.0

flameBurst.live = {}
flameBurst.live.startTime = 0.4583
flameBurst.live.endTime = 0.9166
flameBurst.live.rate = 1.0

flameBurst.die = {}
flameBurst.die.startTime = 0.9166
flameBurst.die.endTime = 1.083
flameBurst.die.rate = 1.0

function GetFlameBurstVar(animation, data)
	return flameBurst[animation][data]
end