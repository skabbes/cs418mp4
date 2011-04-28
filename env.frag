varying vec3 N;
varying vec3 V;

uniform sampler2D env;
uniform sampler2D texture;
uniform sampler2D mixture;

void main (void)
{  
   // Light, eye, and R
   vec3 L = normalize(gl_LightSource[0].position.xyz - V);   

   // we are in Eye Coordinates, so EyePos is (0,0,0)  
   vec3 E = normalize(-V); 
   vec3 R = normalize(-reflect(L,N));  

   vec4 envColor = texture2D(env, gl_TexCoord[1].xy);
   vec4 finalTexColor = envColor;
 
   //calculate Ambient Term:  
   vec4 Iamb = gl_FrontLightProduct[0].ambient;

   //calculate Diffuse Term:  
   vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);
   Idiff = clamp(Idiff, 0.0, 1.0);     
   
   // calculate Specular Term:
   vec4 Ispec = gl_FrontLightProduct[0].specular * pow(max(dot(R,E),0.0), gl_FrontMaterial.shininess);
   Ispec = clamp(Ispec, 0.0, 1.0); 
   vec4 color = gl_FrontLightModelProduct.sceneColor + Iamb + Idiff + Ispec;  

   // write Total Color:  
   float mixPercent = (color.r + color.g + color.b) / 3.0;
   vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
   gl_FragColor = mix(black, finalTexColor, mixPercent);

}
