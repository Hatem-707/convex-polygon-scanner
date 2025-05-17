import numpy as np
import random 

with open("/home/hatem/Programing/cpp_vscode/convex-polygon-scanner/code/newExtensionData.txt") as file:
    lines = file.readlines()

    index = 0
    for line in lines:
        line = line.strip()
        points = line.split("|")
        tuples = []
        for point in points:
            pair = point.split()
            if len(pair) == 2:
                tuples.append((int(pair[0]), int(pair[1]), int(pair[0]) , int(pair[1])))
        
        noise = []
        for i in range(16):
            while True:
                ra_X = random.randint(0,100)
                ra_Y = random.randint(0,100)
                if not( (ra_X, ra_Y, ra_X, ra_Y) in tuples):
                    break

            noise.append((ra_X, ra_Y, ra_X, ra_Y))
        
        fullset = tuples.copy()
        fullset.extend(noise)
        random.shuffle(fullset)

        labels = []

        empty = 0

        for point in fullset:
            if point in tuples:
                labels.append(0)
                empty+=1
            else:
                labels.append(1)
        
        data = np.array(fullset, dtype=np.float32)
        annot = np.array(labels, dtype=np.uint32)

        data.tofile(f"/home/hatem/Programing/cpp_vscode/convex-polygon-scanner/code/01/velodyne/{index}.bin")
        annot.tofile(f"/home/hatem/Programing/cpp_vscode/convex-polygon-scanner/code/01/labels/{index}.label")
        index+=1
        print(empty)


        