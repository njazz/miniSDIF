import minisdifpy as m
import matplotlib.pyplot as plt
import numpy as np

from mpl_toolkits.mplot3d import Axes3D

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
# v = f1.readFile("Piano-B.sdif")
# v = f1.readFile("moanin.add.sdif")
v = f1.readFile("Piano-B.sdif")
# print(v)

f_l = []
for f in f1.frames():
    f_l += [f]

for n in m.SDIFType("1TRC").columnNames():
    print(n)
print("frames: ",f1.frameCount())
print("ms: ",f_l[3].matrixCount())


v_l = []
v_l_a = []
v_l_i = []

for i in range(3,f1.frameCount()):
    layer_id = i

    m_l = []
    for fi in f_l[layer_id].matrices():
        m_l += [fi]

    if (len(m_l)):
        for v in m_l[0].valuesAtColumn(1):  #freq
            v_l += [v/1000]

        for v in m_l[0].valuesAtColumn(2):  #amp
            v_l_a += [v*5]

        for v in m_l[0].valuesAtColumn(0):  #idx
            v_l_i += [layer_id/100]

# print("dim: ",m_l[0].rows(), m_l[0].columns())

print(len(v_l))
# print(v_l)

# fig = plt.figure()
# ax = fig.add_subplot(111, projection="3d")
# ax.plot(v_l_i,v_l, v_l_a)
# plt.show()


filepath = "export.obj"

idx = 0
with open(filepath, 'w') as f:
    f.write("# OBJ file\n")
    for v in v_l_i:
        f.write("v %.4f %.4f %.4f\n" % (v_l_i[idx] , v_l[idx] , v_l_a[idx]))
        idx += 1

#     for fi in f.matrices():
#         print(fi.rows())
#         print(fi.columns())
#         for v in fi.dataVec():
#              mp.plot("x","y",v)
