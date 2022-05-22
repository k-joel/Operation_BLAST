pilotLight = {}

pilotLight.default = {}
pilotLight.default.startTime = 0.0416
pilotLight.default.endTime = 0.333
pilotLight.default.rate = 1.0

pilotLight.idle = {}
pilotLight.idle.startTime = 0.0416
pilotLight.idle.endTime = 0.333
pilotLight.idle.rate = 1.0

pilotLight.fire = {}
pilotLight.fire.startTime = 0.0416
pilotLight.fire.endTime = 0.333
pilotLight.fire.rate = 1.0

function GetPilotLightVar(animation, data)
	return pilotLight[animation][data]
end