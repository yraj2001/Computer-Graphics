Display "Cylinder Test" "Screen" "rgbdouble"

Background 0.0 0.0 0.0

CameraEye -6 2.7 5
CameraAt 0 1.2 2.7
CameraUp 0 0 1
CameraFOV 80.0

ObjectBegin "N"

PolySet "P"
20 #vertices
12 #Faces

1.0 6.0 0.0 #v0
1.0 5.0 0.0 #v1
1.0 3.0 0.0 #v2
1.0 2.0 0.0 #v3
1.0 3.0 1.0 #v4
1.0 5.0 3.0 #v5
1.0 6.0 4.0 #v6
1.0 5.0 4.0 #v7
1.0 3.0 4.0 #v8
1.0 2.0 4.0 #v9

-1.0 6.0 0.0 #v10
-1.0 5.0 0.0 #v11
-1.0 3.0 0.0 #v12
-1.0 2.0 0.0 #v13
-1.0 3.0 1.0 #v14
-1.0 5.0 3.0 #v15
-1.0 6.0 4.0 #v16
-1.0 5.0 4.0 #v17
-1.0 3.0 4.0 #v18
-1.0 2.0 4.0 #v19
#Face
0 1 5 2 3 9 8 4 7 6 -1
10 11 15 12 13 19 18 14 17 16 -1
10 0 6 16 -1
11 1 5 15 -1
15 5 2 12 -1
17 7 4 14 -1
18 8 4 14 -1
19 9 3 13 -1
16 6 7 17 -1
18 8 9 19 -1
10 0 1 11 -1
12 2 3 13 -1
ObjectEnd # N

FrameBegin 1
WorldBegin
Color 1.0 0.0 0.0

ObjectInstance "N"

XformPush
Translate 0.0 0.0 2.0
Scale 1.0 1.0 2.0
Cube
Translate 0.0 0.0 2.0
Scale 1.0 1.0 0.5
Sphere 1.0 0.0 0.0 360.0
XformPop

XformPush
Translate 0.0 -2.5 2.0
Scale 1.0 0.5 2.0
Cube
Translate 0.0 -4.0 0.0
Cube
#Sphere 1.0 0.0 0.0 360.0
XformPop

Translate 0.0 -3.55 0.5
Scale 1.0 1.5 0.5
Cube

WorldEnd
FrameEnd
