crate = {}

crate.default = {}
crate.default.startTime = 0.0416
crate.default.endTime = 0.93
crate.default.rate = 1.0

----------crateExplosion-------------------------

crate.damage = {}
crate.damage.startTime = 0.0416
crate.damage.endTime = 0.625
crate.damage.rate = 1.0

crate.explode = {}
crate.explode.startTime = 0.625
crate.explode.endTime = 0.93
crate.explode.rate = 1.0

function GetCrateVar(animation, data)
	return crate[animation][data]
end