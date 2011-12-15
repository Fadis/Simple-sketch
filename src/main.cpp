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

#include <iostream>
#include <cstdlib>

#include <GL/glew.h>
#include <GL/glfw.h>

#include <IL/il.h>
#include <IL/ilu.h>

float prev_mouse_x = 0.0f;
float prev_mouse_y = 0.0f;
float mouse_x = 0.0f;
float mouse_y = 0.0f;
bool mouse_is_pressed = false;
bool quit_flag = false;

void traceMouse( int x, int y ) {
  prev_mouse_x = mouse_x; mouse_x = static_cast<float>( x ) / 256.0f - 1.0f;
  prev_mouse_y = mouse_y; mouse_y = static_cast<float>( 512 - y ) / 256.0f - 1.0f;
}

void checkMouseButton( int button, int action ) {
  if( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS )
    mouse_is_pressed = true;
  else if( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE )
    mouse_is_pressed = false;
}

void checkKeyboard( int key, int action ) {
  if( action == GLFW_PRESS && key == GLFW_KEY_ESC )
    quit_flag = true;
}

int main( int argc, char *argv[] ) {
  if( argc == 1 ) {
    std::cout << "Usage: " << argv[ 0 ] << " <filename>" << std::endl;
    exit( EXIT_SUCCESS );
  }
  glfwInit();
  glfwOpenWindow( 512, 512, 8, 8, 8, 8, 16, 0, GLFW_WINDOW );
  glfwSetMousePosCallback( &traceMouse );
  glfwSetMouseButtonCallback( &checkMouseButton );
  glfwSetKeyCallback( &checkKeyboard );
  GLenum err = glewInit();
  if ( GLEW_OK != err ) {
    std::cerr << "Error: GLEW: " << glewGetErrorString(err) << std::endl;
    abort();
  }
  ilInit();
  iluInit();
  FrameBuffer fb( 9 );

  glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
  glDrawBuffer( GL_FRONT );
  glClear(GL_COLOR_BUFFER_BIT );
  fb.begin();
  glClear(GL_COLOR_BUFFER_BIT );
  fb.end();
  while( !quit_flag ) {
    double begin_time = glfwGetTime();
    glViewport( 0, 0, 512, 512 );
    if( mouse_is_pressed ) {
      glBegin( GL_LINES );
      glColor4f( 0.0f, 0.0f, 0.0f, 1.0f );glVertex2f( prev_mouse_x, prev_mouse_y );
      glColor4f( 0.0f, 0.0f, 0.0f, 1.0f );glVertex2f( mouse_x, mouse_y );
      glEnd();
    }
    glFlush();
    fb.begin();
    if( mouse_is_pressed ) {
      glBegin( GL_LINES );
      glColor4f( 0.0f, 0.0f, 0.0f, 1.0f );glVertex2f( prev_mouse_x, prev_mouse_y );
      glColor4f( 0.0f, 0.0f, 0.0f, 1.0f );glVertex2f( mouse_x, mouse_y );
      glEnd();
    }
    glFlush();
    fb.end();
    glfwPollEvents();
    double end_time = glfwGetTime();
    glfwSleep( 1.0 / 60.0 - ( end_time - begin_time ) );
  }
  fb.dump( argv[ 1 ] );
  glfwCloseWindow();
  glfwTerminate();
}

