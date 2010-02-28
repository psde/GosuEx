uniform sampler2D texture0;

void main(void)
{
  vec4 Sepia1 = vec4( 0.2, 0.05, 0.0, 1.0 );    
  vec4 Sepia2 = vec4( 1.0, 0.9, 0.5, 1.0 );
 
  vec4 Color = texture2D(texture0, vec2(gl_TexCoord[0]));
 
  float SepiaMix = dot(vec3(0.3, 0.59, 0.11), vec3(Color));
  Color = mix(Color, vec4(SepiaMix), vec4(0.5));
  vec4 Sepia = mix(Sepia1, Sepia2, SepiaMix);
 
  gl_FragColor = mix(Color, Sepia, 1.0);

}
