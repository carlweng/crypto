filename = "Frame"

nBlanket = []

outputFile = open("NinFrames.txt", "w")

for i in range(0, 21):
    if i == 4:
        continue
    name = filename + str(i)
    frame = open(name, "r")
    content = frame.readline()
    n = content[0:256]
    e = content[256:512]
    c = content[512:768]
    print "frame" + str(i)
    #print n
    print e
    print c
    #outputFile.write(n)

#frame 0 and 4 have same n
#e = 10001    frame 1, 2, 5, 6, 9, 10, 13, 14, 17, 18, 19
#e = 5        frame 3, 8, 12, 16, 20
#e = 3        frame 7, 11, 15

#t is a f
#. Imagin
#m A to B
#My secre
#will get
# That is
# "Logic 
#instein.