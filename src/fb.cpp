/***************************************************************************
 *   Copyright (C) 2011 by Naomasa Matsubayashi   *
 *   fadis@quaternion.sakura.ne.jp   *
 *                                                                         *
 *   All rights reserved.                                                  *
 *                                                                         *
 * Redistribution and use in source and binary forms, with or without      *
 * modification, are permitted provided that the following conditions are  *
 * met:                                                                    *
 *                                                                         *
 *  1. Redistributions of source code must retain the above copyright      *
 *     notice, this list of conditions and the following disclaimer.       *
 *  2. Redistributions in binary form must reproduce the above copyright   *
 *     notice, this list of conditions and the following disclaimer in the *
 *     documentation and/or other materials provided with the distribution.*
 *                                                                         *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS     *
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT       *
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A *
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT      *
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,   *
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT        *
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,   *
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY   *
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT     *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE   *
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.    *
 ***************************************************************************/

#include "fb.hpp"

#include <vector>
#include <iostream>

#include <IL/il.h>
#include <IL/ilu.h>

void checkIL() {
  ILenum error = ilGetError();
  if( IL_NO_ERROR != error ) {
    std::cerr << "Error: DevIL: " << iluErrorString( error ) << std::endl;
    abort();
  }
}

FrameBuffer::FrameBuffer( unsigned int _internal_size ) {
  internal_size = 1u << _internal_size;
  glGenTextures( 1, &name );
  glBindTexture( GL_TEXTURE_2D, name );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, internal_size, internal_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
  glGenRenderbuffers( 1, &depth_buffer );
  glBindRenderbuffer( GL_RENDERBUFFER, depth_buffer );
  glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, internal_size, internal_size );
  glGenFramebuffers( 1, &frame_buffer );
  glBindFramebuffer( GL_FRAMEBUFFER, frame_buffer );
  glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, depth_buffer );
  glBindFramebuffer( GL_FRAMEBUFFER, 0 );
  glBindRenderbuffer( GL_RENDERBUFFER, 0 );
}

void FrameBuffer::begin() const {
  if( GLEW_VERSION_1_5 ) {
    glBindRenderbuffer( GL_RENDERBUFFER, depth_buffer );
    glBindFramebuffer( GL_FRAMEBUFFER, frame_buffer );
    glViewport( 0, 0, internal_size, internal_size );
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, name, 0 );
  }
}
void FrameBuffer::end() const {
  glBindFramebuffer( GL_FRAMEBUFFER, 0 );
  glBindRenderbuffer( GL_RENDERBUFFER, 0 );
}
void FrameBuffer::dump( const std::string &filename ) const {
  glBindTexture( GL_TEXTURE_2D, name );
  std::vector< GLubyte > buffer;
  buffer.resize( internal_size * internal_size * 4 );
  glGetTexImage( GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)&buffer.front() );
  ilTexImage( internal_size, internal_size, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, (void*)&buffer.front() );
  checkIL();
  ilSaveImage( filename.c_str() );
  checkIL();
  glBindTexture( GL_TEXTURE_2D, 0 );
}
GLuint FrameBuffer::operator()() const {
  return name;
}
