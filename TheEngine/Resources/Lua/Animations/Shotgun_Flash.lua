shotgunFlash = {}

shotgunFlash.default = {}
shotgunFlash.default.startTime = 0.0416
shotgunFlash.default.endTime = 0.333
shotgunFlash.default.rate = 1.0

shotgunFlash.idle = {}
shotgunFlash.idle.startTime = 0.0416
shotgunFlash.idle.endTime = 0.333
shotgunFlash.idle.rate = 1.0

shotgunFlash.fire = {}
shotgunFlash.fire.startTime = 0.0416
shotgunFlash.fire.endTime = 0.333
shotgunFlash.fire.rate = 1.0

function GetShotgunFlashVar(animation, data)
	return shotgunFlash[animation][data]
end