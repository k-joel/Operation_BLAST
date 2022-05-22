boss = {}

boss.default = {}
boss.default.startTime = 0.0416
boss.default.endTime = 1.0
boss.default.rate = 1.0

boss.dance = {}
boss.dance.startTime = 0.0416
boss.dance.endTime = 1.0
boss.dance.rate = 1.0

--old boss shit
---------------------phase1---------------------

boss.walk = {}
boss.walk.startTime = 0.0416
boss.walk.endTime = 1.0
boss.walk.rate = 1.0

boss.stomp = {}
boss.stomp.startTime = 0.0416
boss.stomp.endTime = 1.0
boss.stomp.rate = 1.0

boss.endPhase1 = {}
boss.endPhase1.startTime = 0.0416
boss.endPhase1.endTime = 1.0
boss.endPhase1.rate = 1.0

---------------------phase2---------------------

boss.crawl = {}
boss.crawl.startTime = 0.0416
boss.crawl.endTime = 1.0
boss.crawl.rate = 1.0

boss.crawlSmash = {}
boss.crawlSmash.startTime = 0.0416
boss.crawlSmash.endTime = 1.0
boss.crawlSmash.rate = 1.0

boss.crawlLazer = {}
boss.crawlLazer.startTime = 0.0416
boss.crawlLazer.endTime = 1.0
boss.crawlLazer.rate = 1.0

boss.endPhase2 = {}
boss.endPhase2.startTime = 0.0416
boss.endPhase2.endTime = 1.0
boss.endPhase2.rate = 1.0

---------------------phase3---------------------

function GetBossVar(animation, data)
	return boss[animation][data]
end