#!/usr/bin/python
# Generate C-style wrapper functions
# Thomas Perl <m@thp.io>; 2012-10-19
# Updated for full GLES wrapper generation (2012-11-19)
# License: See the COPYING file.

import re
import os
import sys
import subprocess

# You need to have the Harmattan Platform SDK and the OpenGL ES development
# packages installed: libgles1-sgx-img-dev libgles2-sgx-img-dev
SCRATCHBOX_USR_INCLUDE = ('/scratchbox/users/' +
        os.environ['USER'] +
        '/targets/HARMATTAN_ARMEL/usr/include/')

def mangle(name):
    return 'my_' + name

class Argument:
    def __init__(self, type_, name):
        self.type_ = type_
        self.name = name

class Function:
    def __init__(self, retval, name, args):
        self.retval = retval
        self.name = name
        self.raw_args = args
        self.parsing_error = None
        self.args = list(self._parse_args(args))

    def _parse_args(self, args):
        for arg in args.split(','):
            xarg = re.match(r'^(.*?)([A-Za-z0-9_]+)$', arg)
            if not xarg:
                xarg = re.match(r'^(.*?)([A-Za-z0-9_]+)\[\d+\]$', arg)
            if not xarg:
                self.parsing_error = 'Could not parse: ' + repr(arg)
                print self.parsing_error
                continue
            type_, name = xarg.groups()
            yield Argument(type_, name)

    def _format_args(self):
        if self.raw_args == 'void':
            return ''
        else:
            return self.raw_args

    def _format_return(self):
        if self.retval == 'void':
            return ''
        else:
            return 'return '

    def _format_call_args(self):
        return ', '.join(a.name for a in self.args if a.type_)

    def _format_call_args_formatstring(self):
        return ''

    def _format_call(self):
        if any(self.name.endswith(postfix)
                for postfix in ('OES', 'EXT', 'ARB', 'IMG')):
            return '/* No CALL */ printf("UNIMPLEMENTED: %s\\n");' % self.name
        else:
            return '%s%s(%s);' % (self._format_return(),
                self.name, self._format_call_args())

    def _format_prototype(self):
        funcname = mangle(self.name)
        return """
        %s\n%s(%s)
        """.strip() % (self.retval,
                funcname,
                self._format_args())

    def prototype(self):
        return self._format_prototype() + ' SOFTFP;'

    def implementation(self):
        return """%s
{
    WRAPPERS_DEBUG_PRINTF("%s(%s)\\n"%s%s);
    %s
}
        """.strip() % (self._format_prototype(),
                self.name,
                self._format_call_args_formatstring(),
                ', ' if self._format_call_args() else '',
                self._format_call_args(),
                self._format_call())

    def mapping(self):
        if self.parsing_error:
            return '/* unmapped: %s (%s) */' % (self.name, self.parsing_error)
        return """
        {"%s", %s},
        """.strip() % (self.name, mangle(self.name))

for filename in ('gles.h', 'gles2.h'):
    data = subprocess.check_output(['cproto',
        '-I' + SCRATCHBOX_USR_INCLUDE,
        '-x', filename])
    feature_test_macro = 'APKENV_' + os.path.splitext(filename)[0].upper()

    funcs = []
    for line in data.splitlines():
        if re.match(r'^/\*.*\*/', line):
            continue
        retval, funcname, args = re.match(r'^(.+ [*]?)([A-Za-z0-9_]+)\((.*)\);\s*$', line).groups()
        retval, funcname, args = [x.strip() for x in (retval, funcname, args)]
        funcs.append(Function(retval, funcname, args))

    basename = os.path.basename(filename)
    basename = basename[:basename.index('.')]

    with open(basename+'_wrappers.h', 'w') as fp:
        print >>fp, '#ifdef', feature_test_macro
        print >>fp, open(filename).read()
        print >>fp, '#include "../apkenv.h"'
        for function in funcs:
            print >>fp, function.prototype()
        print >>fp, '#endif /*', feature_test_macro, '*/'

    with open(basename+'_wrappers.c', 'w') as fp:
        print >>fp, '#ifdef', feature_test_macro
        print >>fp, '#include "'+basename+'_wrappers.h"'
        print >>fp, '#include <assert.h>'
        print >>fp, """
#ifdef APKENV_DEBUG
#  define WRAPPERS_DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#  define WRAPPERS_DEBUG_PRINTF(...)
#endif
        """.strip()
        for function in funcs:
            print >>fp, function.implementation()
        print >>fp, '#endif /*', feature_test_macro, '*/'

    with open(basename+'_mapping.h', 'w') as fp:
        print >>fp, '#ifdef', feature_test_macro
        for function in funcs:
            print >>fp, function.mapping()
        print >>fp, '#endif /*', feature_test_macro, '*/'

