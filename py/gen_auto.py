import sys

from pybindgen import FileCodeSink
from pybindgen.gccxmlparser import ModuleParser

def my_module_gen():
    module_parser = ModuleParser('minisdifpy', '::')
    module_parser.parse(["../src/mSDIFUtils.hpp"], includes=['"../src/mSDIF.hpp"'], pygen_sink=FileCodeSink(sys.stdout))

# if __name__ == '__main__':
my_module_gen()
