// Copyright (c) 2014 Bauhaus-Universitaet Weimar
// This Software is distributed under the Modified BSD License, see license.txt.
//
// Virtual Reality and Visualization Research Group 
// Faculty of Media, Bauhaus-Universitaet Weimar
// http://www.uni-weimar.de/medien/vr

#include <lamure/gl/array_buffer.h>

namespace lamure {
namespace gl {


array_buffer_t::array_buffer_t(
  uint32_t num_vertices,
  uint32_t vertex_size)
: buffer_t(num_vertices, vertex_size, vertex_size/sizeof(float32_t))
{
  glGenBuffers(1, &buffer_);
  glBindBuffer(GL_ARRAY_BUFFER, buffer_);

  glBufferData(GL_ARRAY_BUFFER, get_size(), 0, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}


array_buffer_t::~array_buffer_t()
{
  glDeleteBuffers(1, &buffer_);
}

void*
array_buffer_t::map(GLuint map_mode)
{
  glBindBuffer(GL_ARRAY_BUFFER, buffer_);
  void* mapped = (void*)glMapBuffer(GL_ARRAY_BUFFER, map_mode);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return mapped;
}

void
array_buffer_t::unmap()
{
  glBindBuffer(GL_ARRAY_BUFFER, buffer_);
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}


} // namespace gl
} // namespace lamure
