effect = {}

effect.default = {}
effect.default.startTime = 0.0416
effect.default.endTime = 0.0832
effect.default.rate = 1.0

effect.idle = {}
effect.idle.startTime = 0.0416
effect.idle.endTime = 0.0832
effect.idle.rate = 1.0

----------flameBurst----------------------

effect.flameBurstGrow = {}
effect.flameBurstGrow.startTime = 0.0416
effect.flameBurstGrow.endTime = 0.4583
effect.flameBurstGrow.rate = 1.0

effect.flameBurstLive = {}
effect.flameBurstLive.startTime = 0.4583
effect.flameBurstLive.endTime = 0.9166
effect.flameBurstLive.rate = 1.0

effect.flameBurstDie = {}
effect.flameBurstDie.startTime = 0.9166
effect.flameBurstDie.endTime = 1.083
effect.flameBurstDie.rate = 1.0

----------muzzleFlash---------------------

effect.muzzleFlashFire = {}
effect.muzzleFlashFire.startTime = 0.0416
effect.muzzleFlashFire.endTime = 0.4166
effect.muzzleFlashFire.rate = 1.0

--------bigLazer-------------------

effect.lazerCharge = {}
effect.lazerCharge.startTime = 0.0416
effect.lazerCharge.endTime = 0.5416
effect.lazerCharge.rate = 1.0

effect.lazerSustain = {}
effect.lazerSustain.startTime = 0.5416
effect.lazerSustain.endTime = 0.7916
effect.lazerSustain.rate = 1.0

effect.lazerDie = {}
effect.lazerDie.startTime = 0.7916
effect.lazerDie.endTime = 1.0
effect.lazerDie.rate = 1.0

----------crateExplosion-------------------------

effect.crateDamage = {}
effect.crateDamage.startTime = 0.0416
effect.crateDamage.endTime = 0.625
effect.crateDamage.rate = 1.0

effect.crateExplode = {}
effect.crateExplode.startTime = 0.625
effect.crateExplode.endTime = 1.0
effect.crateExplode.rate = 1.0

-------barrelExplosion----------------------

effect.barrelExplode = {}
effect.barrelExplode.startTime = 0.0416
effect.barrelExplode.endTime = 0.5
effect.barrelExplode.rate = 1.0

--------dirtExplosion------------------

effect.dirtExplosionA = {}
effect.dirtExplosionA.startTime = 0.0416
effect.dirtExplosionA.endTime = 0.5
effect.dirtExplosionA.rate = 1.0

effect.dirtExplosionB = {}
effect.dirtExplosionB.startTime = 0.0416
effect.dirtExplosionB.endTime = 0.5
effect.dirtExplosionB.rate = 1.0

-------smallExplosion---------------------

effect.smallExplosion = {}
effect.smallExplosion.startTime = 0.0416
effect.smallExplosion.endTime = 0.3333
effect.smallExplosion.rate = 1.0

-------largeExplosion---------------------

effect.largeExplosion = {}
effect.largeExplosion.startTime = 0.0416
effect.largeExplosion.endTime = 0.625
effect.largeExplosion.rate = 1.0

-------shotgunEffects---------------------

effect.shotgunBlast = {}
effect.shotgunBlast.startTime = 0.0416
effect.shotgunBlast.endTime = 0.3333
effect.shotgunBlast.rate = 0.3333

-----forFuture----------------------------

function GetEffectVar(animation, data)
	return effect[animation][data]
end