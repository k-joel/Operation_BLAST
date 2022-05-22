pop = {}

-------smallExplosion---------------------

pop.default = {}
pop.default.startTime = 0.0416
pop.default.endTime = 0.3333
pop.default.rate = 1.0

function GetPopVar(animation, data)
	return pop[animation][data]
end