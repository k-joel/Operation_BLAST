laserFlash = {}

laserFlash.default = {}
laserFlash.default.startTime = 0.0416
laserFlash.default.endTime = 0.333
laserFlash.default.rate = 1.0

laserFlash.idle = {}
laserFlash.idle.startTime = 0.0416
laserFlash.idle.endTime = 0.333
laserFlash.idle.rate = 1.0

laserFlash.fire = {}
laserFlash.fire.startTime = 0.0416
laserFlash.fire.endTime = 0.333
laserFlash.fire.rate = 1.0

function GetLaserFlashVar(animation, data)
	return laserFlash[animation][data]
end