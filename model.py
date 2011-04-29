from math import sqrt

# verticies
vs = []

# springs
ss = []

# faces
fs = []

for x in range(7):
    for y in range(3):
        for z in range(9):
            if z < 3 or z > 5 or (x >= 2 and x <= 4):
                vs.append( (x, y, z) )

for (x, y, z) in vs:
    cur = vs.index( (x, y, z) );
    # try to connect all verticies to surrounding verticies
    for i in [-1, 0, 1]:
        for j in [-1, 0, 1]:
            for k in [-1, 0, 1]:
                # don't connect to yourself and don't connect to non-existant verticies
                if i != 0 or j != 0 or k != 0:
                    point = (x+i, y+j, z+k)
                    if point in vs:
                        dist = sqrt( i*i + j*j + k*k )
                        ss.append( (cur, vs.index(point), dist) )


def frontFace(x, y, z):
    current = (x, y, z)
    up = (x, y, z+1)
    right = (x+1, y, z)
    up_right = (x+1, y, z+1)

    fs.append( (vs.index(current), vs.index(up_right), vs.index(up)) )
    fs.append( (vs.index(current), vs.index(right), vs.index(up_right)) )

def backFace(x, y, z):
    current = (x, y, z)
    up = (x, y, z+1)
    right = (x+1, y, z)
    up_right = (x+1, y, z+1)

    fs.append( (vs.index(current), vs.index(up), vs.index(up_right)) )
    fs.append( (vs.index(current), vs.index(up_right), vs.index(right)) )


def leftFace(x, y, z):
    current = (x, y, z)
    up = (x, y, z+1)
    right = (x, y+1, z)
    up_right = (x, y+1, z+1)

    fs.append( (vs.index(current), vs.index(up), vs.index(up_right)) )
    fs.append( (vs.index(current), vs.index(up_right), vs.index(right)) )


def rightFace(x, y, z):
    current = (x, y, z)
    up = (x, y, z+1)
    right = (x, y+1, z)
    up_right = (x, y+1, z+1)

    fs.append( (vs.index(current), vs.index(up_right), vs.index(up)) )
    fs.append( (vs.index(current), vs.index(right), vs.index(up_right)) )

def topFace(x, y, z):
    current = (x, y, z)
    up = (x, y+1, z)
    left = (x+1, y, z)
    up_left = (x+1, y+1, z)
    fs.append( (vs.index(current), vs.index(up_left), vs.index(up)) )
    fs.append( (vs.index(current), vs.index(left), vs.index(up_left)) )

def bottomFace(x, y, z):
    current = (x, y, z)
    up = (x, y+1, z)
    left = (x+1, y, z)
    up_left = (x+1, y+1, z)
    fs.append( (vs.index(current), vs.index(up), vs.index(up_left)) )
    fs.append( (vs.index(current), vs.index(up_left), vs.index(left)) )


# front bottom faces
for x in range(6):
    for z in [0, 1]:
        backFace(x, 2, z)
        frontFace(x, 0, z)

# front middle faces
for x in [2, 3]: 
    for z in [2, 3, 4, 5]:
        backFace(x, 2, z)
        frontFace(x, 0, z)

# front top faces
for x in range(6):
    for z in [6, 7]:
        backFace(x, 2, z)
        frontFace(x, 0, z)

# side faces
for y in [0, 1]:
    for z in [0, 1, 6, 7]:
        leftFace(0, y, z)
        rightFace(6, y, z)

for y in [0, 1]:
    for z in [2, 3, 4, 5]:
        leftFace(2, y, z)
        rightFace(4, y, z)

# top/bottom faces
for x in [0, 1, 2, 3, 4, 5]:
    for y in [0, 1]:
        topFace(x, y, 8)
        bottomFace(x, y, 0)

for x in [0, 1, 4, 5]:
    for y in [0, 1]:
        topFace(x, y, 2)
        bottomFace(x, y, 6)


for (x, y, z) in vs:
    print( "v " + str(x) + " " + str(y) + " " + str(z) )

for (i, j, dist) in ss:
    print( "s " + str(i) + " " + str(j) + " " + str(dist) )

for (a, b, c) in fs:
    print( "f " + str(a) + " " + str(b) + " " + str(c) )
