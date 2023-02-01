
#
# apkenv GLES serialization generator
# Copyright (c) 2023, Thomas Perl <m@thp.io>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
# IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

import re
import textwrap
import argparse

parser = argparse.ArgumentParser(description='Generate GL serialization code')
parser.add_argument('infile', type=str, help='Input file')
parser.add_argument('outfile', type=str, help='Output file')

args = parser.parse_args()

IMPLEMENTED_FUNCS = """
glAlphaFunc
glClearColor
glClearDepthf
#glClipPlanef
glColor4f
glDepthRangef
glFogf
#glFogfv
glFrustumf
#glGetClipPlanef
#glGetFloatv
#glGetLightfv
#glGetMaterialfv
#glGetTexEnvfv
#glGetTexParameterfv
glLightModelf
glLightModelfv
glLightf
glLightfv
glLineWidth
glLoadMatrixf
glMaterialf
glMaterialfv
glMultMatrixf
glMultiTexCoord4f
glNormal3f
glOrthof
glPointParameterf
#glPointParameterfv
glPointSize
glPolygonOffset
glRotatef
glScalef
glTexEnvf
#glTexEnvfv
glTexParameterf
#glTexParameterfv
glTranslatef
glActiveTexture
glAlphaFuncx
glBindBuffer
glBindTexture
glBlendFunc
glBufferData
glBufferSubData
glClear
glClearColorx
glClearDepthx
glClearStencil
glClientActiveTexture
#glClipPlanex
glColor4ub
glColor4x
glColorMask
glColorPointer
#glCompressedTexImage2D
#glCompressedTexSubImage2D
#glCopyTexImage2D
#glCopyTexSubImage2D
glCullFace
glDeleteBuffers
glDeleteTextures
glDepthFunc
glDepthMask
glDepthRangex
glDisable
glDisableClientState
glDrawArrays
glDrawElements
glEnable
glEnableClientState
glFinish
glFlush
glFogx
#glFogxv
glFrontFace
glFrustumx
#glGetBooleanv
#glGetBufferParameteriv
#glGetClipPlanex
glGenBuffers
glGenTextures
glGetError
#glGetFixedv
#glGetIntegerv
#glGetLightxv
#glGetMaterialxv
#glGetPointerv
#glGetString
#glGetTexEnviv
#glGetTexEnvxv
#glGetTexParameteriv
#glGetTexParameterxv
glHint
glIsBuffer
glIsEnabled
glIsTexture
glLightModelx
#glLightModelxv
glLightx
#glLightxv
glLineWidthx
glLoadIdentity
#glLoadMatrixx
glLogicOp
glMaterialx
#glMaterialxv
glMatrixMode
#glMultMatrixx
glMultiTexCoord4x
glNormal3x
glNormalPointer
glOrthox
glPixelStorei
glPointParameterx
#glPointParameterxv
glPointSizex
glPolygonOffsetx
glPopMatrix
glPushMatrix
#glReadPixels
glRotatex
glSampleCoverage
glSampleCoveragex
glScalex
glScissor
glShadeModel
glStencilFunc
glStencilMask
glStencilOp
glTexCoordPointer
glTexEnvi
glTexEnvx
#glTexEnviv
#glTexEnvxv
glTexImage2D
glTexParameteri
glTexParameterx
#glTexParameteriv
#glTexParameterxv
glTexSubImage2D
glTranslatex
glVertexPointer
glViewport
#glPointSizePointerOES

glIsRenderbufferOES
glBindRenderbufferOES
glDeleteRenderbuffersOES
glGenRenderbuffersOES
glRenderbufferStorageOES
#glGetRenderbufferParameterivOES
glIsFramebufferOES
glBindFramebufferOES
glDeleteFramebuffersOES
glGenFramebuffersOES
glCheckFramebufferStatusOES
glFramebufferRenderbufferOES
glFramebufferTexture2DOES
#glGetFramebufferAttachmentParameterivOES
glGenerateMipmapOES

#glCurrentPaletteMatrixOES
#glLoadPaletteFromModelViewMatrixOES
#glMatrixIndexPointerOES
#glWeightPointerOES

#glQueryMatrixxOES

#glMapBufferOES
#glUnmapBufferOES
#glGetBufferPointervOES
""".strip().split()

DELAYED_FUNCTIONS = {
    'glColorPointer',
    'glNormalPointer',
    'glTexCoordPointer',
    'glVertexPointer',
}

