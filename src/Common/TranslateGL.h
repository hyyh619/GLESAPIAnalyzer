#ifndef __TRANSLATE_GL_H
#define __TRANSLATE_GL_H

#include <GLES3/gl31.h>
#include <GLES2/gl2ext.h>
#include "types.h"

#define CASE_GL(name) \
    case name: sprintf(output, #name); break;

GLvoid TranslateInternalFormat2String(gceSURF_FORMAT InFormat, char *output);
GLvoid TranslateImageFormat2TextureFormat(gceSURF_FORMAT InFormat, char *OutFormat);
GLvoid TranslateGL2GCFormat(GLenum Format, GLenum Type, char *ImageFormat, char *TextureFormat, char *Bpp);
GLvoid TranslateTexTarget(GLenum target, char *output);
GLvoid TranslateShaderType(GLenum type, char *output);
GLvoid TranslateShaderDataType(GLenum type, char *output);;
GLvoid TranslateUsageToString(GLenum usage, char *output);
GLvoid TranslateBufferTarget(GLenum target, char *output);
GLvoid TranslateAccessBit(GLbitfield access, GLchar *output);
GLvoid TranslateBlendFunc(GLenum BlendFuncGL, char *output);
GLvoid TranslateBlendMode(GLenum BlendModeGL, char *output);
GLvoid TranslateDrawMode(GLenum mode, char *output);
GLvoid TranslateCapability(GLenum cap, char *output, GLboolean flag);
GLvoid TranslateFunc(GLenum Func, char *output);
GLvoid TranslateAttachment(GLenum Attachment, char *output);
GLvoid TranslateFramebufferRenderbuffer(GLenum type, char *output);
GLvoid TranslateAttribStateName(GLenum state, char *output);
GLvoid TranslateRBOFormat(GLenum format, GLchar *output);
GLvoid TranslateError(GLenum error, char *output);
GLvoid TranslateShaderInfo(GLenum pname, char *output);
GLvoid TranslateProgramInfo(GLenum pname, char *output);
GLvoid TranslateFace(GLenum face, char *output);
GLvoid TranslateOperation(GLenum Operation, char *output);
GLvoid TranslateTexParameter(GLenum param, GLenum value, char *output);
GLvoid TranslateTexValue(GLenum value, char *output);
GLvoid TranslateDataType(GLenum type, char *output);
GLvoid TranslateDataTypeForSaveTexture(GLenum type, char *output);
GLvoid TranslateTexFormat(GLenum format, char *output);
GLvoid TranslateClearMask(int mask, char *output);
GLvoid TranslateTexParamName(GLenum param, char *output);
GLvoid TranslateTexValue(GLenum value, char *output);
GLvoid TranslatePixelStorei(GLenum Name, GLint Param, char *output);
GLvoid TranslateGetName(GLenum name, char *output);

#endif /* __TRANSLATE_GL_H */