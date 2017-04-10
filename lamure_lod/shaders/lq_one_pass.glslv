// Copyright (c) 2014 Bauhaus-Universitaet Weimar
// This Software is distributed under the Modified BSD License, see license.txt.
//
// Virtual Reality and Visualization Research Group 
// Faculty of Media, Bauhaus-Universitaet Weimar
// http://www.uni-weimar.de/medien/vr

layout (location = 0) in vec4 in_position;
layout (location = 1) in vec4 in_color;
layout (location = 2) in float in_radius;
layout (location = 3) in vec4 in_normal;

uniform mat4 inv_mv_matrix;
uniform float model_radius_scale;
uniform float point_size_factor;

out VertexData {
  //output to geometry shader
  vec3 pass_ms_u;
  vec3 pass_ms_v;

  vec3 pass_point_color;
  vec3 pass_normal;
} VertexOut;

void main()
{
  vec3 ms_n = normalize(in_normal.xyz);
  vec3 ms_u;

  //compute tangent vectors
  if(ms_n.z != 0.0) {
    ms_u = vec3( 1, 1, (-ms_n.x -ms_n.y)/ms_n.z);
  } else if (ms_n.y != 0.0) {
    ms_u = vec3( 1, (-ms_n.x -ms_n.z)/ms_n.y, 1);
  } else {
    ms_u = vec3( (-ms_n.y -ms_n.z)/ms_n.x, 1, 1);
  }

  //assign tangent vectors
  VertexOut.pass_ms_u = normalize(ms_u) * point_size_factor * model_radius_scale * in_radius;
  VertexOut.pass_ms_v = normalize(cross(ms_n, ms_u)) * point_size_factor * model_radius_scale * in_radius;

  VertexOut.pass_normal = normalize((inv_mv_matrix * vec4(in_normal.xyz, 0.0)).xyz );

  gl_Position = vec4(in_position.xyz, 1.0);

  VertexOut.pass_point_color = in_color.rgb;
}

