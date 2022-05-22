striker = {}

striker.default = {}
striker.default.startTime = 0.916
striker.default.endTime = 1.25
striker.default.rate = 0.75

striker.idle = {}
striker.idle.startTime = 0.916
striker.idle.endTime = 1.25
striker.idle.rate = 1.0

striker.walk = {}
striker.walk.startTime = 0.0833
striker.walk.endTime = 0.8
striker.walk.rate = 1.0

striker.die = {}
striker.die.startTime = 0.916
striker.die.endTime = 1.25
striker.die.rate = 0.5

striker.jump = {}
striker.jump.startTime = 1.2916
striker.jump.endTime = 2.08
striker.jump.rate = 1.0

function GetStrikerVar(animation, data)
	return striker[animation][data]
end