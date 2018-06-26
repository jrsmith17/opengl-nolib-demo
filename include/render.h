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