DRAW_FUNCTIONS = {
    'glDrawArrays',
    'glDrawElements',
}

HOOK_FUNCTIONS = {
    'glBindBuffer',
}

func_re = re.compile(r'\s*(.*) \(\*(\w+)\)\((.*)\);$')

def parse_arg(arg):
    m = re.match(r'GLfloat (\w+)(\[\d+\])', arg)
    if m is not None:
        return ('GLfloat *', 'emit_float_array...', 'read_float_array...', m.group(1))

    m = re.match(r'(?:GLenum|GLclampx|GLuint|GLsizeiptr|GLintptr|GLbitfield|GLubyte|GLboolean|GLsizei) (\w+)', arg)
    if m is not None:
        return ('uint32_t', 'emit_uint32', 'read_uint32', m.group(1))

    m = re.match(r'(?:const )?(?:GLint|GLfixed) (\w+)', arg)
    if m is not None:
        return ('int32_t', 'emit_int32', 'read_int32', m.group(1))

    m = re.match(r'(?:const )?GLuint \*(\w+)', arg)
    if m is not None:
        return ('uint32_t *', 'emit_uint32...', 'read_uint32...', m.group(1))

    m = re.match(r'(?:const )?(?:GLint|GLfixed) \*(\w+)', arg)
    if m is not None:
        return ('int32_t *', 'emit_int32...', 'read_int32...', m.group(1))

    m = re.match(r'GLboolean \*(\w+)', arg)
    if m is not None:
        return ('GLboolean', 'emit_uint32', 'read_uint32', m.group(1))

    m = re.match(r'(?:GLclampf|GLfloat) (\w+)', arg)
    if m is not None:
        return ('float', 'emit_float', 'read_float', m.group(1))

    m = re.match(r'(?:const )?GLfloat \*(\w+)', arg)
    if m is not None:
        return ('float *', 'emit_float...', 'read_float...', m.group(1))

    m = re.match(r'(?:GLvoid|const GLvoid|const void) \*(\w+)', arg)
    if m is not None:
        return ('void *', 'emit_pointer...', 'read_pointer...', m.group(1))

    m = re.match(r'(?:GLvoid|const GLvoid|void) \*\*(\w+)', arg)
    if m is not None:
        return ('void **', 'emit_pointer2pointer...', 'read_pointer2pointer...', m.group(1))

    raise ValueError(arg)

def generate_function(ofp, index, return_type, function_name, args, arglist):
    result = []

    argcall = ', '.join(argname for _, _, _, argname in args)

    decode_lines = []

    if function_name in DELAYED_FUNCTIONS:
        function_name_encode = function_name + '_delayed'
    else:
        function_name_encode = function_name

    print(f'static {return_type} encode_{function_name_encode}({arglist});', file=ofp)

    result.append(textwrap.dedent(f"""
    static {return_type} encode_{function_name_encode}({arglist})
    {{"""))
    result.append(f'    emit_begin();')
    if function_name in DRAW_FUNCTIONS:
        result.append(f'    emit_draw_sync_begin();')
        result.append(f'    emit_draw_sync_{function_name}({argcall});')
    if function_name in HOOK_FUNCTIONS:
        result.append(f'    serialize_hook_{function_name}({argcall});')
    result.append(f'    emit_uint32({index});')

    upargs = []
    for argtype, emit_func, read_func, argname in args:
        if emit_func.endswith('...'):
            emit_func = emit_func[:-3]
            assert read_func.endswith('...')
            read_func = read_func[:-3]
            result.append(f'    {emit_func}_{function_name}_{argname}({argcall});')
            decode_lines.append(f'    const {argtype} {argname} = {read_func}_{function_name}_{argname}({", ".join(upargs)});')
        else:
            assert not read_func.endswith('...')
            result.append(f'    {emit_func}({argname});')
            decode_lines.append(f'    {argtype} {argname} = {read_func}();')
        upargs.append(argname)

    if function_name in DRAW_FUNCTIONS:
        result.append(f'    emit_draw_sync_end();')
    result.append('    emit_end();')
    result.append('}')

    result.append(textwrap.dedent(f"""
    static {return_type} decode_{function_name}(void)
    {{"""))

    result.extend(decode_lines)
    result.append(f'    decoder_functions.{function_name}({argcall});')
    result.append('}')

    return '\n'.join(result)

