bossParts = {}

bossParts.default = {}
bossParts.default.startTime = 0.0416
bossParts.default.endTime = 1.6
bossParts.default.rate = 1.0

bossParts.idle = {}
bossParts.idle.startTime = 0.0416
bossParts.idle.endTime = 1.6
bossParts.idle.rate = 1.0

------------Conveyer------------
bossParts.conveyerIdle = {}
bossParts.conveyerIdle.startTime = 0.0832
bossParts.conveyerIdle.endTime = 2.0416
bossParts.conveyerIdle.rate = 1.0


------------Large Cannon------------
bossParts.largeCannonIdle = {}
bossParts.largeCannonIdle.startTime = 0.0832
bossParts.largeCannonIdle.endTime = 1.0416
bossParts.largeCannonIdle.rate = 1.0

bossParts.largeCannonShoot = {}
bossParts.largeCannonShoot.startTime = 1.0832
bossParts.largeCannonShoot.endTime = 1.7916
bossParts.largeCannonShoot.rate = 2.0

------------Top Cannon------------
bossParts.topCannonCharge = {}
bossParts.topCannonCharge.startTime = 0.0832
bossParts.topCannonCharge.endTime = 1.0416
bossParts.topCannonCharge.rate = 0.25

bossParts.topCannonShoot = {}
bossParts.topCannonShoot.startTime = 1.0832
bossParts.topCannonShoot.endTime = 2.0416
bossParts.topCannonShoot.rate = 0.25

bossParts.topCannonIdle = {}
bossParts.topCannonIdle.startTime = 2.0833
bossParts.topCannonIdle.endTime = 2.999999
bossParts.topCannonIdle.rate = 1.0

------------Bottom Cannon------------
bossParts.bottomCannonCharge = {}
bossParts.bottomCannonCharge.startTime = 0.0833
bossParts.bottomCannonCharge.endTime = 1.0416
bossParts.bottomCannonCharge.rate = 0.25

bossParts.bottomCannonShoot = {}
bossParts.bottomCannonShoot.startTime = 1.0832
bossParts.bottomCannonShoot.endTime = 2.0416
bossParts.bottomCannonShoot.rate = 0.25

bossParts.bottomCannonIdle = {}
bossParts.bottomCannonIdle.startTime = 2.0833
bossParts.bottomCannonIdle.endTime = 2.999999
bossParts.bottomCannonIdle.rate = 1.0

function GetBossPartsVar(animation, data)
	return bossParts[animation][data]
end