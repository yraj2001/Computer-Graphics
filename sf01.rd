Display "Square and Fill"  "Screen" "rgbsingle"
Format   600   600

CameraEye   200   200 -200
CameraAt   200   200   0
CameraUp   0.0 -1.0   0.0

FrameBegin   1
Background   1.0   1.0   1.0

WorldBegin

Color 0.0 0.0 0.0

# Draws the square
Line   100   100   0   500   100   0
Line   500   100   0   500   500   0
Line   500   500   0   100   500   0
Line   100   500   0   100   100   0

# Draws Lines
Line   300   100   0   300   500   0
Line   100   300   0   500   300   0

# Point

Color 1.0 1.0 1.0

Point 300 300 0

#FIll

Color 1.0 0.4 0
Fill 200 200 0

Color 1.0 0.6 0.2
Fill 200 400 0

Color 0.7778 1.0 0
Fill 400 200 0

Color 0.3333 1.0 0
Fill 400 400 0

WorldEnd
FrameEnd
