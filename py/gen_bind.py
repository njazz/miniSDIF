from pybindgen import *
import pybindgen
import sys

mod = pybindgen.Module('minisdifpy')

mod.add_include('"../src/mSDIF.hpp"')

msd_matrix = mod.add_class('MSDIFMatrix')

msd_matrix.add_constructor([\
param('std::string','signature'),\
param('uint32_t','rows'),\
param('uint32_t','columns'),\
param('uint32_t','type')\
])

msd_matrix.add_method('rows', retval('int'), [], is_const=False)
msd_matrix.add_method('columns', retval('int'), [], is_const=False)

msd_matrix.add_method('newSize', None, [\
param('uint32_t', 'rows'),\
param('uint32_t', 'columns')\
])

msd_matrix.add_method('resize', None, [\
param('uint32_t', 'rows'),\
param('uint32_t', 'columns')\
])

msd_matrix.add_method('resizeRows', None, [\
param('uint32_t', 'rows')\
])

msd_matrix.add_method('addRow', None, [])
msd_matrix.add_method('addColumn', None, [])

msd_matrix.add_method('deleteRow', None, [param('size_t', 'idx')])
msd_matrix.add_method('deleteColumn', None, [param('size_t', 'idx')])

msd_matrix.add_method('matrixDataSize', retval('uint32_t'), [],)
msd_matrix.add_method('paddingSize', retval('int'), [],)

msd_matrix.add_method('signature', retval('char*'), [],)

msd_matrix.add_method('info', retval('std::string'), [],)

msd_matrix.add_method('applyGain', None, [param('float', 'g')])

msd_matrix.add_method('maximumIndexValue', retval('size_t'), [],)

msd_matrix.add_method('shiftIndices', None, [param('size_t', 'idx')])

# TODO: templates

##########

msd_frame = mod.add_class('MSDIFFrame')

msd_frame.add_constructor([\
param('std::string','signature'),\
param('int32_t','streamID'),\
])

msd_matrix.add_method('signature', retval('char'), [])
msd_matrix.add_method('frameSize', retval('uint32_t'), [])

msd_matrix.add_method('time', retval('double'), [])
msd_matrix.add_method('setTime', None, [param('double','t')])

msd_matrix.add_method('streamID', retval('int32_t'), [])
msd_matrix.add_method('setStreamID', None, [param('int32_t','s')])

msd_matrix.add_method('matrixCount', retval('uint32_t'), [])

###

mod.generate(sys.stdout)
