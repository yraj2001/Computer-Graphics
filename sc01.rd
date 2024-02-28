Display "Circles"  "Screen" "rgbsingle"
Format   1000   600

CameraEye   250   150 -150
CameraAt   250   150   0
CameraUp   0.0 -1.0   0.0

FrameBegin   1
Background   0.3   0.3   0.3

WorldBegin

# Left half with odd radii
Color   1.0   0.6   0.0
Circle   250   300   0   1
Color   0.6   1.0   0.0
Circle   250   300   0   3
Color   0.0   1.0   0.6
Circle   250   300   0   5
Color   0.0   0.6   1.0
Circle   250   300   0   7
Color   0.6   0.0   1.0
Circle   250   300   0   9
Color   1.0   0.0   0.6
Circle   250   300   0   11

# Right half with even radii
Color   0.6   0.0   1.0
Circle   750   300   0   2
Color   1.0   0.0   0.6
Circle   750   300   0   4
Color   0.0   1.0   0.6
Circle   750   300   0   6
Color   0.6   1.0   0.0
Circle   750   300   0   8
Color   0.0   0.6   1.0
Circle   750   300   0   10
Color   1.0   0.6   0.0
Circle   750   300   0   12

WorldEnd
FrameEnd
