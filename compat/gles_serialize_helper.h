
/**
 * apkenv
 * Copyright (c) 2023, Thomas Perl <m@thp.io>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/


static void
emit_pointer_glBufferData_data(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage)
{
    emit_bytes(data, size);
}

static const void *
read_pointer_glBufferData_data(GLenum target, GLsizeiptr size)
{
    return read_bytes(size);
}

static void
emit_pointer_glBufferSubData_data(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data)
{
    emit_bytes(data, size);
}

static const void *
read_pointer_glBufferSubData_data(GLenum target, GLintptr offset, GLsizeiptr size)
{
    return read_bytes(size);
}

static size_t
get_texture_image_size(GLsizei width, GLsizei height, GLenum format, GLenum type)
{
    int components = 1;
    switch (format) {
        case GL_ALPHA:
        case GL_LUMINANCE:
            components = 1;
            break;
        case GL_LUMINANCE_ALPHA:
            components = 2;
            break;
        case GL_RGB:
            components = 3;
            break;
        case GL_RGBA:
            components = 4;
            break;
        default:
            fprintf(stderr, "Unhandled format: 0x%04x\n", format);
            exit(1);
            break;
    }

    int size = 0;
    switch (type) {
        case GL_UNSIGNED_BYTE:
            size = components;
            break;
        case GL_UNSIGNED_SHORT_5_6_5:
        case GL_UNSIGNED_SHORT_4_4_4_4:
        case GL_UNSIGNED_SHORT_5_5_5_1:
            size = 2;
            break;
        default:
            fprintf(stderr, "Unhandled type: 0x%04x\n", type);
            exit(1);
            break;
    }

    // TODO: Take unpack alignment into account

    return width * height * size;
}

static void
emit_pointer_glTexImage2D_pixels(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
    size_t size = get_texture_image_size(width, height, format, type);
    emit_bytes(pixels, size);
}

static const void *
read_pointer_glTexImage2D_pixels(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type)
{
    size_t size = get_texture_image_size(width, height, format, type);

    return read_bytes(size);
}

static GLuint
array_buffer_binding = 0;

static GLuint
index_buffer_binding = 0;

static void
emit_draw_sync_begin()
{
}

static void
emit_draw_sync_end()
{
    // Restore buffer bindings after draw calls potentially overwrote them
    encode_glBindBuffer(GL_ARRAY_BUFFER, array_buffer_binding);
    encode_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_binding);
}

static void
serialize_hook_glBindBuffer(GLenum target, GLuint buffer)
{
    switch (target) {
        case GL_ARRAY_BUFFER:
            array_buffer_binding = buffer;
            break;
        case GL_ELEMENT_ARRAY_BUFFER:
            index_buffer_binding = buffer;
            break;
        default:
            fprintf(stderr, "Unhandled glBindBuffer(target=0x%04x)\n", buffer);
            break;
    }
}

static size_t
size_from_type(GLenum type)
{
    switch (type) {
        case GL_UNSIGNED_BYTE:
        case GL_BYTE:
            return 1;
        case GL_UNSIGNED_SHORT:
        case GL_SHORT:
            return 2;
        case GL_FIXED:
        case GL_FLOAT:
            return 4;
        default:
            fprintf(stderr, "Unhandled type: 0x%04x\n", type);
            exit(1);
    }

    return 0;
}

static void
emit_pointer_glDrawElements_indices(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
    emit_uint32(index_buffer_binding);

    if (index_buffer_binding == 0) {
        size_t size = count * size_from_type(type);
        emit_bytes(indices, size);
    } else {
        emit_uint32((intptr_t)indices);
    }
}

static const void *
read_pointer_glDrawElements_indices(GLenum mode, GLsizei count, GLenum type)
{
    uint32_t buffer_binding = read_uint32();

    // It's fine to overwrite this here, we'll restore it in emit_draw_sync_end()
    decoder_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_binding);

    if (buffer_binding == 0) {
        size_t size = count * size_from_type(type);
        return read_bytes(size);
    } else {
        return (const void *)(intptr_t)read_uint32();
    }
}

static void
emit_uint32_glGenTextures_textures(GLsizei n, GLuint *textures)
{
    // TODO: Actually generate index values here
    static GLuint seq = 1;
    for (int i=0; i<n; ++i) {
        textures[i] = seq++;
    }

    emit_bytes(textures, n * sizeof(GLuint));
}

static const GLuint *
read_uint32_glGenTextures_textures(GLsizei n)
{
    // TODO: Establish mapping between server and client IDs

    return read_bytes(sizeof(GLuint) * n);
}

#define emit_uint32_glGenBuffers_buffers emit_uint32_glGenTextures_textures
#define read_uint32_glGenBuffers_buffers read_uint32_glGenTextures_textures

static void
emit_uint32_glDeleteTextures_textures(GLsizei n, const GLuint *textures)
{
    emit_bytes(textures, n * sizeof(GLuint));
}

static const GLuint *
read_uint32_glDeleteTextures_textures(GLsizei n)
{
    // TODO: Establish mapping between server and client IDs

    return read_bytes(sizeof(GLuint) * n);
}

#define emit_uint32_glDeleteBuffers_buffers emit_uint32_glDeleteTextures_textures
#define read_uint32_glDeleteBuffers_buffers read_uint32_glDeleteTextures_textures

static void
emit_float_glMultMatrixf_m(const GLfloat *m)
{
    emit_bytes(m, sizeof(float) * 16);
}

static const float *
read_float_glMultMatrixf_m(void)
{
    return read_bytes(sizeof(float) * 16);
}

#define emit_float_glLoadMatrixf_m emit_float_glMultMatrixf_m
#define read_float_glLoadMatrixf_m read_float_glMultMatrixf_m


struct DelayedArrayPointer {
    GLint size;
    GLenum type;
    GLsizei stride;
    const GLvoid *pointer;
    size_t bytes_to_transfer;
    GLuint buffer_binding;
};

static size_t
stride_from_delayed_array_pointer(const struct DelayedArrayPointer *ptr)
{
    if (ptr->stride != 0) {
        return ptr->stride;
    }

    return ptr->size * size_from_type(ptr->type);
}

static struct DelayedArrayPointer
vertex_pointer = { 0, 0, 0, NULL, 0, 0 };

static void
encode_glVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    vertex_pointer = (struct DelayedArrayPointer){
        .size = size,
        .type = type,
        .stride = stride,
        .pointer = pointer,
        .bytes_to_transfer = 0,
        .buffer_binding = array_buffer_binding,
    };
}

static void
emit_array_pointer(struct DelayedArrayPointer *src, const GLvoid *pointer)
{
    emit_uint32(src->buffer_binding);

    if (src->buffer_binding == 0) {
        emit_uint32(src->bytes_to_transfer);
        emit_bytes(pointer, src->bytes_to_transfer);
    } else {
        emit_uint32((intptr_t)pointer);
    }
}

static const void *
read_array_pointer()
{
    uint32_t buffer_binding = read_uint32();

    // It's fine to overwrite this here, we'll restore it in emit_draw_sync_end()
    decoder_functions.glBindBuffer(GL_ARRAY_BUFFER, buffer_binding);

    if (buffer_binding == 0) {
        uint32_t length = read_uint32();
        return read_bytes(length);
    } else {
        return (const void *)(intptr_t)read_uint32();
    }
}

static void
emit_pointer_glVertexPointer_pointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    emit_array_pointer(&vertex_pointer, pointer);
}

static const void *
read_pointer_glVertexPointer_pointer(GLint size, GLenum type, GLsizei stride)
{
    return read_array_pointer();
}

static struct DelayedArrayPointer
color_pointer = { 0, 0, 0, NULL, 0, 0 };

static void
encode_glColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    color_pointer = (struct DelayedArrayPointer){
        .size = size,
        .type = type,
        .stride = stride,
        .pointer = pointer,
        .bytes_to_transfer = 0,
        .buffer_binding = array_buffer_binding,
    };
}

static void
emit_pointer_glColorPointer_pointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    emit_array_pointer(&color_pointer, pointer);
}

static const void *
read_pointer_glColorPointer_pointer(GLint size, GLenum type, GLsizei stride)
{
    return read_array_pointer();
}

static struct DelayedArrayPointer
normal_pointer = { 0, 0, 0, NULL, 0, 0 };

static void
encode_glNormalPointer(GLenum type, GLsizei stride, const GLvoid *pointer)
{
    normal_pointer = (struct DelayedArrayPointer){
        .size = 3,
        .type = type,
        .stride = stride,
        .pointer = pointer,
        .bytes_to_transfer = 0,
        .buffer_binding = array_buffer_binding,
    };
}

static void
emit_pointer_glNormalPointer_pointer(GLenum type, GLsizei stride, const GLvoid *pointer)
{
    emit_array_pointer(&normal_pointer, pointer);
}

static const void *
read_pointer_glNormalPointer_pointer(GLenum type, GLsizei stride)
{
    return read_array_pointer();
}

static struct DelayedArrayPointer
tex_coord_pointer = { 0, 0, 0, NULL, 0, 0 };

static void
encode_glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    tex_coord_pointer = (struct DelayedArrayPointer){
        .size = size,
        .type = type,
        .stride = stride,
        .pointer = pointer,
        .bytes_to_transfer = 0,
        .buffer_binding = array_buffer_binding,
    };
}

static void
emit_pointer_glTexCoordPointer_pointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    emit_array_pointer(&tex_coord_pointer, pointer);
}

static const void *
read_pointer_glTexCoordPointer_pointer(GLint size, GLenum type, GLsizei stride)
{
    return read_array_pointer();
}

static void
sync_draw_pointers(GLint begin_index, GLint end_index)
{
    // end index = index after last element we care about

    if (vertex_pointer.size != 0 && vertex_pointer.type != 0) {
        vertex_pointer.bytes_to_transfer = end_index * stride_from_delayed_array_pointer(&vertex_pointer);
        encode_glVertexPointer_delayed(vertex_pointer.size, vertex_pointer.type, vertex_pointer.stride, vertex_pointer.pointer);
    }

    if (color_pointer.size != 0 && color_pointer.type != 0) {
        color_pointer.bytes_to_transfer = end_index * stride_from_delayed_array_pointer(&color_pointer);
        encode_glColorPointer_delayed(color_pointer.size, color_pointer.type, color_pointer.stride, color_pointer.pointer);
    }

    if (normal_pointer.size != 0 && normal_pointer.type != 0) {
        normal_pointer.bytes_to_transfer = end_index * stride_from_delayed_array_pointer(&normal_pointer);
        encode_glNormalPointer_delayed(normal_pointer.type, normal_pointer.stride, normal_pointer.pointer);
    }

    if (tex_coord_pointer.size != 0 && tex_coord_pointer.type != 0) {
        tex_coord_pointer.bytes_to_transfer = end_index * stride_from_delayed_array_pointer(&tex_coord_pointer);
        encode_glTexCoordPointer_delayed(tex_coord_pointer.size, tex_coord_pointer.type, tex_coord_pointer.stride, tex_coord_pointer.pointer);
    }
}

static void
emit_draw_sync_glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
    sync_draw_pointers(first, first + count);
}

static void
emit_draw_sync_glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
    if (count == 0) {
        return;
    }

    // XXX: first and last isn't correct like this, but it's only used
    // as fallback if indices == NULL (TODO: implement reading from array buffers)
    GLint first = 0, last = count;

    switch (type) {
        case GL_UNSIGNED_BYTE:
            if (indices != NULL) {
                // FIXME: read indices from bound buffer
                const uint8_t *idx = indices;

                first = last = idx[0];
                for (GLsizei i=0; i<count; ++i) {
                    if (idx[i] < first) {
                        first = idx[i];
                    }
                    if (idx[i] > last) {
                        last = idx[i];
                    }
                }
            }
            break;
        case GL_UNSIGNED_SHORT:
            if (indices != NULL) {
                // FIXME: read indices from bound buffer
                const uint16_t *idx = indices;

                first = last = idx[0];
                for (GLsizei i=0; i<count; ++i) {
                    if (idx[i] < first) {
                        first = idx[i];
                    }
                    if (idx[i] > last) {
                        last = idx[i];
                    }
                }
            }
            break;
        default:
            fprintf(stderr, "Unhandled type: 0x%04x\n", type);
            exit(1);
    }

    sync_draw_pointers(first, last + 1);
}

const size_t
components_for_light_parameter(GLenum pname)
{
    switch (pname) {
        case GL_AMBIENT:
        case GL_DIFFUSE:
        case GL_SPECULAR:
        case GL_POSITION:
        case GL_EMISSION:
        case GL_AMBIENT_AND_DIFFUSE:
        case GL_LIGHT_MODEL_AMBIENT:
            return 4;
        case GL_SPOT_DIRECTION:
            return 3;
        case GL_SPOT_EXPONENT:
        case GL_SPOT_CUTOFF:
        case GL_CONSTANT_ATTENUATION:
        case GL_LINEAR_ATTENUATION:
        case GL_QUADRATIC_ATTENUATION:
        case GL_SHININESS:
        case GL_LIGHT_MODEL_TWO_SIDE:
            return 1;
        default:
            fprintf(stderr, "Unhandled light parameter: 0x%04x\n", pname);
            exit(1);
    }

    return 0;
}

static void
emit_float_glLightfv_params(GLenum light, GLenum pname, const GLfloat *params)
{
    emit_bytes(params, components_for_light_parameter(pname) * sizeof(float));
}

static const float *
read_float_glLightfv_params(GLenum light, GLenum pname)
{
    return read_bytes(components_for_light_parameter(pname) * sizeof(float));
}

static void
emit_float_glMaterialfv_params(GLenum face, GLenum pname, const GLfloat *params)
{
    emit_bytes(params, components_for_light_parameter(pname) * sizeof(float));
}

static const float *
read_float_glMaterialfv_params(GLenum face, GLenum pname)
{
    return read_bytes(components_for_light_parameter(pname) * sizeof(float));
}

static void
emit_float_glLightModelfv_params(GLenum pname, const GLfloat *params)
{
    emit_bytes(params, components_for_light_parameter(pname) * sizeof(float));
}

static const float *
read_float_glLightModelfv_params(GLenum pname)
{
    return read_bytes(components_for_light_parameter(pname) * sizeof(float));
}
