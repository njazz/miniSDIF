from pybindgen import *
import pybindgen
import sys

mod = pybindgen.Module('minisdifpy')

mod.add_container('std::vector<std::string>', 'std::string', 'vector')

mod.add_include('"../src/mSDIF.hpp"')

msd_t = mod.add_class('MSDIFType')
msd_t.custom_name = "SDIFType"

# msd_t.add_method('fromSignature',None,[param('std::string','signature')])

msd_t.add_function_as_constructor('MSDIFType::fromSignature',None,[param('std::string','signature')])

msd_t.add_method('signature',retval('std::string'),[])
msd_t.add_method('dataType',retval('uint32_t'),[])
msd_t.add_method('columnNames',retval('std::vector<std::string>'),[])
msd_t.add_method('description',retval('std::string'),[])

# f1 = mod.add_function('MSDIFType::fromSignature',None,[param('std::string','signature')])
# f1.custom_name = "TypeFromSignature"

###

msd_matrix = mod.add_class('MSDIFMatrix')
msd_matrix.custom_name = "SDIFMatrix"

msd_matrix.add_constructor([\
param('std::string','signature'),\
param('uint32_t','rows'),\
param('uint32_t','columns'),\
param('uint32_t','type')\
])

msd_matrix.add_constructor([\
param('std::string','signature'),
])

msd_matrix.add_copy_constructor()

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

# msd_matrix.add_method('addRow', None, [])
# msd_matrix.add_method('addColumn', None, [])
#
# msd_matrix.add_method('deleteRow', None, [param('size_t', 'idx')])
# msd_matrix.add_method('deleteColumn', None, [param('size_t', 'idx')])
#
msd_matrix.add_method('matrixDataSize', retval('uint32_t'), [],)
msd_matrix.add_method('paddingSize', retval('int'), [],)

msd_matrix.add_method('signature', retval('char*'), [],)

msd_matrix.add_method('info', retval('std::string'), [],)

msd_matrix.add_method('applyGain', None, [param('float', 'g')])

msd_matrix.add_method('maximumIndexValue', retval('size_t'), [],)

msd_matrix.add_method('shiftIndices', None, [param('size_t', 'idx')])

# TODO: templates

mod.add_container("std::vector<float>","float","list", custom_name="FVec")
msd_matrix.add_method('dataVec<float>', ReturnValue.new("std::vector<float>"),[], custom_name="dataVec")

# fc =mod.add_class("float")
# cf = mod.add_container("std::vector<float>","float","list", custom_name="FVec")
#
# msd_matrix.add_method('data<float>', ReturnValue.new("const float*", caller_owns_return=True),[])
#
# msd_matrix.add_method('valuesAtRow<float>', ReturnValue.new("const float*", caller_owns_return=True), [\
# param('size_t', 'idx')\
# ], custom_name="valuesAtRow")
#
msd_matrix.add_method('valuesAtColumnVec<float>', ReturnValue.new("std::vector<float>"), [\
param('size_t', 'idx')\
], custom_name="valuesAtColumn")

##########

msd_frame = mod.add_class('MSDIFFrame')
msd_frame.custom_name = "SDIFFrame"

msd_frame.add_constructor([\
param('std::string','signature'),\
param('int32_t','streamID'),\
])

msd_frame.add_copy_constructor()

msd_frame.add_method('signature', retval('char*'), [])
msd_frame.add_method('frameSize', retval('uint32_t'), [])

msd_frame.add_method('time', retval('double'), [])
msd_frame.add_method('setTime', None, [param('double','t')])

msd_frame.add_method('streamID', retval('int32_t'), [])
msd_frame.add_method('setStreamID', None, [param('int32_t','s')])

msd_frame.add_method('matrixCount', retval('uint32_t'), [])

# mod.add_container('std::vector<float*>', 'float', 'vector')
mod.add_container('std::vector<MSDIFMatrix>', 'MSDIFMatrix', 'vector', custom_name="SDIFMatrixVec")
msd_frame.add_method('matrices',retval('std::vector<MSDIFMatrix>'),[])

###

msd_file = mod.add_class("MSDIFFile")
msd_file.custom_name = "SDIFFile"

msd_file.add_constructor([])

msd_file.add_method('readFile', retval('int'), [param('std::string','filename')])
msd_file.add_method('writeFile', retval('int'), [param('std::string','filename')])

msd_file.add_method('frameCount', retval('uint32_t'), [])
# mod.add_typedef(["std::vector<MSDIFFrame*>"],["MSDIFFrameVector" ])
# msd_file.add_method('frameCount', retval('int'), [])

# mod.add_container('std::vector<float*>', 'float', 'vector')
fl =mod.add_container('std::vector<MSDIFFrame>', 'MSDIFFrame', 'vector')
# mod.add_container('std::vector<MSDIFFrame*>', 'MSDIFFrame', 'vector')

msd_file.add_method('frames', retval('std::vector<MSDIFFrame>'), [])

mod.generate(sys.stdout)
