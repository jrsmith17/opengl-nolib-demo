/*
License provided at the end of the file
*/

#ifndef RENDER_H
#define RENDER_H

#include <GL/GL.h>
#include <wglext.h>
#include <glcorearb.h>
#include <glext.h>

#define OPENGL_LIST \
    /*  ret, name, params */ \
	GLE(void, GenVertexArrays, GLsizei n, GLuint *arrays); \
	GLE(void, BindVertexArray, GLuint array); \
	GLE(void, GenBuffers, GLsizei n, GLuint *buffers); \
	GLE(void, BindBuffer, GLenum target, GLuint buffer); \
	GLE(void, BufferData, GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage); \
	GLE(void, EnableVertexAttribArray, GLuint index); \
	GLE(void, DisableVertexAttribArray, GLuint index); \
	GLE(void, VertexAttribPointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer); \
	GLE(GLuint, CreateShader, GLenum shaderType); \
	GLE(void, ShaderSource, GLuint shader, GLsizei count, const GLchar** string, const GLint* length); \
	GLE(void, CompileShader, GLuint shader); \
	GLE(void, GetShaderiv, GLuint shader, GLenum pname, GLint* params); \
	GLE(void, GetShaderInfoLog, GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog); \
	GLE(GLuint, CreateProgram); \
	GLE(void, AttachShader, GLuint program, GLuint shader); \
	GLE(void, LinkProgram, GLuint program); \
	GLE(void, GetProgramiv, GLuint program, GLenum pname, GLint* params); \
	GLE(void, GetProgramInfoLog, GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog); \
	GLE(void, DetachShader, GLuint program, GLuint shader); \
	GLE(void, DeleteShader, GLuint shader); \
	GLE(void, UseProgram, GLuint program); \
	GLE(GLenum, GetUniformLocation, GLuint program, const GLchar* name); \
	GLE(GLenum, UniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value); \
	GLE(GLenum, BufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data); \
	GLE(GLenum, ClearBufferiv, GLenum buffer, GLint drawbuffer, const GLint * value); \
	GLE(void, BufferStorage, GLenum target, GLsizeiptr size, const GLvoid * data, GLbitfield flags); \
	GLE(void, GenerateMipmap, GLenum target); \
	GLE(void, ActiveTexture, GLenum texture); \
	GLE(void, Uniform1i, GLint location, GLint v0); \
	GLE(void, DebugMessageCallback, GLDEBUGPROC callback, void * userParam); \
	GLE(void, DebugMessageControl, GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled); \

#define GLE(ret, name, ...) typedef ret name##proc(__VA_ARGS__); static name##proc * gl##name;
OPENGL_LIST
#undef GLE

#endif // !RENDER_H

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2018 JR Smith
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/