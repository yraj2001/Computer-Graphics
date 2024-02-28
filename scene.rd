Display "Scene NIU"  "Screen" "rgbdouble"
Format   700   600

CameraEye   200   200 -200
CameraAt   200   200   0
CameraUp   0.0 -1.0   0.0

FrameBegin   1

WorldBegin


# Draws the N
Line   100   100   0   100   500   0
Line   100   500   0   150   500   0
Line   150   500   0   150   200   0
Line   150   200   0   250   500   0
Line   250   500   0   300   500   0
Line   300   500   0   300   100   0
Line   300   100   0   250   100   0
Line   250   100   0   250   400   0
Line   250   400   0   150   100   0
Line   150   100   0   100   100   0

#Draws i

Line   350   200   0   350   500   0
Line   350   500   0   400   500   0
Line   400   500   0   400   200   0
Line   400   200   0   350   200   0

Circle 375 150 0 40
Circle 375 150 0 15

#Draws U
Line   450   100   0   450   500   0
Line   450   500   0   650   500   0
Line   650   500   0   650   100   0
Line   650   100   0   600   100   0
Line   600   100   0   600   450   0
Line   600   450   0   500   450   0
Line   500   450   0   500   100   0
Line   500   100   0   450   100   0

Color 0.9 0.2 0.2

Fill 200 300 0
Fill 375 360 0
Fill 400 150 0
Fill 475 320 0


WorldEnd
FrameEnd
