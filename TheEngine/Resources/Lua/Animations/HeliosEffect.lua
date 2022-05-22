heliosFX = {}

heliosFX.default = {}
heliosFX.default.startTime = 0.0
heliosFX.default.endTime = 0.42
heliosFX.default.rate = 0.6

function GetHeliosFXVar(animation, data)
	return heliosFX[animation][data]
end