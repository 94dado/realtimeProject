def avg(l):
    return sum(l)/len(l)

f = open("fps.txt","r")
rows = f.readlines()
fps_list = [int(row.split(":")[1]) for row in rows]
print(int(round(avg(fps_list))))