with open(args.outfile, 'w') as ofp:
    print(textwrap.dedent("""
    #include "../gles_vtable.h"

    #include <stdio.h>
    #include <stdbool.h>
    #include <stdlib.h>
    #include <string.h>


    /* DECODE STATE */

    static const void *read_buf = NULL;
    static size_t read_len = 0;
    static size_t read_pos = 0;

    const void *read_bytes(size_t len) {
        const void *result = read_buf + read_pos;
        read_pos += len;
        return result;
    }

    int32_t read_int32() { return *((int32_t *)read_bytes(sizeof(int32_t))); }
    uint32_t read_uint32() { return *((uint32_t *)read_bytes(sizeof(uint32_t))); }
    float read_float() { return *((float *)read_bytes(sizeof(float))); }


    /* ENCODE STATE */

    static void (*emit_bytes_func)(const void *, size_t, void *) = NULL;
    static void *emit_bytes_user_data = NULL;

    void gles_serialize_set_sink(void (*func)(const void *, size_t, void *), void *user_data) {
        emit_bytes_func = func;
        emit_bytes_user_data = user_data;
    }

    static uint8_t *write_buf = NULL;
    static size_t write_len = 0;
    static size_t write_pos = 0;
    static int emit_depth = 0;

    void emit_begin() {
        ++emit_depth;
    }

    void emit_end() {
        --emit_depth;
        if (emit_depth == 0) {
            emit_bytes_func(write_buf, write_pos, emit_bytes_user_data);
            write_pos = 0;
        }
    }

    void emit_bytes(const void *data, size_t len) {
        if (write_pos + len > write_len) {
            write_len += len * 2;
            write_buf = realloc(write_buf, write_len);
        }

        memcpy(write_buf + write_pos, data, len);
        write_pos += len;
    }

    void emit_int32(int32_t v) { emit_bytes(&v, sizeof(v)); }
    void emit_uint32(uint32_t v) { emit_bytes(&v, sizeof(v)); }
    void emit_float(float v) { emit_bytes(&v, sizeof(v)); }

    static struct gles1_functions decoder_functions;
    """), file=ofp)

    assignments = []
    switches = []
    function_bodies = []
    function_names = []
    index = 0

    with open(args.infile, 'r') as fp:
        inside = False

        for line in fp:
            line = line.rstrip()
            if line == 'struct gles1_functions {':
                inside = True
            elif inside and line == '};':
                inside = False
            elif inside:
                m = func_re.match(line)
                if m is not None:
                    return_type, function_name, args = m.groups()
                    arglist = args
                    if args == 'void':
                        args = []
                    else:
                        args = [parse_arg(arg) for arg in args.split(', ')]

                    index += 1

                    function_names.append(function_name)

                    if function_name in IMPLEMENTED_FUNCS:
                        function_bodies.append(generate_function(ofp, index, return_type, function_name, args, arglist))
                        switches.append(f'case {index}: decode_{function_name}(); break;')
                        assignments.append(f'dst->{function_name} = encode_{function_name};')
                elif not line.strip() or line.strip().startswith('/*') and line.strip().endswith('*/'):
                    ...  # empty line or comment line
                else:
                    raise ValueError(line)


    print('#include "../gles_serialize_helper.h"', file=ofp)

    for function_body in function_bodies:
        print(function_body, file=ofp)

    print(textwrap.dedent("""
    void gles_serialize_install_encoders(struct gles1_functions *dst)
    {
    """), file=ofp)
    for assignment in assignments:
        print(f'    {assignment}', file=ofp)
    print(textwrap.dedent("""
    }
    """), file=ofp)

    print(textwrap.dedent("""
    void gles_serialize_install_decoder(void *(*resolve)(const char *))
    {
    """), file=ofp)
    for function_name in function_names:
        print(f'    decoder_functions.{function_name} = resolve("{function_name}");', file=ofp)
    print(textwrap.dedent("""
    }
    """), file=ofp)

    print(textwrap.dedent("""
    void gles_serialize_decode(const void *buf, size_t len)
    {
        read_buf = buf;
        read_len = len;
        read_pos = 0;

        while (read_pos < read_len) {
            uint32_t cmd = read_uint32();
            switch (cmd) {
    """), file=ofp)
    for switch in switches:
        print(f'            {switch}', file=ofp)
    print(textwrap.dedent("""
                default: fprintf(stderr, "Unhandled cmd: %d\\n", cmd); exit(1);
            }
        }

        read_buf = NULL;
        read_len = 0;
        read_pos = 0;
    }
    """), file=ofp)


