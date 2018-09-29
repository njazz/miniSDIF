import minisdifpy as m
import matplotlib.pyplot as mp

print("imported")

print(dir(m))

print(dir(m.SDIFFile))
print(dir(m.SDIFFrame))
print(dir(m.SDIFMatrix))

# t1 = m.SDIFType('1TRC')
# print(str(t1))
# for o in t1.columnNames():
#     print(o)
# print(str(t1.description()))
# print(str(t1.signature()))

f1 = m.SDIFFile()
v = f1.readFile("Piano-B.sdif")
# print(v)

f_l = []
for f in f1.frames():
    f_l += [f]

m_l = []
for fi in f_l[0].matrices():
    m_l += [fi]

v_l = []
for v in m_l[0].dataVec():
    v_l += [v]


mp.plot("x","y", v_l)
mp.show()


#     for fi in f.matrices():
#         print(fi.rows())
#         print(fi.columns())
#         for v in fi.dataVec():
#              mp.plot("x","y",v)
