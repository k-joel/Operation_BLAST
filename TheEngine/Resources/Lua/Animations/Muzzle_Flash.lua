muzzleFlash = {}

muzzleFlash.default = {}
muzzleFlash.default.startTime = 0.0416
muzzleFlash.default.endTime = 0.333
muzzleFlash.default.rate = 1.0

muzzleFlash.idle = {}
muzzleFlash.idle.startTime = 0.0416
muzzleFlash.idle.endTime = 0.333
muzzleFlash.idle.rate = 1.0

muzzleFlash.fire = {}
muzzleFlash.fire.startTime = 0.0416
muzzleFlash.fire.endTime = 0.333
muzzleFlash.fire.rate = 1.0

function GetMuzzleBurstVar(animation, data)
	return muzzleFlash[animation][data]
end