#!/usr/bin/python
# Generate C-style wrapper functions
# Thomas Perl <m@thp.io>; 2012-10-19
# License: See the COPYING file.

import re
import os
import sys

funcs = []

# Some special cases to to avoid having to parse the full C grammar
FIXED_MAPPINGS = {
    'struct __jmp_buf_tag __env[1]': '__env',
    'void *(*__start_routine)(void *)': '__start_routine',
    'void (*__destr_function)(void *)': '__destr_function',
    'void (*__init_routine)(void)': '__init_routine',
}

# Never generate wrappers for these functions
BLACKLISTED_FUNCS = [
    'pthread_mutex_init',
    'pthread_mutex_lock',
    'pthread_mutex_trylock',
    'pthread_mutex_unlock',
    'pthread_mutex_destroy',
]

# Generate wrappers, but do not call original functions
EMPTYCALL_FUNCS = [
    'glBindFramebufferOES',
    'glBindRenderbufferOES',
    'glCheckFramebufferStatusOES',
    'glDeleteFramebuffersOES',
    'glDeleteRenderbuffersOES',
    'glFramebufferRenderbufferOES',
    'glFramebufferTexture2DOES',
    'glGenFramebuffersOES',
    'glGenRenderbuffersOES',
    'glRenderbufferStorageOES',
]

def clean_arg(arg):
    arg = arg.replace('__restrict', '')
    arg = arg.replace('__const', 'const')
    return arg.strip()

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
        for arg in map(clean_arg, args.split(',')):
            arg = clean_arg(arg)
            xarg = re.match(r'^(.*?)([A-Za-z0-9_]+)$', arg)
            if not xarg:
                # Unknown argument
                if arg in FIXED_MAPPINGS:
                    yield Argument('<fixed>', FIXED_MAPPINGS[arg])
                    continue
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
        if self.name in EMPTYCALL_FUNCS:
            return '/* No CALL */'
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


if len(sys.argv) != 2:
    print >>sys.stderr, """
    Usage: %s output/<yoursharedlib>.required.includes.harmattan.h
    """ % (sys.argv[0],)
    sys.exit(1)

filename = sys.argv[1]

for line in open(filename):
    retval, funcname, args = re.match(r'^(.+ [*]?)([A-Za-z0-9_]+)\((.*)\);\s*$', line).groups()
    retval, funcname, args = [x.strip() for x in (retval, funcname, args)]
    if funcname not in BLACKLISTED_FUNCS:
        funcs.append(Function(retval, funcname, args))

basename = os.path.basename(filename)
basename = basename[:basename.index('.')]

with open('output/'+basename+'_wrappers.h', 'w') as fp:
    print >>fp, open('output/'+basename+'.required.includes').read()
    print >>fp, open('default_includes.h').read()
    for function in funcs:
        print >>fp, function.prototype()

with open('output/'+basename+'_wrappers.c', 'w') as fp:
    print >>fp, '#include "'+basename+'_wrappers.h"'
    for function in funcs:
        print >>fp, function.implementation()

with open('output/'+basename+'_mapping.h', 'w') as fp:
    for function in funcs:
        print >>fp, function.mapping()

