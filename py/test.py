import minisdifpy as m

print("imported")

print(dir(m))

# t1 = m.SDIFType('1TRC')
# print(str(t1))
# for o in t1.columnNames():
#     print(o)
# print(str(t1.description()))
# print(str(t1.signature()))

f1 = m.SDIFFile()
v = f1.readFile("Piano-B.sdif")
print(v)
